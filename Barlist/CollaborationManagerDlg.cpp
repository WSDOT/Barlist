///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2025  Washington State Department of Transportation
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


// CollaborationManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CollaborationManagerDlg.h"
#include "afxdialogex.h"

#include <EAF\EAFDocument.h>


// CCollaborationManagerDlg dialog

IMPLEMENT_DYNAMIC(CCollaborationManagerDlg, CDialog)

CCollaborationManagerDlg::CCollaborationManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_COLLABORATION_MANAGER, pParent)
{

}

CCollaborationManagerDlg::~CCollaborationManagerDlg()
{
}

void CCollaborationManagerDlg::DoDataExchange(CDataExchange* pDX)
{
   USES_CONVERSION;
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_FILES, m_lbFiles);

   if (pDX->m_bSaveAndValidate)
   {
      m_vFiles.clear();
      int nItems = m_lbFiles.GetCount();
      m_vFiles.reserve(nItems);

      for (int i = 0; i < nItems; i++)
      {
         CString strFile;
         m_lbFiles.GetText(i, strFile);
         m_vFiles.push_back(std::string(T2A(strFile.GetBuffer())));
      }
   }
   else
   {
      m_lbFiles.ResetContent();
      for (const auto& strFile : m_vFiles)
      {
         m_lbFiles.AddString(A2T(strFile.c_str()));
      }
   }
}


BEGIN_MESSAGE_MAP(CCollaborationManagerDlg, CDialog)
   ON_WM_DROPFILES()
   ON_WM_QUERYDRAGICON()
   ON_BN_CLICKED(IDC_REMOVE, &CCollaborationManagerDlg::OnBnClickedRemove)
   ON_BN_CLICKED(IDC_MOVE_UP, &CCollaborationManagerDlg::OnBnClickedMoveUp)
   ON_BN_CLICKED(IDC_MOVE_DOWN, &CCollaborationManagerDlg::OnBnClickedMoveDown)
   ON_LBN_SELCHANGE(IDC_FILES, &CCollaborationManagerDlg::OnSelchangeFiles)
   ON_BN_CLICKED(IDC_ADD, &CCollaborationManagerDlg::OnBnClickedAdd)
   ON_BN_CLICKED(IDCHELP, &CCollaborationManagerDlg::OnBnClickedChelp)
   ON_WM_DESTROY()
   ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CCollaborationManagerDlg message handlers


BOOL CCollaborationManagerDlg::OnInitDialog()
{
   USES_CONVERSION;
   CDialog::OnInitDialog();

   CRect rect;
   GetWindowRect(&rect);
   m_cxMin = rect.Width();
   m_cyMin = rect.Height();

   DragAcceptFiles();
   m_lbFiles.DragAcceptFiles();
   UpdateButtons();

   // TODO:  Add extra initialization here

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CCollaborationManagerDlg::OnDropFiles(HDROP dropInfo)
{
   CString strFile;
   DWORD nBuffer = 0;

   int nFilesDropped = DragQueryFile(dropInfo, 0xFFFFFFFF, NULL, 0);
   for (int i = 0; i < nFilesDropped; i++)
   {
      nBuffer = DragQueryFile(dropInfo, i, NULL, 0);
      DragQueryFile(dropInfo, i, strFile.GetBuffer(nBuffer + 1), nBuffer + 1);
      strFile.ReleaseBuffer();
      CString strExt(PathFindExtension(strFile));
      if (strExt == _T(".bar"))
      {
         m_lbFiles.AddString(strFile);
      }
   }
   DragFinish(dropInfo);
   UpdateButtons();
}


HCURSOR CCollaborationManagerDlg::OnQueryDragIcon()
{
   // TODO: Add your message handler code here and/or call default

   return CDialog::OnQueryDragIcon();
}


void CCollaborationManagerDlg::OnBnClickedRemove()
{
   m_lbFiles.DeleteString(m_lbFiles.GetCurSel());
}


void CCollaborationManagerDlg::OnBnClickedMoveUp()
{
   MoveFiles(-1);
   UpdateButtons();
}


void CCollaborationManagerDlg::OnBnClickedMoveDown()
{
   MoveFiles(1);
   UpdateButtons();
}

void CCollaborationManagerDlg::MoveFiles(int step)
{
   int curSel = m_lbFiles.GetCurSel();
   int nItems = m_lbFiles.GetCount();
   if ((step < 0 && curSel == 0) || (0 < step && curSel == nItems - 1))
   {
      // can't move item above top or below bottom
      return;
   }

   CString strItem;
   m_lbFiles.GetText(curSel, strItem);
   m_lbFiles.DeleteString(curSel);
   curSel += step;
   m_lbFiles.InsertString(curSel, strItem);
   m_lbFiles.SetCurSel(curSel);
}

void CCollaborationManagerDlg::UpdateButtons()
{
   BOOL bEnableUp = TRUE;
   BOOL bEnableDown = TRUE;
   BOOL bEnableRemove = TRUE;
   int curSel = m_lbFiles.GetCurSel();
   int nItems = m_lbFiles.GetCount();

   if (curSel == LB_ERR)
   {
      bEnableUp = FALSE;
      bEnableDown = FALSE;
      bEnableRemove = FALSE;
   }
   else
   {
      if (curSel == 0)
      {
         bEnableUp = FALSE;
      }

      if (curSel == nItems - 1)
      {
         bEnableDown = FALSE;
      }
   }

   GetDlgItem(IDC_MOVE_UP)->EnableWindow(bEnableUp);
   GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(bEnableDown);
   GetDlgItem(IDC_REMOVE)->EnableWindow(bEnableRemove);
}

void CCollaborationManagerDlg::OnSelchangeFiles()
{
   UpdateButtons();
}


void CCollaborationManagerDlg::OnBnClickedAdd()
{
   CFileDialog dlg(TRUE, _T("bar"), nullptr, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT,_T("Barlist Files (*.bar)|*.bar|"));
   if (dlg.DoModal() == IDOK)
   {
      POSITION pos = dlg.GetStartPosition();
      while (pos)
      {
         CString strFile(dlg.GetNextPathName(pos));
         m_lbFiles.AddString(strFile);
      }
   }
}


void CCollaborationManagerDlg::OnBnClickedChelp()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(), IDH_COLLABORATION);
}


LRESULT CCollaborationManagerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CCollaborationManagerDlg::OnDestroy()
{
   // Save the layout of the application window
   CEAFApp* pApp = EAFGetApp();
   WINDOWPLACEMENT wp;
   wp.length = sizeof wp;
   if (GetWindowPlacement(&wp))
   {
      pApp->WriteWindowPlacement(_T("Window Positions"), _T("CollaborationManager"), &wp);
   }

   CDialog::OnDestroy();
}

void CCollaborationManagerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
   CDialog::OnShowWindow(bShow, nStatus);

   // Restore the layout of the application window
   if (bShow)
   {
      WINDOWPLACEMENT wp;
      if (EAFGetApp()->ReadWindowPlacement(_T("Window Positions"), _T("CollaborationManager"), &wp))
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
