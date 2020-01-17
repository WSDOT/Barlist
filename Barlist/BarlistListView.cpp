///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2020  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

// BarlistListView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BarlistListView.h"
#include "BarlistTreeView.h"
#include "CollaborationDoc.h"
#include "Events.h"

#include "GenerateMarkNumbersDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLIPFORMAT CBarlistListView::ms_cFormat = ::RegisterClipboardFormat(_T("Barlist.Bars"));

/////////////////////////////////////////////////////////////////////////////
// CBarlistListView

IMPLEMENT_DYNCREATE(CBarlistListView, CListView)

CBarlistListView::CBarlistListView()
{
   m_GroupIdx = -1;
   m_pTreeView = nullptr;
}

CBarlistListView::~CBarlistListView()
{
}


BEGIN_MESSAGE_MAP(CBarlistListView, CListView)
	//{{AFX_MSG_MAP(CBarlistListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
   ON_WM_CREATE()
   ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnBarRenamed)
   ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClicked)
   ON_UPDATE_COMMAND_UI(IDC_MOVE_UP, &CBarlistListView::OnUpdateMoveUp)
   ON_COMMAND(IDC_MOVE_UP, &CBarlistListView::OnMoveUp)
   ON_UPDATE_COMMAND_UI(IDC_MOVE_DOWN, &CBarlistListView::OnUpdateMoveDown)
   ON_COMMAND(IDC_MOVE_DOWN, &CBarlistListView::OnMoveDown)
   ON_UPDATE_COMMAND_UI(IDC_DELETE, &CBarlistListView::OnUpdateDelete)
   ON_COMMAND(IDC_DELETE, &CBarlistListView::OnDelete)
   ON_UPDATE_COMMAND_UI(IDC_RENAME, &CBarlistListView::OnUpdateRename)
   ON_COMMAND(IDC_RENAME, &CBarlistListView::OnRename)
   ON_NOTIFY_REFLECT(NM_DBLCLK, &CBarlistListView::OnNMDblclk)
   ON_WM_LBUTTONDOWN()
   ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CBarlistListView::OnUpdateEditCut)
   ON_COMMAND(ID_EDIT_CUT, &CBarlistListView::OnEditCut)
   ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CBarlistListView::OnUpdateEditCopy)
   ON_COMMAND(ID_EDIT_COPY, &CBarlistListView::OnEditCopy)
   ON_UPDATE_COMMAND_UI(ID_GENERATE_MARK_NUMBERS, &CBarlistListView::OnUpdateGenerateMarkNumbers)
   ON_COMMAND(ID_GENERATE_MARK_NUMBERS, &CBarlistListView::OnGenerateMarkNumbers)
   ON_WM_DESTROY()
   ON_NOTIFY_REFLECT(NM_RCLICK, &CBarlistListView::OnNMRClick)
   ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarlistListView drawing

void CBarlistListView::OnDraw(CDC* pDC)
{
   // drawing is used for printing... printing is delegated to the tree view
   m_pTreeView->OnDraw(pDC);
}

BOOL CBarlistListView::PreCreateWindow(CREATESTRUCT& cs)
{
   cs.style |= LVS_REPORT | LVS_EDITLABELS;
   return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBarlistListView diagnostics

#ifdef _DEBUG
void CBarlistListView::AssertValid() const
{
   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CListView::AssertValid();
}

void CBarlistListView::Dump(CDumpContext& dc) const
{
   CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBarlistListView message handlers


int CBarlistListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CListView::OnCreate(lpCreateStruct) == -1)
      return -1;

   // TODO:  Add your specialized creation code here
   CListCtrl& list = GetListCtrl();
   list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

   return 0;
}


