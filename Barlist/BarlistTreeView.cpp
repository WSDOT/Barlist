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

// BarlistView.cpp : implementation of the CBarlistView class
//

#include "stdafx.h"
#include "resource.h"
#include "CollaborationDoc.h"
#include "BarlistTreeView.h"
#include "BarlistListView.h"
#include "BarlistFrame.h"
#include "Events.h"

#include "BarlistPrintDialog.h"

#include "Barlist.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarlistView

IMPLEMENT_DYNCREATE(CBarlistTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CBarlistTreeView, CTreeView)
	//{{AFX_MSG_MAP(CBarlistTreeView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
   ON_WM_CREATE()
   ON_WM_DESTROY()
   ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelChanged)
   ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnGroupRenamed)
   ON_UPDATE_COMMAND_UI(IDC_MOVE_UP, &CBarlistTreeView::OnUpdateMoveUp)
   ON_COMMAND(IDC_MOVE_UP, &CBarlistTreeView::OnMoveUp)
   ON_UPDATE_COMMAND_UI(IDC_MOVE_DOWN, &CBarlistTreeView::OnUpdateMoveDown)
   ON_COMMAND(IDC_MOVE_DOWN, &CBarlistTreeView::OnMoveDown)
   ON_UPDATE_COMMAND_UI(IDC_DELETE, &CBarlistTreeView::OnUpdateDelete)
   ON_COMMAND(IDC_DELETE, &CBarlistTreeView::OnDelete)
   ON_UPDATE_COMMAND_UI(IDC_RENAME, &CBarlistTreeView::OnUpdateRename)
   ON_COMMAND(IDC_RENAME, &CBarlistTreeView::OnRename)
   //}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
   ON_WM_MOUSEMOVE()
   ON_NOTIFY_REFLECT(NM_DBLCLK, &CBarlistTreeView::OnNMDblclk)
   ON_NOTIFY_REFLECT(NM_RCLICK, &CBarlistTreeView::OnNMRClick)
   ON_WM_LBUTTONDOWN()
   ON_WM_RBUTTONDOWN()
   ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CBarlistTreeView::OnUpdateEditCut)
   ON_COMMAND(ID_EDIT_CUT, &CBarlistTreeView::OnEditCut)
   ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CBarlistTreeView::OnUpdateEditCopy)
   ON_COMMAND(ID_EDIT_COPY, &CBarlistTreeView::OnEditCopy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarlistTreeView construction/destruction

CBarlistTreeView::CBarlistTreeView()
{
	// TODO: add construction code here
   m_pListView = nullptr;
   m_pFrame = nullptr;
}

CBarlistTreeView::~CBarlistTreeView()
{
}

void CBarlistTreeView::SetListView(CBarlistListView* pListView)
{
   m_pListView = pListView;
}

void CBarlistTreeView::SetFrame(CBarlistFrame* pFrame)
{
   m_pFrame = pFrame;
}

long CBarlistTreeView::GetSelectedGroup()
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hItem = tree.GetSelectedItem();
   if (hItem)
   {
      return (long)tree.GetItemData(hItem);
   }
   else
   {
      return -1;
   }
}

void CBarlistTreeView::SelectGroup(long groupIdx)
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hProjectItem = tree.GetRootItem();

   if (groupIdx == -1)
   {
      tree.SelectItem(hProjectItem);
   }
   else
   {
      CBarlistDoc* pDoc = GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);

      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);

      long nGroups;
      groups->get_Count(&nGroups);
      HTREEITEM hGroupItem = tree.GetNextItem(hProjectItem, TVGN_CHILD); // gets first child from root... that's the first barlist group
      for (long idx = 0; idx < groupIdx && idx < nGroups; idx++)
      {
         hGroupItem = tree.GetNextItem(hGroupItem, TVGN_NEXT); // gets the next group at the current level
      }
      tree.SelectItem(hGroupItem);
   }

   m_pFrame->UpdateQuantities(groupIdx);
}

