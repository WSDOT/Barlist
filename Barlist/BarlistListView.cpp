///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2024  Washington State Department of Transportation
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
#include "BarlistFrame.h"
#include "CollaborationDoc.h"
#include "Events.h"
#include "Helpers.h"

#include "GenerateMarkNumbersDlg.h"

#include "Barlist.hxx"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLIPFORMAT CBarlistListView::ms_cBarFormat = ::RegisterClipboardFormat(_T("Barlist.Bars"));
CLIPFORMAT CBarlistListView::ms_cGroupFormat = ::RegisterClipboardFormat(_T("Barlist.Group"));

/////////////////////////////////////////////////////////////////////////////
// CBarlistListView

IMPLEMENT_DYNCREATE(CBarlistListView, CListView)

CBarlistListView::CBarlistListView()
{
   m_GroupIdx = -1;
   m_pTreeView = nullptr;
   m_pFrame = nullptr;
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
   ON_WM_RBUTTONDOWN()
   ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CBarlistListView::OnUpdateEditCut)
   ON_COMMAND(ID_EDIT_CUT, &CBarlistListView::OnEditCut)
   ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CBarlistListView::OnUpdateEditCopy)
   ON_COMMAND(ID_EDIT_COPY, &CBarlistListView::OnEditCopy)
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

   m_DropTarget.Register(this);

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
   list.InsertColumn(col++, _T("Material"), LVCFMT_LEFT, width);
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

void CBarlistListView::SetFrame(CBarlistFrame* pFrame)
{
   m_pFrame = pFrame;
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

   MaterialType material;
   pBarRecord->get_Material(&material);
   CString strMaterial = GetMaterialSpecification(material);
   list.SetItemText(row, subItem++, strMaterial);

   VARIANT_BOOL vbFlag;
   pBarRecord->get_Substructure(&vbFlag);
   list.SetItemText(row, subItem++, (vbFlag == VARIANT_TRUE ? _T("S") : _T("")));

   pBarRecord->get_Epoxy(&vbFlag);
   list.SetItemText(row, subItem++, (vbFlag == VARIANT_TRUE ? _T("E") : _T("")));

   pBarRecord->get_Varies(&vbFlag);
   list.SetItemText(row, subItem++, (vbFlag == VARIANT_TRUE ? _T("V") : _T("")));

   // length
   CComPtr<IBend> primaryBend;
   pBarRecord->get_PrimaryBend(&primaryBend);
   Float64 length;
   primaryBend->get_Length(&length);
   CString strLength = Formatter::FormatLength(length);
   list.SetItemText(row, subItem++, strLength);

   if (material != D7957)
   {
      Float64 mass;
      pBarRecord->get_Mass(&mass);
      CString strMass = Formatter::FormatMass(mass);
      list.SetItemText(row, subItem++, strMass);
   }
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
   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);
   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);
   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(m_GroupIdx), &group);
   CComPtr<IBarRecordCollection> bars;
   group->get_BarRecords(&bars);

   CComBSTR bstrName;
   group->get_Name(&bstrName);

   CComPtr<IBarlist> source_barlist;
   source_barlist.CoCreateInstance(CLSID_Barlist);
   CComPtr<IGroupCollection> source_groups;
   source_barlist->get_Groups(&source_groups);
   source_groups->Add(bstrName);
   CComPtr<IGroup> source_group;
   source_groups->get_Item(CComVariant(0), &source_group);
   CComPtr<IBarRecordCollection> source_bars;
   source_group->get_BarRecords(&source_bars);

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

      source_bars->Add(clone);
   }

   Barlist barlist_xml = pDoc->CreateXML(source_barlist);
   std::ostringstream os;
   Barlist_(os, barlist_xml, xml_schema::namespace_infomap(), _T("UTF-8"));

   size_t size = os.str().size();
   size++;
   HGLOBAL hGlobal = ::GlobalAlloc(GHND, size);
   LPSTR strXML = (LPSTR)::GlobalLock(hGlobal);
   ::strcpy_s(strXML, os.str().size()+1, os.str().c_str());
   strXML[os.str().size()] = 0; // null terminate the string
   ::GlobalUnlock(hGlobal);
   dataSource.CacheGlobalData(CBarlistListView::ms_cBarFormat, hGlobal);
}

