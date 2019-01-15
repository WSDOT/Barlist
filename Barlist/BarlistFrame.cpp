///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 2009-2019  Washington State Department of Transportation
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
#include "resource.h"

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

   // introduce the views to each oterh
   GetTreeView()->SetListView(GetListView());
   GetListView()->SetTreeView(GetTreeView());

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

   // TODO:  Add your specialized creation code here
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   SetIcon(AfxGetApp()->LoadIcon(IDI_BARLIST),TRUE);

   return 0;
}


void CBarlistFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
   COleDataObject dataObj;
   BOOL bEnable = FALSE;
   if (dataObj.AttachClipboard() && dataObj.IsDataAvailable(CBarlistListView::ms_cFormat))
   {
      bEnable = TRUE; // its our kind of data

      HGLOBAL hGlobal = dataObj.GetGlobalData(CBarlistListView::ms_cFormat);
      CBarlistClipboardData* pCBD = (CBarlistClipboardData*)::GlobalLock(hGlobal);
      if (pCBD->nThreadID != AfxGetThread()->m_nThreadID)
      {
         // can't paste into the source group or
         // the data is coming from a different thread
         bEnable = FALSE;
      }
      ::GlobalUnlock(hGlobal);
   }
   pCmdUI->Enable(bEnable);
}

void CBarlistFrame::OnEditPaste()
{
   COleDataObject dataObj;
   if (dataObj.AttachClipboard() && dataObj.IsDataAvailable(CBarlistListView::ms_cFormat))
   {
      HGLOBAL hGlobal = dataObj.GetGlobalData(CBarlistListView::ms_cFormat);
      CBarlistClipboardData* pCBD = (CBarlistClipboardData*)::GlobalLock(hGlobal);

      CBarlistDoc* pDoc = GetTreeView()->GetDocument();
      CComPtr<IBarlist> barlist;
      pDoc->GetBarlist(&barlist);

      CComPtr<IGroupCollection> groups;
      barlist->get_Groups(&groups);

      long targetGroupIdx = GetTreeView()->GetSelectedGroup();

      CComPtr<IGroup> targetGroup;
      groups->get_Item(CComVariant(targetGroupIdx), &targetGroup);

      CComPtr<IBarRecordCollection> targetBars;
      targetGroup->get_BarRecords(&targetBars);

      CComPtr<IGroup> sourceGroup;
      groups->get_Item(CComVariant(pCBD->sourceGroupIdx), &sourceGroup);
      CComPtr<IBarRecordCollection> sourceBars;
      sourceGroup->get_BarRecords(&sourceBars);

      long nBars;
      pCBD->bars->get_Count(&nBars);
      for (long barIdx = 0; barIdx < nBars; barIdx++)
      {
         CComPtr<IBarRecord> bar;
         pCBD->bars->get_Item(CComVariant(barIdx), &bar);

         // even though we've already made a copy of the bar when the clipboard operation started
         // we have to copy again because of multiple paste operations... each paste must
         // result in a new bar... a bar cannot be added to multiple groups or multiple times in the 
         // same group
         CComPtr<IBarRecord> clone;
         pDoc->CopyBar(bar, &clone);
         targetBars->Add(clone);
      }

      ::GlobalUnlock(hGlobal);
   }
}