void CBarlistTreeView::OnInitialUpdate()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   USES_CONVERSION;

   CTreeView::OnInitialUpdate();

   m_ImageList.Create(12, 12, ILC_COLOR4, 3, 1);
   auto hInstance = AfxGetInstanceHandle();
   int result = m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GREEN)));
   ATLASSERT(result == 0);
   result = m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YELLOW)));
   ATLASSERT(result == 1);
   result = m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RED)));
   ATLASSERT(result == 2);

   CTreeCtrl& tree = GetTreeCtrl();
   tree.SetImageList(&m_ImageList, LVSIL_NORMAL);

   UpdateTree();
}

/////////////////////////////////////////////////////////////////////////////
// CBarlistTreeView drawing

void CBarlistTreeView::OnDraw(CDC* pDC)
{

}

/////////////////////////////////////////////////////////////////////////////
// CBarlistTreeView printing

BOOL CBarlistTreeView::OnPreparePrinting(CPrintInfo* pInfo)
{
   // default preparation

   // Use our own printer dialog
   // Our printer dialog is the default MFC printer dialog
   // however, we force the page setup to be Landscape and Letter size paper
   delete pInfo->m_pPD;
   pInfo->m_pPD = new CBarlistPrintDialog(FALSE);

   CBarlistDoc* pDoc = GetDocument();
   CReport& report = pDoc->GetReport();

   pInfo->m_pPD->m_pd.Flags |= PD_ALLPAGES | PD_NOPAGENUMS | PD_NOSELECTION | PD_NOCURRENTPAGE;
   pInfo->m_pPD->m_pd.nFromPage = 1;
   pInfo->m_pPD->m_pd.nMinPage = 1;
   pInfo->m_pPD->m_pd.nToPage = -1;
   pInfo->m_pPD->m_pd.nMaxPage = -1;

   return DoPreparePrinting(pInfo);
}

void CBarlistTreeView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
   CBarlistDoc* pDoc = GetDocument();
   CReport& report = pDoc->GetReport();
   report.PrepareForPrinting(pDC, pInfo);
   report.Print(pDC, pInfo);
   report.EndPrinting(pDC, pInfo);
}

void CBarlistTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   if (lHint == HINT_PROJECT_CHANGED || lHint == HINT_GROUP_ADDED || lHint == HINT_GROUP_REMOVED || lHint == HINT_GROUP_CHANGED)
   {
      UpdateTree();
   }
   else if (lHint == HINT_GROUP_MOVED)
   {
      CGroupEventHint* pEventHint = (CGroupEventHint*)pHint;
      UpdateTree(pEventHint->toIdx);
   }
   else if (lHint == HINT_BAR_ADDED || lHint == HINT_BAR_CHANGED || lHint == HINT_BAR_REMOVED)
   {
      UpdateStatus();
      m_pFrame->UpdateQuantities(((CGroupEventHint*)pHint)->group);
   }
}

/////////////////////////////////////////////////////////////////////////////
// CBarlistTreeView diagnostics

#ifdef _DEBUG
void CBarlistTreeView::AssertValid() const
{
   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CTreeView::AssertValid();
}

void CBarlistTreeView::Dump(CDumpContext& dc) const
{
   CTreeView::Dump(dc);
}

CBarlistDoc* CBarlistTreeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBarlistDoc)));
	return (CBarlistDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBarlistTreeView message handlers

int CBarlistTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   lpCreateStruct->style |= TVS_SHOWSELALWAYS | TVS_FULLROWSELECT | TVS_HASLINES | /*TVS_LINESATROOT | TVS_HASBUTTONS |*/ TVS_EDITLABELS | TVS_TRACKSELECT;

   if (CTreeView::OnCreate(lpCreateStruct) == -1)
   {
      return -1;
   }

   m_DropTarget.Register(this);


   return 0;
}

void CBarlistTreeView::OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
   NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

   // tell the entry list view to update itself.
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hItem = tree.GetSelectedItem();
   if (hItem)
   {
      DWORD_PTR itemData = tree.GetItemData(hItem);
      m_pListView->OnGroupSelected((long)itemData);
   }

   *pResult = 0;
}