BOOL CBarlistListView::MouseButtonDrag(UINT nFlags, CPoint point)
{
   // if we begin the drag and drop on left button down, left button dbl click messages
   // never get through
   BOOL bReturn = FALSE;
   CBarlistDoc* pDoc = GetDocument();
   if (!pDoc->IsKindOf(RUNTIME_CLASS(CCollaborationDoc)))
   {
      // Drag and drop will only occur if the mouse is on the icon in the list view
      CListCtrl& list = GetListCtrl();
      int idx = list.HitTest(point);
      if (idx != -1)
      {
         CRect rect;
         list.GetItemRect(idx, &rect, LVIR_ICON);
         if (rect.PtInRect(point))
         {
            list.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);

            COleDataSource dataSource;
            CacheBarlistClipboardData(dataSource);

            if (dataSource.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE) == DROPEFFECT_MOVE)
            {
               // the bars were moved, so remove them from their current location
               LPDATAOBJECT lpDataObject = (LPDATAOBJECT)dataSource.GetInterface(&IID_IDataObject);
               COleDataObject dataObj;
               dataObj.Attach(lpDataObject, FALSE);
               HGLOBAL hGlobal = dataObj.GetGlobalData(CBarlistListView::ms_cBarFormat);

               // reconstitute the source barlist data from the XML string
               LPCSTR strXML = (LPCSTR)::GlobalLock(hGlobal);
               CComPtr<IBarlist> source_barlist;
               pDoc->CreateBarlist(strXML, &source_barlist);
               ::GlobalUnlock(hGlobal);

               // get the collection of bar records to paste
               CComPtr<IGroupCollection> source_groups;
               source_barlist->get_Groups(&source_groups);
               CComPtr<IGroup> source_group;
               source_groups->get_Item(CComVariant(0), &source_group);
               CComPtr<IBarRecordCollection> source_bars;
               source_group->get_BarRecords(&source_bars);

               CComBSTR bstrName;
               source_group->get_Name(&bstrName);

               CComPtr<IBarlist> barlist;
               pDoc->GetBarlist(&barlist);
               CComPtr<IGroupCollection> groups;
               barlist->get_Groups(&groups);
               CComPtr<IGroup> group;
               groups->get_Item(CComVariant(bstrName), &group);
               CComPtr<IBarRecordCollection> bars;
               group->get_BarRecords(&bars);


               long nBars;
               source_bars->get_Count(&nBars);
               for (long barIdx = 0; barIdx < nBars; barIdx++)
               {
                  CComPtr<IBarRecord> bar;
                  source_bars->get_Item(CComVariant(barIdx), &bar);

                  CComBSTR bstrMark;
                  bar->get_Mark(&bstrMark);
                  bars->Remove(CComVariant(bstrMark));
               }
            }
         
            bReturn = TRUE;
         }
      }
   }
   return bReturn;
}

void CBarlistListView::OnRButtonDown(UINT nFlags, CPoint point)
{
   if ( !MouseButtonDrag(nFlags, point) )
      CListView::OnRButtonDown(nFlags, point);
}

void CBarlistListView::OnLButtonDown(UINT nFlags, CPoint point)
{
   if ( !MouseButtonDrag(nFlags, point) )
      CListView::OnLButtonDown(nFlags, point);
}

DROPEFFECT CBarlistListView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
   m_bRMouse = (dwKeyState == VK_RBUTTON);
   return CListView::OnDragEnter(pDataObject, dwKeyState, point);
}

void CBarlistListView::OnDragLeave()
{
   m_bRMouse = false;
   CListView::OnDragLeave();
}

DROPEFFECT CBarlistListView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
   CBarlistDoc* pDoc = GetDocument();
   if (pDoc->IsKindOf(RUNTIME_CLASS(CCollaborationDoc)))
   {
      // can't drop onto a collaboration document
      return DROPEFFECT_NONE;
   }

   if (pDataObject->IsDataAvailable(CBarlistListView::ms_cBarFormat))
   {
      return m_GroupIdx == -1 ? DROPEFFECT_NONE : ((dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE);
   }

   return CListView::OnDragOver(pDataObject, dwKeyState, point);
}

DROPEFFECT CBarlistListView::OnDropEx(COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point)
{
   if (pDataObject->IsDataAvailable(CBarlistListView::ms_cBarFormat))
   {
      DROPEFFECT deResult = dropDefault;

      if (m_bRMouse)
      {
         AFX_MANAGE_STATE(AfxGetStaticModuleState());
         ClientToScreen(&point);
         CMenu menu;
         VERIFY(menu.LoadMenu(IDR_DROP));

         CMenu* pPopup = menu.GetSubMenu(0);
         ASSERT(pPopup != nullptr);

         pPopup->SetDefaultItem(dropDefault == DROPEFFECT_COPY ? 0 : 1,MF_BYPOSITION);
         BOOL nCmd = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
         if (nCmd == 0)
         {
            // user cancelled the menu
            return DROPEFFECT_NONE;
         }

         deResult = (nCmd == ID_EDIT_COPY ? DROPEFFECT_COPY : DROPEFFECT_MOVE);
      }

      CBarlistDoc* pDoc = GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);

      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);

      CComPtr<IGroup> targetGroup;
      groups->get_Item(CComVariant(m_GroupIdx), &targetGroup);

      CComPtr<IBarRecordCollection> targetBars;
      targetGroup->get_BarRecords(&targetBars);

      // reconstitute the source barlist data from the XML string
      HGLOBAL hGlobal = pDataObject->GetGlobalData(CBarlistListView::ms_cBarFormat);
      LPCSTR strXML = (LPCSTR)::GlobalLock(hGlobal);
      CComPtr<IBarlist> source_barlist;
      pDoc->CreateBarlist(strXML, &source_barlist);
      ::GlobalUnlock(hGlobal);

      // get the collection of bar records to paste
      CComPtr<IGroupCollection> source_groups;
      source_barlist->get_Groups(&source_groups);
      CComPtr<IGroup> source_group;
      source_groups->get_Item(CComVariant(0), &source_group);
      CComPtr<IBarRecordCollection> source_bars;
      source_group->get_BarRecords(&source_bars);

      long nBars;
      source_bars->get_Count(&nBars);
      for (long barIdx = 0; barIdx < nBars; barIdx++)
      {
         CComPtr<IBarRecord> bar;
         source_bars->get_Item(CComVariant(barIdx), &bar);
         targetBars->Add(bar);
      }

      return deResult;
   }

   return CListView::OnDropEx(pDataObject, dropDefault, dropList, point);
}


void CBarlistListView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
   OnUpdateEditCopy(pCmdUI);
}


void CBarlistListView::OnEditCut()
{
   OnEditCopy();

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
      if (n == 0)
      {
         SelectAll();
         n = list.GetSelectedCount();
      }

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
