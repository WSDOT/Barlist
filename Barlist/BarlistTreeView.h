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

// BarlistView.h : interface of the CBarlistView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BarlistView_H__536DF63B_D04E_4BD4_A347_53ADDDDA2863__INCLUDED_)
#define AFX_BarlistView_H__536DF63B_D04E_4BD4_A347_53ADDDDA2863__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BarlistDoc.h"
#include "QuantitiesDlg.h"

class CBarlistListView;

class CBarlistTreeView : public CTreeView
{
protected: // create from serialization only
   CBarlistTreeView();
	DECLARE_DYNCREATE(CBarlistTreeView)

// Attributes
public:
	CBarlistDoc* GetDocument();

   void SetListView(CBarlistListView* pListView);

   long GetSelectedGroup();
   void SelectGroup(long groupIdx);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarlistView)
	public:
	virtual void OnDraw(CDC* pDC) override;  // overridden to draw this view
   virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
   virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
   //}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBarlistTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const override;
	virtual void Dump(CDumpContext& dc) const override;
#endif

protected:
   friend CBarlistListView;
   CBarlistListView* m_pListView;

   CImageList m_ImageList;

   COleDropTarget m_DropTarget;

   void UpdateTree(long selectGroupIdx=-1);
   void UpdateStatus();

   std::unique_ptr<CQuantitiesDlg> m_pQuantitiesDlg;
   void UpdateQuantities(long grpIdx);
   void UpdateQuantities(IGroup* pGroup);

// Generated message map functions
protected:
	//{{AFX_MSG(CBarlistTreeView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnGroupRenamed(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnUpdateMoveUp(CCmdUI *pCmdUI);
   afx_msg void OnMoveUp();
   afx_msg void OnUpdateMoveDown(CCmdUI *pCmdUI);
   afx_msg void OnMoveDown();
   afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
   afx_msg void OnDelete();
   afx_msg void OnUpdateRename(CCmdUI *pCmdUI);
   afx_msg void OnRename();
   //}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
   afx_msg void OnQuantnties();
   afx_msg void OnNcDestroy();
   afx_msg void OnUpdateQuantities(CCmdUI *pCmdUI);
   virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
   virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
   afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
   virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
};

#ifndef _DEBUG  // debug version in BarlistTreeView.cpp
inline CBarlistDoc* CBarlistTreeView::GetDocument()
   { return (CBarlistDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BarlistView_H__536DF63B_D04E_4BD4_A347_53ADDDDA2863__INCLUDED_)
