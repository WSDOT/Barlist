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

// PropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PropertiesDlg.h"
#include "afxdialogex.h"

#include "BarDlg.h" // for DDX_Text - bstring version


// CPropertiesDlg dialog

IMPLEMENT_DYNAMIC(CPropertiesDlg, CDialog)

CPropertiesDlg::CPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPERTIES, pParent)
{

}

CPropertiesDlg::~CPropertiesDlg()
{
}

void CPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

   DDX_Text(pDX, IDC_PROJECT_NAME, m_Project.m_str);
   if (pDX->m_bSaveAndValidate)
   {
      if (m_Project.Length() == 0 || m_Project == CComBSTR(_T("")))
      {
         pDX->PrepareEditCtrl(IDC_PROJECT_NAME);
         AfxMessageBox(_T("Project Name is required"), MB_ICONEXCLAMATION | MB_OK);
         pDX->Fail();
      }
   }

   DDX_Text(pDX, IDC_JOB_NUMBER, m_JobNumber.m_str);
   DDX_Text(pDX, IDC_ENGINEER, m_Engineer.m_str);
   DDX_Text(pDX, IDC_COMPANY, m_Company.m_str);
   DDX_Text(pDX, IDC_COMMENTS, m_Comments.m_str);
}


BEGIN_MESSAGE_MAP(CPropertiesDlg, CDialog)
END_MESSAGE_MAP()


// CPropertiesDlg message handlers
