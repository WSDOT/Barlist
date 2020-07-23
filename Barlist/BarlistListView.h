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
#if !defined(AFX_BarlistListView_H__536620BD_A422_4A74_880F_39C6CAEE3834__INCLUDED_)
#define AFX_BarlistListView_H__536620BD_A422_4A74_880F_39C6CAEE3834__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarlistListView.h : header file
//

class CBarlistDoc;
class CBarlistTreeView;
class CBarlistFrame;
interface IBarRecordCollection;
interface IBarRecord;

/////////////////////////////////////////////////////////////////////////////
// CBarlistListView view

class CBarlistListView : public CListView
{
protected:
	CBarlistListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBarlistListView)

// Attributes
public:
   static CLIPFORMAT ms_cBarFormat;
   static CLIPFORMAT ms_cGroupFormat;

// Operations
public:
   CBarlistDoc* GetDocument();
   void SetTreeView(CBarlistTreeView* pTreeView);
   void SetFrame(CBarlistFrame* pFrame);

   void OnGroupSelected(long groupIdx);
   void SelectAll();
   long GetSelectedBar();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarlistListView)
	protected:
	virtual void OnDraw(CDC* pDC) override;      // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void OnInitialUpdate();
   virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
   //}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBarlistListView();
#ifdef _DEBUG
	virtual void AssertValid() const override;
	virtual void Dump(CDumpContext& dc) const override;
#endif

	// Generated message map functions
protected:
   virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
   virtual BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
   virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
   virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;
   
   //{{AFX_MSG(CBarlistListView)
		// NOTE - the ClassWizard will add and remove member functions here.
   afx_msg void OnBarRenamed(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnColumnClicked(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnUpdateMoveUp(CCmdUI *pCmdUI);
   afx_msg void OnMoveUp();
   afx_msg void OnUpdateMoveDown(CCmdUI *pCmdUI);
   afx_msg void OnMoveDown();
   afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
   afx_msg void OnDelete();
   afx_msg void OnUpdateRename(CCmdUI *pCmdUI);
   afx_msg void OnRename();	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   long m_GroupIdx;

   CBarlistTreeView* m_pTreeView;
   CBarlistFrame* m_pFrame;

   CImageList m_ImageList;

   COleDropTarget m_DropTarget;
   BOOL m_bRMouse;

   void CacheBarlistClipboardData(COleDataSource& dataSource);
   void SetBarRecord(int row, IBarRecord* pBarRecord);
   BOOL MouseButtonDrag(UINT nFlags, CPoint point);

public:
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
   virtual void OnDragLeave();
   virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
   virtual DROPEFFECT OnDropEx(COleDataObject* pDataObject, DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);
   afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
   afx_msg void OnEditCut();
   afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
   afx_msg void OnEditCopy();
   afx_msg void OnGenerateMarkNumbers();
   afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BarlistListView_H__536620BD_A422_4A74_880F_39C6CAEE3834__INCLUDED_)
