///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2023  Washington State Department of Transportation
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

// BarlistFrame.h : interface of the CBarlistFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__D71038F7_24C7_41A9_87E6_A9539977EABE__INCLUDED_)
#define AFX_CHILDFRM_H__D71038F7_24C7_41A9_87E6_A9539977EABE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BarlistTreeView.h"
#include "BarlistListView.h"

class CBarlistFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CBarlistFrame)
public:
	CBarlistFrame();

// Attributes
public:

// Operations
public:
   CBarlistTreeView* GetTreeView();
   CBarlistListView* GetListView();

   void UpdateQuantities(long grpIdx);
   void UpdateQuantities(IGroup* pGroup);
   void ClearQuantities();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarlistFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBarlistFrame();
#ifdef _DEBUG
	virtual void AssertValid() const override;
	virtual void Dump(CDumpContext& dc) const override;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CBarlistFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
   virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


   CSplitterWnd m_SplitterWnd;


   std::unique_ptr<CQuantitiesDlg> m_pQuantitiesDlg;

public:
   afx_msg void OnSelectAll();
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
   afx_msg void OnEditPaste();
   afx_msg void OnHelpFinder();
   afx_msg void OnQuantnties();
   afx_msg void OnUpdateQuantities(CCmdUI *pCmdUI);
   afx_msg void OnNcDestroy();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__D71038F7_24C7_41A9_87E6_A9539977EABE__INCLUDED_)