void CBarlistTreeView::OnGroupRenamed(NMHDR* pNMHDR, LRESULT* pResult)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   NMTVDISPINFO* pInfo = (NMTVDISPINFO*)pNMHDR;
   if (pInfo->item.pszText != nullptr)
   {
      CTreeCtrl& tree = GetTreeCtrl();
      long groupIdx = (long)tree.GetItemData(pInfo->item.hItem);

      CBarlistDoc* pDoc = GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);
      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(groupIdx), &group);
      if (FAILED(group->put_Name(CComBSTR(pInfo->item.pszText))))
      {
         AfxMessageBox(_T("That group name already exists."), MB_ICONINFORMATION | MB_OK);
      }
   }
}

void CBarlistTreeView::OnMouseMove(UINT nFlags, CPoint point)
{
   // Tracking quantities
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hItem = tree.HitTest(point);

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   long grpIdx = -1;
   if (hItem == NULL)
   {
      // the mouse isn't over a tree item, so get the current selection
      grpIdx = GetSelectedGroup();
   }
   else
   {
      grpIdx = (long)tree.GetItemData(hItem);
   }

   if (grpIdx == -1 && hItem == NULL)
   {
      m_pFrame->ClearQuantities();
   }
   else
   {
      m_pFrame->UpdateQuantities(grpIdx);
   }

   CTreeView::OnMouseMove(nFlags, point);
}

void CBarlistTreeView::OnUpdateMoveUp(CCmdUI *pCmdUI)
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hSelectedItem = tree.GetSelectedItem();
   BOOL bEnable = (hSelectedItem != tree.GetRootItem() && hSelectedItem != NULL);
   pCmdUI->Enable(bEnable);
}

void CBarlistTreeView::OnMoveUp()
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hItem = tree.GetSelectedItem();
   if (hItem)
   {
      long idx = (long)tree.GetItemData(hItem);
      CBarlistDoc* pDoc = GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);
      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);
      groups->MoveUp(CComVariant(idx));
   }
}

void CBarlistTreeView::OnUpdateMoveDown(CCmdUI *pCmdUI)
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hSelectedItem = tree.GetSelectedItem();
   BOOL bEnable = (hSelectedItem != tree.GetRootItem() && hSelectedItem != NULL);
   pCmdUI->Enable(bEnable);
}

void CBarlistTreeView::OnMoveDown()
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hItem = tree.GetSelectedItem();
   if (hItem)
   {
      long idx = (long)tree.GetItemData(hItem);
      CBarlistDoc* pDoc = GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);
      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);
      groups->MoveDown(CComVariant(idx));
   }
}

void CBarlistTreeView::OnUpdateDelete(CCmdUI *pCmdUI)
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hSelectedItem = tree.GetSelectedItem();
   BOOL bEnable = (hSelectedItem != tree.GetRootItem() && hSelectedItem != NULL);
   pCmdUI->Enable(bEnable);
}

void CBarlistTreeView::OnDelete()
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hItem = tree.GetSelectedItem();
   if (hItem)
   {
      long idx = (long)tree.GetItemData(hItem);
      CBarlistDoc* pDoc = GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);
      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);
      groups->Remove(CComVariant(idx));

      hItem = tree.GetSelectedItem();
      if (hItem)
      {
         idx = (long)tree.GetItemData(hItem);
         m_pListView->OnGroupSelected(idx);
      }
   }
}

void CBarlistTreeView::OnUpdateRename(CCmdUI *pCmdUI)
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hSelectedItem = tree.GetSelectedItem();
   BOOL bEnable = (hSelectedItem != tree.GetRootItem() && hSelectedItem != NULL);
   pCmdUI->Enable(bEnable);
}

void CBarlistTreeView::OnRename()
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hItem = tree.GetSelectedItem();
   if (hItem)
   {
      tree.EditLabel(hItem);
   }
}

void CBarlistTreeView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
   OnUpdateEditCopy(pCmdUI);
}

void CBarlistTreeView::OnEditCut()
{
   OnEditCopy();

   // do the cut
   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);
   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);
   
   long groupIdx = GetSelectedGroup();
   groups->Remove(CComVariant(groupIdx));
}

