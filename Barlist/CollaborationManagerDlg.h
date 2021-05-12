///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2021  Washington State Department of Transportation
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

#pragma once

#include "resource.h"
#include <vector>
#include "afxwin.h"


// CCollaborationManagerDlg dialog

class CCollaborationManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CCollaborationManagerDlg)

public:
	CCollaborationManagerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCollaborationManagerDlg();

   std::vector<std::string> m_vFiles;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLABORATION_MANAGER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   afx_msg void OnDropFiles(HDROP dropInfo);

   void MoveFiles(int step);

   void UpdateButtons();

	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();

protected:
   CListBox m_lbFiles;
   int m_cxMin;
   int m_cyMin;


   afx_msg HCURSOR OnQueryDragIcon();
   afx_msg void OnBnClickedRemove();
   afx_msg void OnBnClickedMoveUp();
   afx_msg void OnBnClickedMoveDown();
   afx_msg void OnSelchangeFiles();
   afx_msg void OnBnClickedAdd();
   afx_msg void OnBnClickedChelp();
   afx_msg void OnDestroy();
   afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

   virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;
};
