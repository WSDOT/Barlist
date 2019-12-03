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

#include "Helpers.h"
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
   DDX_Control(pDX, IDC_QUANTITIES_LIST, m_QuantitiesList);
}


BEGIN_MESSAGE_MAP(CQuantitiesDlg, CDialog)
   ON_WM_CREATE()
   ON_WM_DESTROY()
//   ON_WM_CLOSE()
   ON_WM_SHOWWINDOW()
   ON_WM_SIZE()
   ON_WM_DESTROY()
END_MESSAGE_MAP()


// CQuantitiesDlg message handlers


BOOL CQuantitiesDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   CRect rect;
   GetWindowRect(&rect);
   m_cxMin = rect.Width();
   m_cyMin = rect.Height();

   RestoreWindowPosition();

   m_QuantitiesList.GetClientRect(&rect);
   int width = rect.right / 5;
   
   int col = 0;
   m_QuantitiesList.InsertColumn(col++, _T("Material"), LVCFMT_LEFT, width);
   m_QuantitiesList.InsertColumn(col++, _T("Substructure"), LVCFMT_LEFT, width);
   m_QuantitiesList.InsertColumn(col++, _T("Substructure with Epoxy"), LVCFMT_LEFT, width);
   m_QuantitiesList.InsertColumn(col++, _T("Superstructure"), LVCFMT_LEFT, width);
   m_QuantitiesList.InsertColumn(col++, _T("Superstructure with Epoxy"), LVCFMT_LEFT, width);

   for (int i = 0; i < MATERIAL_COUNT; i++)
   {
      MaterialType material = (MaterialType)i;
      CString strMaterial = GetMaterialSpecification(material);
      m_QuantitiesList.InsertItem(m_QuantitiesList.GetItemCount(), strMaterial);
   }


   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuantitiesDlg::SetGroup(BSTR bstrGroup)
{
   GetDlgItem(IDC_GROUP)->SetWindowText(CString(bstrGroup));
}

void CQuantitiesDlg::SetQuantities(MaterialType material, Float64 sub, Float64 subEpoxy, Float64 super, Float64 superEpoxy)
{
   CString strSub;
   CString strSubEpoxy;
   CString strSuper;
   CString strSuperEpoxy;

   if (material == D7957)
   {
      // GFRP
      strSub = FormatLength(sub);
      strSubEpoxy = _T("-");
      strSuper = FormatLength(super);
      strSuperEpoxy = _T("-");
   }
   else
   {
      strSub = FormatMass(sub);
      strSubEpoxy = CanBeEpoxyCoated(material) ? FormatMass(subEpoxy) : _T("-");
      strSuper = FormatMass(super);
      strSuperEpoxy = CanBeEpoxyCoated(material) ? FormatMass(superEpoxy) : _T("-");
   }

   int row = (int)(material)-(int)A706_Grade60;
   m_QuantitiesList.SetItemText(row, 1, strSub);
   m_QuantitiesList.SetItemText(row, 2, strSubEpoxy);
   m_QuantitiesList.SetItemText(row, 3, strSuper);
   m_QuantitiesList.SetItemText(row, 4, strSuperEpoxy);
}

void CQuantitiesDlg::Clear()
{
   //m_QuantitiesList.DeleteAllItems();
   int nItems = m_QuantitiesList.GetItemCount();
   for (int row = 0; row < nItems; row++)
   {
      m_QuantitiesList.SetItemText(row, 1, _T("-"));
      m_QuantitiesList.SetItemText(row, 2, _T("-"));
      m_QuantitiesList.SetItemText(row, 3, _T("-"));
      m_QuantitiesList.SetItemText(row, 4, _T("-"));
   }
}

int CQuantitiesDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CDialog::OnCreate(lpCreateStruct) == -1)
      return -1;

   ShowWindow(SW_HIDE);

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

void CQuantitiesDlg::RestoreWindowPosition()
{
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
         // if window is within the desktop area, set its position... otherwise the default position will be used
         SetWindowPlacement(&wp);
      }
      else
      {
         ShowWindow(wp.showCmd);
      }
   }
}


void CQuantitiesDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
   if (nStatus == SW_PARENTCLOSING)
   {
      SaveWindowPosition();
      m_bClosedByParent = true; // the parent is closing the window, so we don't want OnDestry to call SaveWindowPosition()
      // SaveWindowPositions saves the visible state of the window (Show/Hide). By the time OnDestroy calls it, the window
      // is always invisible so the true state at closing gets lost. We save the window position information here and skip
      // in OnDestroy
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

LRESULT CQuantitiesDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CQuantitiesDlg::OnSize(UINT nType, int cx, int cy)
{
   CDialog::OnSize(nType, cx, cy);

   //if (m_QuantitiesList.GetSafeHwnd())
   //{
   //   RECT rect;
   //   m_QuantitiesList.GetClientRect(&rect);
   //   int width = rect.right / 5;

   //   int col = 0;
   //   m_QuantitiesList.SetColumnWidth(col++, width);
   //   m_QuantitiesList.SetColumnWidth(col++, width);
   //   m_QuantitiesList.SetColumnWidth(col++, width);
   //   m_QuantitiesList.SetColumnWidth(col++, width);
   //   m_QuantitiesList.SetColumnWidth(col++, width);
   //}
}