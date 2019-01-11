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

// ReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReportDlg.h"
#include "afxdialogex.h"
#include <EAF\EAFApp.h>
#include "BarlistDoc.h"
#include "Report.h"


// CReportDlg dialog

IMPLEMENT_DYNAMIC(CReportDlg, CDialog)

CReportDlg::CReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_REPORT, pParent)
{

}

CReportDlg::~CReportDlg()
{
}

void CReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReportDlg, CDialog)
   ON_BN_CLICKED(IDC_PRINT, &CReportDlg::OnClickedPrint)
END_MESSAGE_MAP()


// CReportDlg message handlers


BOOL CReportDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   m_Font.CreatePointFont(100, _T("Courier New"));

   CString strHeader;
   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      strHeader =
         CString("                                             +---------- T = Transverse, S = Seismic\r\n") +
         CString("                                             | +-------- L = Lump sum\r\n") +
         CString("                                             | | +------ S = Substructure\r\n") +
         CString("                                             | | | +---- E = Epoxy\r\n") +
         CString("                                             | | | | +-- V = Varies\r\n") +
         CString("                                             | | | | |\r\n") +
         CString("Mark Description                  Si Num  Be | | | | | #     U        W        X       Y       Z    T1  T2   LENGTH     MASS\r\n") +
         CString(" #                                ze Reqd nd T L S E V Ea  METER    METER    METER   METER   METER  DEG DEG   METER      KG\r\n") +
         CString("==== ============================ == ==== == = = = = = == ===.===  ===.===  ===.=== ===.=== ===.=== === ===  ===.===   =====");
   }
   else
   {
      strHeader = 
         CString("                                             +---------- T = Transverse, S = Seismic\r\n") +
         CString("                                             | +-------- L = Lump sum\r\n") +
         CString("                                             | | +------ S = Substructure\r\n") +
         CString("                                             | | | +---- E = Epoxy\r\n") +
         CString("                                             | | | | +-- V = Varies\r\n") +
         CString("                                             | | | | |\r\n") +
         CString("Mark Description                  Si Num  Be | | | | | #     U        W         X        Y        Z    T1  T2   LENGTH    WEIGHT\r\n") +
         CString(" #                                ze Reqd nd T L S E V Ea  FT  IN   FT  IN   FT  IN   FT  IN   FT  IN  DEG DEG   FT  IN   LBS\r\n") +
         CString("==== ============================ == ==== == = = = = = == === ==.= === ==.= === ==.= === ==.= === ==.= === ===  === ==.=  =====");
   }
   CWnd* pHeader = GetDlgItem(IDC_HEADER);
   pHeader->SetFont(&m_Font);
   pHeader->SetWindowText(strHeader);

   CWnd* pReport = GetDlgItem(IDC_REPORT);
   pReport->SetFont(&m_Font);

   CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();
   CComPtr<IBarlist> barlist;
   pDoc->GetBarlist(&barlist);
   CReport report;
   pReport->SetWindowText(report.GetReport(barlist));

   CButton* pPrint = (CButton*)GetDlgItem(IDC_PRINT);
   HICON hPrintIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PRINT), IMAGE_ICON, 16, 16, 0);
   pPrint->SetIcon(hPrintIcon);


   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}


void CReportDlg::OnClickedPrint()
{
   // TODO: Add your control notification handler code here
#pragma Reminder("WORKING HERE - implement printing")
   AfxMessageBox(_T("Implement"));
}
