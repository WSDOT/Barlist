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
// QuantitiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QuantitiesDlg.h"
#include "afxdialogex.h"

#include "BarlistDoc.h" // for FormatMass


// CQuantitiesDlg dialog

IMPLEMENT_DYNAMIC(CQuantitiesDlg, CDialog)

CQuantitiesDlg::CQuantitiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_QUANTITIES, pParent)
{

}

CQuantitiesDlg::~CQuantitiesDlg()
{
}

void CQuantitiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQuantitiesDlg, CDialog)
   ON_WM_CREATE()
   ON_WM_DESTROY()
//   ON_WM_CLOSE()
ON_WM_SHOWWINDOW()
ON_WM_DESTROY()
END_MESSAGE_MAP()


// CQuantitiesDlg message handlers


BOOL CQuantitiesDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   // TODO:  Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuantitiesDlg::SetQuantities(BSTR bstrGroup, Float64 sub, Float64 subEpoxy, Float64 super, Float64 superEpoxy)
{
   GetDlgItem(IDC_GROUP)->SetWindowText(CString(bstrGroup));
   GetDlgItem(IDC_SUBSTRUCTURE)->SetWindowText(FormatMass(sub));
   GetDlgItem(IDC_SUBSTRUCTURE_EPOXY)->SetWindowText(FormatMass(subEpoxy));
   GetDlgItem(IDC_SUPERSTRUCTURE)->SetWindowText(FormatMass(super));
   GetDlgItem(IDC_SUPERSTRUCTURE_EPOXY)->SetWindowText(FormatMass(superEpoxy));
}

void CQuantitiesDlg::Clear()
{
   GetDlgItem(IDC_GROUP)->SetWindowText(_T(""));
   GetDlgItem(IDC_SUBSTRUCTURE)->SetWindowText(_T(""));
   GetDlgItem(IDC_SUBSTRUCTURE_EPOXY)->SetWindowText(_T(""));
   GetDlgItem(IDC_SUPERSTRUCTURE)->SetWindowText(_T(""));
   GetDlgItem(IDC_SUPERSTRUCTURE_EPOXY)->SetWindowText(_T(""));
}

int CQuantitiesDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CDialog::OnCreate(lpCreateStruct) == -1)
      return -1;

   WINDOWPLACEMENT wp;
   if (EAFGetApp()->ReadWindowPlacement(CString(_T("Window Positions")), _T("Quantities"), &wp))
   {
      CWnd* pDesktop = GetDesktopWindow();
      //CRect rDesktop;
      //pDesktop->GetWindowRect(&rDesktop); // this is the size of one monitor.... use GetSystemMetrics to get the entire desktop
      CRect rDesktop(0, 0, GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN));
      CRect rThisWnd(wp.rcNormalPosition);
      if (rDesktop.PtInRect(rThisWnd.TopLeft()) && rDesktop.PtInRect(rThisWnd.BottomRight()))
      {
         // if window is within the desktop area, set its position... otherwise the default position will be sued
         SetWindowPos(NULL, wp.rcNormalPosition.left, wp.rcNormalPosition.top, wp.rcNormalPosition.right - wp.rcNormalPosition.left, wp.rcNormalPosition.bottom - wp.rcNormalPosition.top, 0);
      }
      ShowWindow(wp.showCmd);
   }
   else
   {
      ShowWindow(SW_HIDE);
   }

   return 0;
}

void CQuantitiesDlg::SaveWindowPosition()
{
   WINDOWPLACEMENT wp;
   if (GetWindowPlacement(&wp))
   {
      wp.showCmd = IsWindowVisible() ? SW_SHOW : SW_HIDE;
      EAFGetApp()->WriteWindowPlacement(CString(_T("Window Positions")), _T("Quantities"), &wp);
   }
}


void CQuantitiesDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
   if (nStatus == SW_PARENTCLOSING)
   {
      SaveWindowPosition();
      m_bClosedByParent = true;
   }

   CDialog::OnShowWindow(bShow, nStatus);
}


void CQuantitiesDlg::OnDestroy()
{
   if (!m_bClosedByParent)
   {
      SaveWindowPosition();
   }

   CDialog::OnDestroy();
}
