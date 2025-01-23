///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2025  Washington State Department of Transportation
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

// ChildFrm.cpp : implementation of the CBarlistFrame class
//

#include "stdafx.h"
#include "BarlistFrame.h"
#include "Helpers.h"
#include "resource.h"

#include "Barlist.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarlistFrame

IMPLEMENT_DYNCREATE(CBarlistFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CBarlistFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CBarlistFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
   ON_COMMAND(IDC_SELECT_ALL, &CBarlistFrame::OnSelectAll)
   ON_WM_CREATE()
   ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CBarlistFrame::OnUpdateEditPaste)
   ON_COMMAND(ID_EDIT_PASTE, &CBarlistFrame::OnEditPaste)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
   ON_COMMAND(IDC_QUANTITIES, &CBarlistFrame::OnQuantnties)
   ON_UPDATE_COMMAND_UI(IDC_QUANTITIES, &CBarlistFrame::OnUpdateQuantities)
   ON_WM_NCDESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarlistFrame construction/destruction

CBarlistFrame::CBarlistFrame()
{
	// TODO: add member initialization code here
	
}

CBarlistFrame::~CBarlistFrame()
{
}

BOOL CBarlistFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // force this window to be maximized (not sure why WS_VISIBLE is required)
   cs.style |= WS_MAXIMIZE | WS_VISIBLE;
   cs.style &= ~FWS_ADDTOTITLE;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


BOOL CBarlistFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
   // TODO: Add your specialized code here and/or call the base class

   // Create a splitter window with 1 rows and 2 column
   if (!m_SplitterWnd.CreateStatic(this, 1, 2))
   {
      TRACE0("Failed to create static splitter");
      return FALSE;
   }

   // Add the first pane
   if (!m_SplitterWnd.CreateView(0, 0, pContext->m_pNewViewClass, CSize(lpcs->cx / 3, lpcs->cy), pContext))
   {
      TRACE0("Failed to create first pane");
      return FALSE;
   }

   // Add the second pane
   if (!m_SplitterWnd.CreateView(0, 1, RUNTIME_CLASS(CBarlistListView), CSize(0, 0), pContext))
   {
      TRACE0("Failed to create second pane");
      return FALSE;
   }


   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      m_pQuantitiesDlg = std::make_unique<CQuantitiesDlg>(this);
      if (!m_pQuantitiesDlg->Create(CQuantitiesDlg::IDD))
      {
         m_pQuantitiesDlg = nullptr;
      }
   }

   // introduce the views to each oterh
   GetTreeView()->SetListView(GetListView());
   GetTreeView()->SetFrame(this);
   GetListView()->SetTreeView(GetTreeView());
   GetListView()->SetFrame(this);

   // Activate the tree view
   SetActiveView(GetTreeView());

   // I don't know why you don't call the parent but it makes the difference between
   // the splitter working and not.  See the ViewEx example. They don't call the
   // parent method either.
   //return CMDIChildWnd::OnCreateClient(lpcs, pContext);
   return TRUE;
}

CBarlistTreeView* CBarlistFrame::GetTreeView()
{
   return (CBarlistTreeView*)m_SplitterWnd.GetPane(0, 0);
}

CBarlistListView* CBarlistFrame::GetListView()
{
   return (CBarlistListView*)m_SplitterWnd.GetPane(0, 1);
}

/////////////////////////////////////////////////////////////////////////////
// CBarlistFrame diagnostics

#ifdef _DEBUG
void CBarlistFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CBarlistFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBarlistFrame message handlers


void CBarlistFrame::OnSelectAll()
{
   CBarlistListView* pList = (CBarlistListView*)m_SplitterWnd.GetPane(0, 1);
   pList->SelectAll();
}


int CBarlistFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
      return -1;

   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      SetIcon(AfxGetApp()->LoadIcon(IDR_BARLIST), TRUE);
   }

   return 0;
}


void CBarlistFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
   COleDataObject dataObj;
   BOOL bEnable = FALSE;
   if (dataObj.AttachClipboard())
   {
      if (dataObj.IsDataAvailable(CBarlistListView::ms_cBarFormat))
      {
         bEnable = TRUE; // can always paste bars into a group
      }
      else if(dataObj.IsDataAvailable(CBarlistListView::ms_cGroupFormat))
      {
         HGLOBAL hGlobal = dataObj.GetGlobalData(CBarlistListView::ms_cGroupFormat);
         DWORD* pThreadID = (DWORD*)::GlobalLock(hGlobal);
         bEnable = (*pThreadID == AfxGetApp()->m_nThreadID) ? FALSE : TRUE; // can only paste groups from other threads
         ::GlobalUnlock(hGlobal);
      }
   }
   pCmdUI->Enable(bEnable);
}

