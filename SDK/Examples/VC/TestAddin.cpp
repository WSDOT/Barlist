// TestAddin.cpp : Implementation of CTestAddin
#include "stdafx.h"
#include "Addin.h"
#include "TestAddin.h"

/////////////////////////////////////////////////////////////////////////////
// CTestAddin
STDMETHODIMP CTestAddin::Go(IBarlist* pBarlist)
{
   if ( pBarlist == NULL )
      ::MessageBox(0,_T("Barlist is NULL"),_T(""), MB_OK);

   ::MessageBox(0,_T("C++ Addin Called"),_T("Test"),MB_OK);
   return S_OK;
}

STDMETHODIMP CTestAddin::get_MenuItem(BSTR *pVal)
{
   CComBSTR menuItem("C++ Add-in");
   *pVal = menuItem.Copy();
   return S_OK;
}

