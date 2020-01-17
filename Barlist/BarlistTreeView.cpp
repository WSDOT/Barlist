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
#include "Events.h"

#include "BarlistPrintDialog.h"

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
   ON_COMMAND(IDC_QUANTITIES, &CBarlistTreeView::OnQuantnties)
   ON_WM_NCDESTROY()
   ON_UPDATE_COMMAND_UI(IDC_QUANTITIES, &CBarlistTreeView::OnUpdateQuantities)
//   ON_WM_CLOSE()
ON_NOTIFY_REFLECT(NM_DBLCLK, &CBarlistTreeView::OnNMDblclk)
ON_NOTIFY_REFLECT(NM_RCLICK, &CBarlistTreeView::OnNMRClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarlistTreeView construction/destruction

CBarlistTreeView::CBarlistTreeView()
{
	// TODO: add construction code here
   m_pListView = nullptr;

}

CBarlistTreeView::~CBarlistTreeView()
{
}

void CBarlistTreeView::SetListView(CBarlistListView* pListView)
{
   m_pListView = pListView;
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
   UpdateQuantities(groupIdx);
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
      UpdateQuantities(((CGroupEventHint*)pHint)->group);
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

   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      m_pQuantitiesDlg = std::make_unique<CQuantitiesDlg>(this);
      m_pQuantitiesDlg->Create(CQuantitiesDlg::IDD);
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
   DWORD_PTR itemData = tree.GetItemData(hItem);

   m_pListView->OnGroupSelected((long)itemData);

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

   if (m_pQuantitiesDlg)
   {
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
         m_pQuantitiesDlg->Clear();
      }
      else
      {
         UpdateQuantities(grpIdx);
      }
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
   BOOL bEnable = (hSelectedItem != NULL);
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


void CBarlistTreeView::OnQuantnties()
{
   if (m_pQuantitiesDlg)
   {
      m_pQuantitiesDlg->ShowWindow(m_pQuantitiesDlg->IsWindowVisible() ? SW_HIDE : SW_SHOW);
   }
}

void CBarlistTreeView::OnUpdateQuantities(CCmdUI *pCmdUI)
{
   // TODO: Add your command update UI handler code here
   if (m_pQuantitiesDlg)
   {
      CString str;
      str.Format(_T("%s Quantities"), m_pQuantitiesDlg->IsWindowVisible() ? _T("Hide") : _T("Show"));
      pCmdUI->SetText(str);
      pCmdUI->SetRadio(m_pQuantitiesDlg->IsWindowVisible());
   }
}

void CBarlistTreeView::OnNcDestroy()
{
   if (m_pQuantitiesDlg)
   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      m_pQuantitiesDlg->DestroyWindow();
      m_pQuantitiesDlg = nullptr;
   }
   CTreeView::OnNcDestroy();
}

void CBarlistTreeView::UpdateQuantities(long grpIdx)
{
   CBarlistDoc* pDoc = GetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);

   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);

   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(grpIdx), &group);

   UpdateQuantities(group);
}

void CBarlistTreeView::UpdateQuantities(IGroup* pGroup)
{
   if (m_pQuantitiesDlg)
   {
      CComBSTR bstrName;
      Float64 sub, subEpoxy, super, superEpoxy;
      if (pGroup == nullptr)
      {
         CBarlistDoc* pDoc = GetDocument();
         CComPtr<IBarlist> barlist;
         pDoc->GetBarlist(&barlist);

         barlist->get_Project(&bstrName);

         barlist->get_SubstructureMass(&sub);
         barlist->get_SubstructureMassEpoxy(&subEpoxy);
         barlist->get_SuperstructureMass(&super);
         barlist->get_SuperstructureMassEpoxy(&superEpoxy);
      }
      else
      {
         pGroup->get_Name(&bstrName);

         pGroup->get_SubstructureMass(&sub);
         pGroup->get_SubstructureMassEpoxy(&subEpoxy);
         pGroup->get_SuperstructureMass(&super);
         pGroup->get_SuperstructureMassEpoxy(&superEpoxy);
      }
      m_pQuantitiesDlg->SetQuantities(bstrName, sub, subEpoxy, super, superEpoxy);
   }
}

DROPEFFECT CBarlistTreeView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
   if (pDataObject->IsDataAvailable(CBarlistListView::ms_cFormat))
   {
      CTreeCtrl& tree = GetTreeCtrl();

      UINT flags;
      HTREEITEM hItem = tree.HitTest(point, &flags);
      if (hItem == NULL || hItem == tree.GetRootItem())
      {
         // can't drop on tree root
         return DROPEFFECT_NONE;
      }

      HGLOBAL hGlobal = pDataObject->GetGlobalData(CBarlistListView::ms_cFormat);
      CBarlistClipboardData* pCBD = (CBarlistClipboardData*)::GlobalLock(hGlobal);
      ::GlobalUnlock(hGlobal);

      if (pCBD->nThreadID != AfxGetThread()->m_nThreadID)
      {
         // can't copy data from another thread
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

   return CTreeView::OnDragOver(pDataObject, dwKeyState, point);
}

BOOL CBarlistTreeView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
   if (pDataObject->IsDataAvailable(CBarlistListView::ms_cFormat))
   {
      CTreeCtrl& tree = GetTreeCtrl();
      HTREEITEM hTargetItem = tree.HitTest(point);
      if (hTargetItem && hTargetItem != tree.GetRootItem())
      {
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

         HGLOBAL hGlobal = pDataObject->GetGlobalData(CBarlistListView::ms_cFormat);
         CBarlistClipboardData* pCBD = (CBarlistClipboardData*)::GlobalLock(hGlobal);
         long nBars;
         pCBD->bars->get_Count(&nBars);
         for (long barIdx = 0; barIdx < nBars; barIdx++)
         {
            CComPtr<IBarRecord> bar;
            pCBD->bars->get_Item(CComVariant(0), &bar);
            pCBD->bars->Remove(CComVariant(0));
            targetBars->Add(bar);
         }

         ::GlobalUnlock(hGlobal);

         tree.SelectItem(hTargetItem);

         return TRUE;
      }
      return FALSE;
   }

   return CTreeView::OnDrop(pDataObject, dropEffect, point);
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
   if (hItem)
   {
      tree.SelectItem(hItem);
      ClientToScreen(&point);

      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      CMenu menu;
      menu.LoadMenu(IDR_TREE_CONTEXT);
      menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, EAFGetMainFrame());
   }

   *pResult = 0;
}

