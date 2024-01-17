///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2024  Washington State Department of Transportation
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

// BarlistPrintDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BarlistPrintDialog.h"

#ifndef _WIN32_WCE // CPrintDialog is not supported for Windows CE.

// CBarlistPrintDialog

IMPLEMENT_DYNAMIC(CBarlistPrintDialog, CPrintDialog)

CBarlistPrintDialog::CBarlistPrintDialog(BOOL bPrintSetupOnly, DWORD dwFlags, CWnd* pParentWnd) :
	CPrintDialog(bPrintSetupOnly, dwFlags, pParentWnd)
{

}

CBarlistPrintDialog::~CBarlistPrintDialog()
{
}


BEGIN_MESSAGE_MAP(CBarlistPrintDialog, CPrintDialog)
END_MESSAGE_MAP()



// CBarlistPrintDialog message handlers


#endif // !_WIN32_WCE


BOOL CBarlistPrintDialog::OnInitDialog()
{
   CPrintDialog::OnInitDialog();

   // TODO:  Add extra initialization here
   LPDEVMODE pDevMode = GetDevMode();
   GlobalLock(pDevMode);
   pDevMode->dmFields |= DM_ORIENTATION | DM_PAPERSIZE;
   pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
   pDevMode->dmPaperSize = DMPAPER_LETTER;
   GlobalUnlock(pDevMode);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}