void CBarlistFrame::OnEditPaste()
{
   COleDataObject dataObj;
   if (dataObj.AttachClipboard())
   {
      if (dataObj.IsDataAvailable(CBarlistListView::ms_cBarFormat))
      {
         CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();

         // reconstitute the source barlist data from the XML string
         HGLOBAL hGlobal = dataObj.GetGlobalData(CBarlistListView::ms_cBarFormat);
         LPCSTR strXML = (LPCSTR)::GlobalLock(hGlobal);
         CComPtr<IBarlist> source_barlist;
         pDoc->CreateBarlist(strXML, &source_barlist);
         ::GlobalUnlock(hGlobal);

         // get the index of the currently selected group because this
         // is where we are going to paste the bar records into
         long targetGroupIdx = GetTreeView()->GetSelectedGroup();

         // get the target group and its bar record collection
         CComPtr<IBarlist> barlist;
         pDoc->GetBarlist(&barlist);

         CComPtr<IGroupCollection> groups;
         barlist->get_Groups(&groups);

         CComPtr<IGroup> targetGroup;
         groups->get_Item(CComVariant(targetGroupIdx), &targetGroup);

         CComPtr<IBarRecordCollection> targetBars;
         targetGroup->get_BarRecords(&targetBars);

         // get the current selection information so we can paste at the selection point
         CListCtrl& list = GetListView()->GetListCtrl();
         POSITION pos = list.GetFirstSelectedItemPosition();
         std::vector<int> vSelectedItems;
         while (pos)
         {
            vSelectedItems.push_back(list.GetNextSelectedItem(pos));
         }
         int insertIdx = (vSelectedItems.size() == 0 ? -1 : vSelectedItems.back() + 1);


         // get the collection of bar records to paste
         CComPtr<IGroupCollection> source_groups;
         source_barlist->get_Groups(&source_groups);
         CComPtr<IGroup> source_group;
         source_groups->get_Item(CComVariant(0), &source_group);
         CComPtr<IBarRecordCollection> source_bars;
         source_group->get_BarRecords(&source_bars);

         // paste the bar records
         long nBars;
         source_bars->get_Count(&nBars);
         for (long barIdx = 0; barIdx < nBars; barIdx++)
         {
            CComPtr<IBarRecord> bar;
            source_bars->get_Item(CComVariant(barIdx), &bar);
            targetBars->Insert(CComVariant(insertIdx + (insertIdx < 0 ? 0 : barIdx)), bar);
         }

         // inserting bars changes the selection state
         // re-instate the list control selection
         for (auto idx : vSelectedItems)
         {
            list.SetItemState(idx, LVIS_SELECTED, LVIS_SELECTED);
         }
      }
      else if (dataObj.IsDataAvailable(CBarlistListView::ms_cGroupFormat))
      {
         HGLOBAL hGlobal = dataObj.GetGlobalData(CBarlistListView::ms_cGroupFormat);
         DWORD* pThreadID = (DWORD*)::GlobalLock(hGlobal);
         ATLASSERT(*pThreadID != AfxGetApp()->m_nThreadID);
         pThreadID++; // advance the pointer to the start of the xml string
         LPCSTR strXML = LPCSTR(pThreadID); // cast the pointer
         CComPtr<IBarlist> source_barlist;
         CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();
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
         for (long groupIdx = 0; groupIdx < nGroups; groupIdx++)
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
      }
   }
}

void CBarlistFrame::OnQuantnties()
{
   if (m_pQuantitiesDlg)
   {
      m_pQuantitiesDlg->ShowWindow(m_pQuantitiesDlg->IsWindowVisible() ? SW_HIDE : SW_SHOW);
   }
}

void CBarlistFrame::OnUpdateQuantities(CCmdUI *pCmdUI)
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

void CBarlistFrame::OnNcDestroy()
{
   if (m_pQuantitiesDlg)
   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      m_pQuantitiesDlg->DestroyWindow();
      m_pQuantitiesDlg = nullptr;
   }
   __super::OnNcDestroy();
}

void CBarlistFrame::ClearQuantities()
{
   if (m_pQuantitiesDlg)
   {
      m_pQuantitiesDlg->Clear();
   }
}

void CBarlistFrame::UpdateQuantities(long grpIdx)
{
   CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);

   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);

   CComPtr<IGroup> group;
   groups->get_Item(CComVariant(grpIdx), &group);

   UpdateQuantities(group);
}

void CBarlistFrame::UpdateQuantities(IGroup* pGroup)
{
   if (m_pQuantitiesDlg)
   {
      CComBSTR bstrName;
      Float64 sub, subEpoxy, super, superEpoxy;
      if (pGroup == nullptr)
      {
         CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();
         CComPtr<IBarlist> barlist;
         pDoc->GetBarlist(&barlist);

         barlist->get_Project(&bstrName);

         m_pQuantitiesDlg->SetGroup(bstrName);
         for (int i = 0; i < MATERIAL_COUNT; i++)
         {
            MaterialType material = (MaterialType)(i);
            barlist->get_Quantity(material, VARIANT_TRUE/*epoxy*/, VARIANT_TRUE/*substructure*/, &subEpoxy);
            barlist->get_Quantity(material, VARIANT_FALSE/*epoxy*/, VARIANT_TRUE/*substructure*/, &sub);
            barlist->get_Quantity(material, VARIANT_TRUE/*epoxy*/, VARIANT_FALSE/*substructure*/, &superEpoxy);
            barlist->get_Quantity(material, VARIANT_FALSE/*epoxy*/, VARIANT_FALSE/*substructure*/, &super);

            m_pQuantitiesDlg->SetQuantities(material, sub, subEpoxy, super, superEpoxy);
         }
      }
      else
      {
         pGroup->get_Name(&bstrName);

         m_pQuantitiesDlg->SetGroup(bstrName);
         for (int i = 0; i < MATERIAL_COUNT; i++)
         {
            MaterialType material = (MaterialType)(i);
            pGroup->get_Quantity(material, VARIANT_TRUE/*epoxy*/, VARIANT_TRUE/*substructure*/, &subEpoxy);
            pGroup->get_Quantity(material, VARIANT_FALSE/*epoxy*/, VARIANT_TRUE/*substructure*/, &sub);
            pGroup->get_Quantity(material, VARIANT_TRUE/*epoxy*/, VARIANT_FALSE/*substructure*/, &superEpoxy);
            pGroup->get_Quantity(material, VARIANT_FALSE/*epoxy*/, VARIANT_FALSE/*substructure*/, &super);

            m_pQuantitiesDlg->SetQuantities(material, sub, subEpoxy, super, superEpoxy);
         }
      }
   }
}

void CBarlistFrame::OnHelpFinder()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   EAFHelp(AfxGetAppName(), IDH_BARLIST);
}