void CBarlistTreeView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
   CTreeCtrl& tree = GetTreeCtrl();
   HTREEITEM hItem = tree.GetSelectedItem();
   pCmdUI->Enable(hItem != NULL && hItem != tree.GetRootItem());
}

void CBarlistTreeView::OnEditCopy()
{
   COleDataSource* pDataSource = new COleDataSource;
   CacheBarlistClipboardData(*pDataSource);
   pDataSource->SetClipboard();
}

void CBarlistTreeView::UpdateTree(long selectGroupIdx)
{
   CTreeCtrl& tree = GetTreeCtrl();

   // if selectGroupIdx == -1, then keep the current selection
   // otherwise, select the group at selectGroupIdx
   HTREEITEM hSelectItem = 0;
   if (selectGroupIdx == -1)
   {
      hSelectItem = tree.GetSelectedItem();
      if (hSelectItem)
      {
         selectGroupIdx = (long)tree.GetItemData(hSelectItem);
      }
   }

   tree.DeleteAllItems();

   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);

   CComBSTR bstrProject;
   barlist->get_Project(&bstrProject);

   HTREEITEM hProjectItem = tree.InsertItem(OLE2T(bstrProject), 0, 0);
   tree.SetItemData(hProjectItem, -1);

   StatusType status;
   barlist->get_Status(&status);
   tree.SetItemImage(hProjectItem, (int)status, (int)status);

   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);

   long nGroups;
   groups->get_Count(&nGroups);
   for (long idx = 0; idx < nGroups; idx++)
   {
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(idx), &group);

      CComBSTR bstrName;
      group->get_Name(&bstrName);

      HTREEITEM hGroupItem = tree.InsertItem(OLE2T(bstrName),/*nImage,nSelectedImage,*/ hProjectItem, TVI_LAST);
      tree.SetItemData(hGroupItem, idx);

      group->get_Status(&status);

      tree.SetItemImage(hGroupItem, (int)status, (int)status);

      if (idx == selectGroupIdx)
      {
         hSelectItem = hGroupItem;
      }
   }

   tree.Expand(hProjectItem, TVE_EXPAND);

   if (hSelectItem)
   {
      tree.SelectItem(hSelectItem);
   }
}

void CBarlistTreeView::UpdateStatus()
{
   CTreeCtrl& tree = GetTreeCtrl();

   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);

   HTREEITEM hProjectItem = tree.GetRootItem();

   StatusType status;
   barlist->get_Status(&status);
   tree.SetItemImage(hProjectItem, (int)status, (int)status);

   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);

   long nGroups;
   groups->get_Count(&nGroups);
   HTREEITEM hGroupItem = tree.GetNextItem(hProjectItem, TVGN_CHILD); // gets first child from root... that's the first barlist group
   for (long idx = 0; idx < nGroups; idx++)
   {
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(idx), &group);
      group->get_Status(&status);

      tree.SetItemImage(hGroupItem, (int)status, (int)status);

      hGroupItem = tree.GetNextItem(hGroupItem, TVGN_NEXT); // gets the next group at the current level
   }
}

DROPEFFECT CBarlistTreeView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
   m_bRMouse = (dwKeyState == VK_RBUTTON);
   return CTreeView::OnDragEnter(pDataObject, dwKeyState, point);
}

void CBarlistTreeView::OnDragLeave()
{
   m_bRMouse = false;
   CTreeView::OnDragLeave();
}

