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

// QNIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QNIDlg.h"
#include "afxdialogex.h"
#include "CollaborationDoc.h"

#include <MfcTools\CustomDDX.h>
#include <EAF\EAFApp.h>

// CQNIDlg dialog

IMPLEMENT_DYNAMIC(CQNIDlg, CDialog)

CQNIDlg::CQNIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_QNI, pParent)
{

}

CQNIDlg::~CQNIDlg()
{
}

void CQNIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const auto* pDisplayUnits = pApp->GetDisplayUnits();

   DDX_UnitValueAndTag(pDX, IDC_TRAFFIC, IDC_TRAFFIC_UNIT, m_Traffic, pDisplayUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX, IDC_BRIDGE, IDC_BRIDGE_UNIT, m_Bridge, pDisplayUnits->GeneralForce);
   DDX_UnitValueAndTag(pDX, IDC_WALL, IDC_WALL_UNIT, m_Wall, pDisplayUnits->GeneralForce);
}


BEGIN_MESSAGE_MAP(CQNIDlg, CDialog)
END_MESSAGE_MAP()


// CQNIDlg message handlers


BOOL CQNIDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   // TODO:  Add extra initialization here
   CEAFDocument* pDoc = EAFGetDocument();
   if (pDoc->IsKindOf(RUNTIME_CLASS(CCollaborationDoc)))
   {
      GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
      GetDlgItem(IDCANCEL)->SetWindowText(_T("Close"));
      SetDefID(IDCANCEL);
   }

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