void CBarlistListView::OnInitialUpdate()
{
   CListView::OnInitialUpdate();


   CListCtrl& list = GetListCtrl();

   RECT rect;
   list.GetClientRect(&rect);
   int width = rect.right / 13;

   int col = 0;
   list.InsertColumn(col++, _T("Mark No."), LVCFMT_LEFT, width);
   list.InsertColumn(col++, _T("Location"), LVCFMT_LEFT, 2*width);
   list.InsertColumn(col++, _T("Size"), LVCFMT_CENTER, width);
   list.InsertColumn(col++, _T("No. Reqd"), LVCFMT_LEFT, width);
   list.InsertColumn(col++, _T("Bend Type"), LVCFMT_LEFT, width);
   list.InsertColumn(col++, _T("Use"), LVCFMT_LEFT, width);
   list.InsertColumn(col++, _T("Lump Sum"), LVCFMT_LEFT, width);
   list.InsertColumn(col++, _T("Substructure"), LVCFMT_LEFT, width);
   list.InsertColumn(col++, _T("Epoxy"), LVCFMT_LEFT, width);
   list.InsertColumn(col++, _T("Varies"), LVCFMT_LEFT, width);
   list.InsertColumn(col++, _T("Length"), LVCFMT_LEFT, width);
   list.InsertColumn(col++, _T("Wgt"), LVCFMT_LEFT, width);

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   m_ImageList.Create(12, 12, ILC_COLOR4, 3, 1);
   auto hInstance = AfxGetInstanceHandle();
   int result = m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GREEN)));
   ATLASSERT(result == 0);
   result = m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YELLOW)));
   ATLASSERT(result == 1);
   result = m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RED)));
   ATLASSERT(result == 2);

   list.SetImageList(&m_ImageList, LVSIL_SMALL);
}

void CBarlistListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   if (lHint == HINT_GROUP_ADDED && m_GroupIdx < 0)
   {
      m_GroupIdx = 0;
   }

   if (lHint == HINT_GROUP_REMOVED)
   {
      m_GroupIdx = -1;
   }

   if (lHint == HINT_BAR_CHANGED)
   {
      CBarRecordEventHint* pBarHint = (CBarRecordEventHint*)pHint;
      CComPtr<IBarRecordCollection> bars;
      pBarHint->group->get_BarRecords(&bars);
      long nBars;
      bars->get_Count(&nBars);
      for (long barIdx = 0; barIdx < nBars; barIdx++)
      {
         CComPtr<IBarRecord> bar;
         bars->get_Item(CComVariant(barIdx), &bar);
         if (bar.IsEqualObject(pBarHint->pBarRecord))
         {
            SetBarRecord(barIdx, pBarHint->pBarRecord);
         }
      }
   }

   if (lHint == EAF_HINT_UNITS_CHANGED || lHint == HINT_BAR_ADDED || lHint == HINT_BAR_MOVED || lHint == HINT_BAR_REMOVED)
   {
      if (m_GroupIdx != -1)
      {
         OnGroupSelected(m_GroupIdx);
      }
   }
   __super::OnUpdate(pSender, lHint, pHint);
}

CBarlistDoc* CBarlistListView::GetDocument()
{
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBarlistDoc)));
   return (CBarlistDoc*)m_pDocument;
}

void CBarlistListView::SetTreeView(CBarlistTreeView* pTreeView)
{
   m_pTreeView = pTreeView;
}

void CBarlistListView::SelectAll()
{
   CListCtrl& list = GetListCtrl();
   list.SetFocus();
   int nItems = list.GetItemCount();
   for (int i = 0; i < nItems; i++)
   {
      list.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
   }
}

long CBarlistListView::GetSelectedBar()
{
   CListCtrl& list = GetListCtrl();
   POSITION pos = list.GetFirstSelectedItemPosition();
   if (pos == NULL)
   {
      return -1;
   }
   else
   {
      int nItem = list.GetNextSelectedItem(pos);
      return (long)list.GetItemData(nItem);
   }
}