DROPEFFECT CBarlistTreeView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
   CBarlistDoc* pDoc = GetDocument();
   if (pDoc->IsKindOf(RUNTIME_CLASS(CCollaborationDoc)))
   {
      // can't drop onto a collaboration document
      return DROPEFFECT_NONE;
   }

   if (pDataObject->IsDataAvailable(CBarlistListView::ms_cBarFormat))
   {
      // bars must drop on a tree node, execpt the root node
      CTreeCtrl& tree = GetTreeCtrl();

      UINT flags;
      HTREEITEM hItem = tree.HitTest(point, &flags);
      if (hItem == NULL || hItem == tree.GetRootItem())
      {
         // can't drop on tree root
         return DROPEFFECT_NONE;
      }

      if(hItem != NULL && (TVHT_ONITEM & flags))
      {
         tree.SelectItem(hItem);
         return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
      }
      else
      {
         return DROPEFFECT_NONE;
      }
   }
   else if (pDataObject->IsDataAvailable(CBarlistListView::ms_cGroupFormat))
   {
      // groups must be dropped on the root node or in the whitespace
      CTreeCtrl& tree = GetTreeCtrl();

      UINT flags;
      HTREEITEM hItem = tree.HitTest(point, &flags);

      // can only drop on whitespace or the root node
      if (hItem == NULL || hItem == tree.GetRootItem())
      {
         HGLOBAL hGlobal = pDataObject->GetGlobalData(CBarlistListView::ms_cGroupFormat);
         DWORD* pThreadID = (DWORD*)::GlobalLock(hGlobal);

         // can only drop a group if it comes from a different thread
         if (*pThreadID == AfxGetApp()->m_nThreadID)
         {
            return DROPEFFECT_NONE;
         }
         else
         {
            return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
         }
      }
      else
      {
         return DROPEFFECT_NONE;
      }
   }

   return CTreeView::OnDragOver(pDataObject, dwKeyState, point);
}

DROPEFFECT CBarlistTreeView::OnDropEx(COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point)
{
   if (pDataObject->IsDataAvailable(CBarlistListView::ms_cBarFormat))
   {
      CTreeCtrl& tree = GetTreeCtrl();
      HTREEITEM hTargetItem = tree.HitTest(point);
      if (hTargetItem && hTargetItem != tree.GetRootItem())
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

            pPopup->SetDefaultItem(dropDefault == DROPEFFECT_COPY ? 0 : 1, MF_BYPOSITION);
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

         long targetGroupIdx = (long)tree.GetItemData(hTargetItem);
         CComPtr<IGroup> targetGroup;
         groups->get_Item(CComVariant(targetGroupIdx), &targetGroup);

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

         tree.SelectItem(hTargetItem);

         return deResult;
      }
      return DROPEFFECT_NONE;
   }
   else if (pDataObject->IsDataAvailable(CBarlistListView::ms_cGroupFormat))
   {
      CTreeCtrl& tree = GetTreeCtrl();
      HTREEITEM hTargetItem = tree.HitTest(point);

      // can only drop group data in whitespace or on the root item
      if (hTargetItem == NULL || hTargetItem == tree.GetRootItem())
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
            pPopup->SetDefaultItem(dropDefault == DROPEFFECT_COPY ? 0 : 1, MF_BYPOSITION);
            BOOL nCmd = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
            if (nCmd == 0)
            {
               // user cancelled the menu
               return DROPEFFECT_NONE;
            }

            deResult = (nCmd == ID_EDIT_COPY ? DROPEFFECT_COPY : DROPEFFECT_MOVE);
         }


         // reconstitute the source barlist data from the XML string
         HGLOBAL hGlobal = pDataObject->GetGlobalData(CBarlistListView::ms_cGroupFormat);
         DWORD* pThreadID = (DWORD*)::GlobalLock(hGlobal);
         ATLASSERT(*pThreadID != AfxGetApp()->m_nThreadID); // can't drop a group from the same application

         pThreadID++;

         LPCSTR strXML = LPCSTR(pThreadID);
         CComPtr<IBarlist> source_barlist;
         CBarlistDoc* pDoc = GetDocument();
         pDoc->CreateBarlist(strXML, &source_barlist);
         ::GlobalUnlock(hGlobal);

         CComPtr<IBarlist> target_barlist;
         pDoc->GetBarlist(&target_barlist);

         CComPtr<IGroupCollection> target_groups;
         target_barlist->get_Groups(&target_groups);

         CComPtr<IGroupCollection> source_groups;
         source_barlist->get_Groups(&source_groups);
         long nGroups;
         source_groups->get_Count(&nGroups);
         for(long groupIdx = 0; groupIdx < nGroups; groupIdx++)
         {
            CComPtr<IGroup> source_group;
            source_groups->get_Item(CComVariant(groupIdx), &source_group);

            // create a new group
            CComBSTR bstrGroup;
            source_group->get_Name(&bstrGroup);
            CString strTargetGroupName(bstrGroup);
            int trial = 0;
            while (FAILED(target_groups->Add(CComBSTR(strTargetGroupName))))
            {
               if (trial == 0)
               {
                  strTargetGroupName.Format(_T("%s_Copy"), OLE2T(bstrGroup));
               }
               else
               {
                  strTargetGroupName.Format(_T("%s_Copy(%d)"), OLE2T(bstrGroup), trial);
               }
               trial++;
            }

            CComPtr<IGroup> target_group;
            target_groups->get_Item(CComVariant(CComBSTR(strTargetGroupName)), &target_group);
            CComPtr<IBarRecordCollection> target_bars;
            target_group->get_BarRecords(&target_bars);

            CComPtr<IBarRecordCollection> source_bars;
            source_group->get_BarRecords(&source_bars);

            long nBars;
            source_bars->get_Count(&nBars);
            for (long barIdx = 0; barIdx < nBars; barIdx++)
            {
               CComPtr<IBarRecord> bar;
               source_bars->get_Item(CComVariant(barIdx), &bar);
               target_bars->Add(bar);
            }
         }

         tree.SelectItem(hTargetItem);

         return deResult;
      }
      return DROPEFFECT_NONE;
   }


   return CTreeView::OnDropEx(pDataObject, dropDefault, dropList, point);
}

