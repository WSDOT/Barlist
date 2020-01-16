///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2019  Washington State Department of Transportation
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
#include <Bars\Barlst.h>

// CQuantitiesDlg dialog

class CQuantitiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CQuantitiesDlg)

public:
	CQuantitiesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQuantitiesDlg();

// Dialog Data
	enum { IDD = IDD_QUANTITIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   bool m_bClosedByParent{ false }; // if true, the parent window is closing the dialog

   CListCtrl m_QuantitiesList;

   int m_cxMin;
   int m_cyMin;

	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();

   void SetGroup(BSTR bstrGroup);
   void SetQuantities(MaterialType material, Float64 sub, Float64 subEpoxy, Float64 super, Float64 superEpoxy);

   void Clear();
   void SaveWindowPosition();
   void RestoreWindowPosition();
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
   afx_msg void OnDestroy();
   virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;
   afx_msg void OnSize(UINT nType, int cx, int cy);
};