void CBarlistListView::OnGroupSelected(long groupIdx)
{
   USES_CONVERSION;

   m_GroupIdx = groupIdx;
   CListCtrl& list = GetListCtrl();
   list.DeleteAllItems();

   if (m_GroupIdx == -1)
   {
      // an actual group isn't selected so just return
      return;
   }

   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);

   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);

   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(groupIdx), &group);

   CComPtr<IBarRecordCollection> barRecords;
   group->get_BarRecords(&barRecords);

   long nBarRecords;
   barRecords->get_Count(&nBarRecords);
   int row = 0;
   for (long barRecordIdx = 0; barRecordIdx < nBarRecords; barRecordIdx++, row++)
   {
      CComPtr<IBarRecord> barRecord;
      barRecords->get_Item(CComVariant(barRecordIdx), &barRecord);

      CComBSTR bstrMark;
      barRecord->get_Mark(&bstrMark);
      list.InsertItem(row, OLE2T(bstrMark));
      list.SetItemData(row, barRecordIdx);

      SetBarRecord(row, barRecord);
   }
}

void CBarlistListView::SetBarRecord(int row, IBarRecord* pBarRecord)
{
   USES_CONVERSION;
   CListCtrl& list = GetListCtrl();

   CComBSTR bstrMark;
   pBarRecord->get_Mark(&bstrMark);
   list.SetItemText(row, 0, OLE2T(bstrMark));

   StatusType status;
   pBarRecord->get_Status(&status);
   VERIFY(list.SetItem(row, 0, LVIF_IMAGE, nullptr, (int)status, 0, 0, 0, 0));

   int subItem = 1;
   CComBSTR bstrLocation;
   pBarRecord->get_Location(&bstrLocation);
   list.SetItemText(row, subItem++, OLE2T(bstrLocation));

   CComBSTR bstrSize;
   pBarRecord->get_Size(&bstrSize);
   list.SetItemText(row, subItem++, OLE2T(bstrSize));

   long nReqd;
   pBarRecord->get_NumReqd(&nReqd);
   CString strReqd;
   strReqd.Format(_T("%d"), nReqd);
   list.SetItemText(row, subItem++, strReqd);

   long bendType;
   pBarRecord->get_BendType(&bendType);
   CString strBendType;
   strBendType.Format(_T("%d"), bendType);
   list.SetItemText(row, subItem++, strBendType);

   // use
   UseType use;
   pBarRecord->get_Use(&use);
   CString strUse("");
   if (use == utTransverse)
   {
      strUse = _T("T");
   }
   else if (use == utSeismic)
   {
      strUse = _T("S");
   }
   list.SetItemText(row, subItem++, strUse);

   VARIANT_BOOL vbFlag;
   pBarRecord->get_LumpSum(&vbFlag);
   list.SetItemText(row, subItem++, (vbFlag == VARIANT_TRUE ? _T("L") : _T("")));

   pBarRecord->get_Substructure(&vbFlag);
   list.SetItemText(row, subItem++, (vbFlag == VARIANT_TRUE ? _T("S") : _T("")));

   pBarRecord->get_Epoxy(&vbFlag);
   list.SetItemText(row, subItem++, (vbFlag == VARIANT_TRUE ? _T("E") : _T("")));

   pBarRecord->get_Varies(&vbFlag);
   list.SetItemText(row, subItem++, (vbFlag == VARIANT_TRUE ? _T("V") : _T("")));

   // length
   CComPtr<IBend> primaryBend;
   pBarRecord->get_PrimaryBend(&primaryBend);
   double length;
   primaryBend->get_Length(&length);
   CString strLength = FormatLength(length);
   list.SetItemText(row, subItem++, strLength);

   double mass;
   pBarRecord->get_Mass(&mass);
   CString strMass = FormatMass(mass);
   list.SetItemText(row, subItem++, strMass);
}


void CBarlistListView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
   // printing is delegated to the tree view class
   m_pTreeView->OnPrint(pDC, pInfo);
}

BOOL CBarlistListView::OnPreparePrinting(CPrintInfo* pInfo)
{
   // printing is delegated to the tree view class
   return m_pTreeView->OnPreparePrinting(pInfo);
}

