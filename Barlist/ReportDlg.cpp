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

// ReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReportDlg.h"
#include "afxdialogex.h"
#include <EAF\EAFApp.h>
#include "BarlistDoc.h"


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
    ON_BN_CLICKED(IDC_CHECK_QTY_BY_GROUP, &CReportDlg::UpdateReport)
    ON_WM_DESTROY()
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()





// CReportDlg message handlers

void CReportDlg::UpdateReport()
{
    CWnd* pReport = GetDlgItem(IDC_REPORT);
    pReport->SetFont(&m_Font);

    CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();

    //define options parameter
    
    CButton* pCheckBox = (CButton*)GetDlgItem(IDC_CHECK_QTY_BY_GROUP);
    int reportOptions = 0;

    if (pCheckBox->GetCheck() == BST_CHECKED) {
        reportOptions = CReport::ReportOptions::REPORT_TOTAL_AND_GROUP_QUANTITIES;
    }
    if (pCheckBox->GetCheck() == BST_UNCHECKED) {
        reportOptions = CReport::ReportOptions::REPORT_GROUP_QUANTITIES;
    }

    const auto& vReportLines = pDoc->GetReport(reportOptions).GetReport();
    CString strReport;
    for (auto line : vReportLines)
    {
        line.Replace(_T("\n"), _T("\r\n"));
        strReport += line;
    }
    pReport->SetWindowText(strReport);
}


BOOL CReportDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   UpdateReport();

   m_Font.CreatePointFont(100, _T("Courier New"));

   CString strHeader = CReport::GetReportHeader();
   CWnd* pHeader = GetDlgItem(IDC_HEADER);
   pHeader->SetFont(&m_Font);
   pHeader->SetWindowText(strHeader);

   CButton* pPrint = (CButton*)GetDlgItem(IDC_PRINT);
   HICON hPrintIcon = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PRINT), IMAGE_ICON, 16, 16, 0);
   pPrint->SetIcon(hPrintIcon);

   // minimum size of the window
   CRect rect;
   GetWindowRect(&rect);
   m_cxMin = rect.Width();
   m_cyMin = rect.Height();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportDlg::OnClickedPrint()
{
   // Send the printing command to the main frame and let the
   // command handler deal with it
   EAFGetMainFrame()->PostMessage(WM_COMMAND, ID_FILE_PRINT);
}


void CReportDlg::OnDestroy()
{
   WINDOWPLACEMENT wp;
   wp.length = sizeof wp;
   if (GetWindowPlacement(&wp))
   {
      EAFGetApp()->WriteWindowPlacement(_T("Window Positions"), _T("Report"), &wp);
   }

   CDialog::OnDestroy();
}


void CReportDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
   CDialog::OnShowWindow(bShow, nStatus);

   if (bShow)
   {
      WINDOWPLACEMENT wp;
      if (EAFGetApp()->ReadWindowPlacement(_T("Window Positions"), _T("Report"), &wp))
      {
         HMONITOR hMonitor = MonitorFromRect(&wp.rcNormalPosition, MONITOR_DEFAULTTONULL); // get the monitor that has maximum overlap with the dialog rectangle (returns null if none)
         if (hMonitor != NULL)
         {
            // if dialog is within a monitor, set its position... otherwise the default position will be sued
            SetWindowPos(NULL, wp.rcNormalPosition.left, wp.rcNormalPosition.top, wp.rcNormalPosition.right - wp.rcNormalPosition.left, wp.rcNormalPosition.bottom - wp.rcNormalPosition.top, 0);
         }
      }
   }
}


LRESULT CReportDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
   // prevent the dialog from getting smaller than the original size
   if (message == WM_SIZING)
   {
      LPRECT rect = (LPRECT)lParam;
      int cx = rect->right - rect->left;
      int cy = rect->bottom - rect->top;

      if (cx < m_cxMin || cy < m_cyMin)
      {
         // prevent the dialog from moving right or down
         if (wParam == WMSZ_BOTTOMLEFT ||
            wParam == WMSZ_LEFT ||
            wParam == WMSZ_TOP ||
            wParam == WMSZ_TOPLEFT ||
            wParam == WMSZ_TOPRIGHT)
         {
            CRect r;
            GetWindowRect(&r);
            rect->left = r.left;
            rect->top = r.top;
         }

         if (cx < m_cxMin)
         {
            rect->right = rect->left + m_cxMin;
         }

         if (cy < m_cyMin)
         {
            rect->bottom = rect->top + m_cyMin;
         }

         return TRUE;
      }
   }

   return CDialog::WindowProc(message, wParam, lParam);
}