void CBarlistTreeView::CacheBarlistClipboardData(COleDataSource& dataSource)
{
   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);
   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);
   long nGroups;
   groups->get_Count(&nGroups);

   CComPtr<IBarlist> source_barlist;
   source_barlist.CoCreateInstance(CLSID_Barlist);
   CComPtr<IGroupCollection> source_groups;
   source_barlist->get_Groups(&source_groups);

   long selectedGroupIdx = GetSelectedGroup();
   long firstGroupIdx = (selectedGroupIdx < 0 ? 0 : selectedGroupIdx);
   long lastGroupIdx = (selectedGroupIdx < 0 ? nGroups - 1 : selectedGroupIdx);
   for (long groupIdx = firstGroupIdx; groupIdx <= lastGroupIdx; groupIdx++)
   {
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(groupIdx), &group);
      CComPtr<IBarRecordCollection> bars;
      group->get_BarRecords(&bars);

      CComBSTR bstrName;
      group->get_Name(&bstrName);

      source_groups->Add(bstrName);
      CComPtr<IGroup> source_group;
      source_groups->get_Item(CComVariant(0), &source_group);
      CComPtr<IBarRecordCollection> source_bars;
      source_group->get_BarRecords(&source_bars);

      long nBars;
      bars->get_Count(&nBars);
      for (long barIdx = 0; barIdx < nBars; barIdx++)
      {
         CComPtr<IBarRecord> bar;
         bars->get_Item(CComVariant(barIdx), &bar);

         CComPtr<IBarRecord> clone;
         pDoc->CopyBar(bar, &clone);

         source_bars->Add(clone);
      }
   }

   Barlist barlist_xml = pDoc->CreateXML(source_barlist);
   std::ostringstream os;
   Barlist_(os, barlist_xml, xml_schema::namespace_infomap(), _T("UTF-8"));

   size_t size = os.str().size();
   size++; // add one more for null terminaator

   // add enough space to hold the thread id as well
   DWORD threadID = AfxGetApp()->m_nThreadID;
   size += sizeof(DWORD);

   // allocate global memory space
   HGLOBAL hGlobal = ::GlobalAlloc(GHND, size);

   // save the thread id
   DWORD* pThreadID = (DWORD*)::GlobalLock(hGlobal);
   *pThreadID = threadID;

   // advance the pointer to where we want to start saving the string
   pThreadID++;

   // cast the pointer and copy the string
   char* pString = (char*)pThreadID;
   ::strcpy_s(pString, os.str().size()+1, os.str().c_str());
   pString[os.str().size()] = 0; // null terminate the string

   // done with the global memory so unlock it
   ::GlobalUnlock(hGlobal);

   // cache the memory handle in the data source for later retreival
   dataSource.CacheGlobalData(CBarlistListView::ms_cGroupFormat, hGlobal);
}