void CBarlistListView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
   // printing is delegated to the tree view class
   m_pTreeView->OnBeginPrinting(pDC, pInfo);
}

void CBarlistListView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
   // printing is delegated to the tree view class
   m_pTreeView->OnEndPrinting(pDC, pInfo);
}

void CBarlistListView::OnBarRenamed(NMHDR* pNMHDR, LRESULT* pResult)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   NMLVDISPINFO* pInfo = (NMLVDISPINFO*)pNMHDR;
   if (pInfo->item.pszText != nullptr)
   {
      CListCtrl& list = GetListCtrl();
      long barRecordIdx = (long)list.GetItemData(pInfo->item.iItem);

      CBarlistDoc* pDoc = GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);
      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(m_GroupIdx), &group);
      CComPtr<IBarRecordCollection> bars;
      group->get_BarRecords(&bars);
      CComPtr<IBarRecord> bar;
      bars->get_Item(CComVariant(barRecordIdx), &bar);
      bar->put_Mark(pInfo->item.pszText);
   }
}

void CBarlistListView::OnColumnClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   NMLISTVIEW* pInfo = (NMLISTVIEW*)pNMHDR;
   if (pInfo->iSubItem == 0)
   {
      CBarlistDoc* pDoc = GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);
      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(m_GroupIdx), &group);
      CComPtr<IBarRecordCollection> bars;
      group->get_BarRecords(&bars);
      bars->Sort();
      
      OnGroupSelected(m_GroupIdx);
   }
}

void CBarlistListView::OnUpdateMoveUp(CCmdUI *pCmdUI)
{
   CListCtrl& list = GetListCtrl();
   UINT nSelected = list.GetSelectedCount();
   pCmdUI->Enable(0 < nSelected);
}

void CBarlistListView::OnMoveUp()
{
   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);
   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);

   CListCtrl& list = GetListCtrl();
   POSITION pos = list.GetFirstSelectedItemPosition();
   std::vector<int> vItems;
   while (pos)
   {
      bars->MoveUp(CComVariant((long)list.GetItemData(list.GetNextSelectedItem(pos))));
   }
}

void CBarlistListView::OnUpdateMoveDown(CCmdUI *pCmdUI)
{
   CListCtrl& list = GetListCtrl();
   UINT nSelected = list.GetSelectedCount();
   pCmdUI->Enable(0 < nSelected);
}

void CBarlistListView::OnMoveDown()
{
   // bar records are moved by index, therefore, they  must be move in the
   // reverse order of the selected list when moving bars down
   CListCtrl& list = GetListCtrl();
   POSITION pos = list.GetFirstSelectedItemPosition();
   std::vector<int> vItems;
   while (pos)
   {
      vItems.push_back(list.GetNextSelectedItem(pos));
   }

   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);
   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);

   // use reverse iterators to traverse the vector backwards
   auto iter = vItems.rbegin();
   auto end = vItems.rend();
   for (; iter != end; iter++)
   {
      long barRecordIdx = (long)list.GetItemData(*iter);

      bars->MoveDown(CComVariant(barRecordIdx));
   }
}

void CBarlistListView::OnUpdateDelete(CCmdUI *pCmdUI)
{
   CListCtrl& list = GetListCtrl();
   UINT nSelected = list.GetSelectedCount();
   pCmdUI->Enable(0 < nSelected);
}

void CBarlistListView::OnDelete()
{
   // bar records are removed by index, therefore, they  must be removed in the
   // reverse order of the selected list
   CListCtrl& list = GetListCtrl();
   POSITION pos = list.GetFirstSelectedItemPosition();
   std::vector<int> vItems;
   while (pos)
   {
      vItems.push_back(list.GetNextSelectedItem(pos));
   }

   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);
   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);

   // use reverse iterators to traverse the vector backwards
   auto iter = vItems.rbegin();
   auto end = vItems.rend();
   for (; iter != end; iter++)
   {
      long barRecordIdx = (long)list.GetItemData(*iter);

      bars->Remove(CComVariant(barRecordIdx));
   }
}

