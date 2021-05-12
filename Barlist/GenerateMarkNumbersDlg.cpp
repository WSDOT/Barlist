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

// GenerateMarkNumbersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GenerateMarkNumbersDlg.h"
#include "afxdialogex.h"


// CGenerateMarkNumbersDlg dialog

IMPLEMENT_DYNAMIC(CGenerateMarkNumbersDlg, CDialog)

CGenerateMarkNumbersDlg::CGenerateMarkNumbersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_RENUMBER, pParent)
   , m_First(1)
   , m_Increment(1)
{

}

CGenerateMarkNumbersDlg::~CGenerateMarkNumbersDlg()
{
}

void CGenerateMarkNumbersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

   DDX_Text(pDX, IDC_FIRST, m_First);
   DDX_Text(pDX, IDC_INCREMENT, m_Increment);
}


BEGIN_MESSAGE_MAP(CGenerateMarkNumbersDlg, CDialog)
END_MESSAGE_MAP()


// CGenerateMarkNumbersDlg message handlers