BOOL CBarlistTreeView::MouseButtonDrag(UINT nFlags, CPoint point)
{
   CBarlistDoc* pDoc = GetDocument();
   if (pDoc->IsKindOf(RUNTIME_CLASS(CCollaborationDoc)))
   {
      return FALSE;
   }
   else
   {
      // Drag and drop will only occur if the mouse is on a tree node
      CTreeCtrl& tree = GetTreeCtrl();
      HTREEITEM hTreeItem = tree.HitTest(point);
      if (hTreeItem != NULL)
      {
         CRect rect;
         tree.GetItemRect(hTreeItem, &rect, FALSE);
         if (rect.PtInRect(point))
         {
            tree.SelectItem(hTreeItem);

            COleDataSource dataSource;
            CacheBarlistClipboardData(dataSource);
            if (dataSource.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE) == DROPEFFECT_MOVE)
            {
               // the group was moved, so remove it from the barlist
               LPDATAOBJECT lpDataObject = (LPDATAOBJECT)dataSource.GetInterface(&IID_IDataObject);
               COleDataObject dataObj;
               dataObj.Attach(lpDataObject, FALSE);
               HGLOBAL hGlobal = dataObj.GetGlobalData(CBarlistListView::ms_cGroupFormat);

               // reconstitute the source barlist data from the XML string
               DWORD* pThreadID = (DWORD*)::GlobalLock(hGlobal);
               ATLASSERT(*pThreadID == AfxGetApp()->m_nThreadID);
               pThreadID++;

               LPCSTR strXML = LPCSTR(pThreadID);
               CComPtr<IBarlist> source_barlist;
               pDoc->CreateBarlist(strXML, &source_barlist);
               ::GlobalUnlock(hGlobal);

               // get the collection of bar records to paste
               CComPtr<IGroupCollection> source_groups;
               source_barlist->get_Groups(&source_groups);
               long nGroups;
               source_groups->get_Count(&nGroups);
               for (long groupIdx = 0; groupIdx < nGroups; groupIdx++)
               {
                  CComPtr<IGroup> source_group;
                  source_groups->get_Item(CComVariant(groupIdx), &source_group);

                  CComBSTR bstrName;
                  source_group->get_Name(&bstrName);

                  CComPtr<IBarlist> barlist;
                  pDoc->GetBarlist(&barlist);
                  CComPtr<IGroupCollection> groups;
                  barlist->get_Groups(&groups);
                  groups->Remove(CComVariant(bstrName));
               }
            }
         }
      }
      return TRUE;
   }
}

void CBarlistTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
   if ( !MouseButtonDrag(nFlags, point) )
      CTreeView::OnRButtonDown(nFlags, point);
}

void CBarlistTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
   if (!MouseButtonDrag(nFlags, point) )
      CTreeView::OnLButtonDown(nFlags, point);
}

void CBarlistTreeView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
   CBarlistDoc* pDoc = GetDocument();
   long groupIdx = GetSelectedGroup();
   long barIdx = m_pListView->GetSelectedBar();
   pDoc->EditBar(groupIdx, barIdx);

   *pResult = 0;
}


void CBarlistTreeView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
   CBarlistDoc* pDoc = GetDocument();
   if (pDoc->IsKindOf(RUNTIME_CLASS(CCollaborationDoc)))
   {
      // no context editing for collaboration projects
      return;
   }

   CTreeCtrl& tree = GetTreeCtrl();
   CPoint point;
   GetCursorPos(&point);
   ScreenToClient(&point);
   HTREEITEM hItem = tree.HitTest(point);
   ClientToScreen(&point);
   if (hItem)
   {
      tree.SelectItem(hItem);
   }

   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CMenu menu;
   menu.LoadMenu(IDR_TREE_CONTEXT);
   menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, EAFGetMainFrame());

   *pResult = 0;
}