void CBarlistListView::OnUpdateRename(CCmdUI *pCmdUI)
{
   CListCtrl& list = GetListCtrl();
   UINT nSelected = list.GetSelectedCount();
   pCmdUI->Enable(nSelected == 1);
}

void CBarlistListView::OnRename()
{
   CListCtrl& list = GetListCtrl();
   POSITION pos = list.GetFirstSelectedItemPosition();
   int nItem = list.GetNextSelectedItem(pos);
   list.EditLabel(nItem);
}

void CBarlistListView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
   LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
   *pResult = 0;

   CBarlistDoc* pDoc = GetDocument();
   long groupIdx = m_pTreeView->GetSelectedGroup();
   long barIdx = GetSelectedBar();
   pDoc->EditBar(groupIdx,barIdx);
}

void CBarlistListView::CacheBarlistClipboardData(COleDataSource& dataSource)
{
   FreeBarlistClipboardData();

   // cache the selected data for use in clipboard and drag/drop operations
   HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(CBarlistClipboardData));
   CBarlistClipboardData* pCBD = (CBarlistClipboardData*)::GlobalLock(hGlobal);
   pCBD->nThreadID = AfxGetThread()->m_nThreadID;
   pCBD->sourceGroupIdx = m_GroupIdx;
   pCBD->bars.CoCreateInstance(CLSID_BarRecordCollection);

   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);
   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);

   CListCtrl& list = GetListCtrl();
   POSITION pos = list.GetFirstSelectedItemPosition();
   while (pos)
   {
      int item = list.GetNextSelectedItem(pos);
      long barRecordIdx = (long)list.GetItemData(item);

      CComPtr<IBarRecord> bar;
      bars->get_Item(CComVariant(barRecordIdx), &bar);

      CComPtr<IBarRecord> clone;
      pDoc->CopyBar(bar, &clone);
      pCBD->bars->Add(clone);
   }

   ::GlobalUnlock(hGlobal);
   dataSource.CacheGlobalData(CBarlistListView::ms_cFormat, hGlobal);
}

void CBarlistListView::OnLButtonDown(UINT nFlags, CPoint point)
{
   // if we begin the drag and drop on left button down, left button double click messages
   // never get through

   // Drag and drop will only occur if the mouse is on the icon in the list view
   CListCtrl& list = GetListCtrl();
   int idx = list.HitTest(point);
   if (idx != -1)
   {
      CRect rect;
      list.GetItemRect(idx, &rect, LVIR_ICON);
      if (rect.PtInRect(point))
      {
         COleDataSource dataSource;
         CacheBarlistClipboardData(dataSource);

         if (dataSource.DoDragDrop() == DROPEFFECT_MOVE)
         {
            // the bars were moved, so remove them from their current location
            LPDATAOBJECT lpDataObject = (LPDATAOBJECT)dataSource.GetInterface(&IID_IDataObject);
            COleDataObject dataObj;
            dataObj.Attach(lpDataObject, FALSE);
            HGLOBAL hGlobal = dataObj.GetGlobalData(CBarlistListView::ms_cFormat);
            CBarlistClipboardData* pCBD = (CBarlistClipboardData*)::GlobalLock(hGlobal);

            CBarlistDoc* pDoc = GetDocument();
            CComPtr<IBarlist> barlist;
            pDoc->GetBarlist(&barlist);
            CComPtr<IGroupCollection> groups;
            barlist->get_Groups(&groups);
            CComPtr<IGroup> group;
            groups->get_Item(CComVariant(pCBD->sourceGroupIdx), &group);
            CComPtr<IBarRecordCollection> bars;
            group->get_BarRecords(&bars);

            long nBars;
            pCBD->bars->get_Count(&nBars);
            for (long barIdx = 0; barIdx < nBars; barIdx++)
            {
               CComPtr<IBarRecord> bar;
               pCBD->bars->get_Item(CComVariant(barIdx), &bar);

               CComBSTR bstrMark;
               bar->get_Mark(&bstrMark);
               bars->Remove(CComVariant(bstrMark));
            }

            ::GlobalUnlock(hGlobal);
         }
      }
   }

   CListView::OnLButtonDown(nFlags, point);
}


void CBarlistListView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
   CListCtrl& list = GetListCtrl();
   pCmdUI->Enable(0 < list.GetSelectedCount());
}


void CBarlistListView::OnEditCut()
{
   // cache the data that is being cut
   COleDataSource* pDataSource = new COleDataSource;
   CacheBarlistClipboardData(*pDataSource);
   pDataSource->SetClipboard();

   // do the cut
   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);
   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);

   // because we are working with index, we have
   // to remove from the collection in reverse order
   // capture the indicies of the selected bar records...
   CListCtrl& list = GetListCtrl();
   POSITION pos = list.GetFirstSelectedItemPosition();
   std::vector<long> vBars;
   while (pos)
   {
      int item = list.GetNextSelectedItem(pos);
      long barRecordIdx = (long)list.GetItemData(item);
      vBars.push_back(barRecordIdx);
   }

   // ... working in reverse order, remove by index
   auto iter = vBars.rbegin();
   auto end = vBars.rend();
   for ( ; iter != end; iter++)
   {
      long barRecordIdx = *iter;
      bars->Remove(CComVariant(barRecordIdx));
   }
}

void CBarlistListView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
   CListCtrl& list = GetListCtrl();
   pCmdUI->Enable(0 < list.GetSelectedCount());
}

void CBarlistListView::OnEditCopy()
{
   COleDataSource* pDataSource = new COleDataSource;
   CacheBarlistClipboardData(*pDataSource);
   pDataSource->SetClipboard();
}


void CBarlistListView::OnUpdateGenerateMarkNumbers(CCmdUI *pCmdUI)
{
   CListCtrl& list = GetListCtrl();
   pCmdUI->Enable(1 < list.GetSelectedCount());
}


void CBarlistListView::OnGenerateMarkNumbers()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGenerateMarkNumbersDlg dlg;
   if (dlg.DoModal() == IDOK)
   {
      CBarlistDoc* pDoc = GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);
      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(m_GroupIdx), &group);
      CComPtr<IBarRecordCollection> bars;
      group->get_BarRecords(&bars);

      CListCtrl& list = GetListCtrl();
      int n = list.GetSelectedCount();
      POSITION pos = list.GetFirstSelectedItemPosition();
      long mark = dlg.m_First;
      while (pos)
      {
         int item = list.GetNextSelectedItem(pos);
         long barRecordIdx = (long)list.GetItemData(item);

         CComPtr<IBarRecord> bar;
         bars->get_Item(CComVariant(barRecordIdx), &bar);

         CString strMark;
         strMark.Format(_T("%d"), mark);

         bar->put_Mark(CComBSTR(strMark));

         mark += dlg.m_Increment;
      }
   }
}

void CBarlistListView::FreeBarlistClipboardData()
{
   COleDataObject dataObj;
   if (dataObj.AttachClipboard() && dataObj.IsDataAvailable(CBarlistListView::ms_cFormat))
   {
      HGLOBAL hGlobal = dataObj.GetGlobalData(CBarlistListView::ms_cFormat);
      ::GlobalFree(hGlobal);
   }
}

void CBarlistListView::OnDestroy()
{
   FreeBarlistClipboardData();

   CListView::OnDestroy();

   // TODO: Add your message handler code here
}


void CBarlistListView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
   CBarlistDoc* pDoc = GetDocument();
   if (pDoc->IsKindOf(RUNTIME_CLASS(CCollaborationDoc)))
   {
      // no context editing for collaboration projects
      return;
   }

   LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CMenu menu;
   menu.LoadMenu(IDR_LIST_CONTEXT);

   CPoint point = pNMItemActivate->ptAction;
   ClientToScreen(&point);
   menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, EAFGetMainFrame());
   *pResult = 0;
}
