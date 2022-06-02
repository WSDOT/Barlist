///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2022  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This software was developed as part of the Alternate Route Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation,
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful,
// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
// PURPOSE.  See the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Open Source License
// along with this program; if not, write to the Washington State
// Department of Transportation, Bridge and Structures Office,
// 4500 3rd Ave SE, P.O. Box 47340, Olympia, WA 98503, USA or e-mail
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////


// AddinMgrImp.cpp : Implementation of CAddinMgr
#include "stdafx.h"
#include "BarlistAddinCatid.h"
#include "Bars.h"
#include "AddinMgrImp.h"
#include "ComCat.h"
#include <tchar.h>

/////////////////////////////////////////////////////////////////////////////
// CAddinMgr
HRESULT CAddinMgr::FinalConstruct()
{
   // Load up all the Addins
   CComPtr<ICatRegister> pICatReg = 0;
   ::CoCreateInstance( CLSID_StdComponentCategoriesMgr,
                       NULL,
                       CLSCTX_INPROC_SERVER,
                       IID_ICatRegister,
                       (void**)&pICatReg );

   CComQIPtr<ICatInformation,&IID_ICatInformation> pICatInfo(pICatReg);
   CComPtr<IEnumCLSID> pIEnumCLSID = 0;

   const int nID = 1;
   CATID ID[nID];
   ID[0] = CATID_BarlistAddin;

   pICatInfo->EnumClassesOfCategories(nID,ID,0,NULL,&pIEnumCLSID);

   CLSID clsid;
   ULONG cFetched;

   while ( pIEnumCLSID->Next( 1, &clsid, &cFetched ) != S_FALSE )
   {
      CComPtr<IBarlistAddin> pAddin;
      HRESULT hr = ::CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IBarlistAddin, (void**)&pAddin);
      if (FAILED(hr))
      {
         //TCHAR strMsg[256];

         //USES_CONVERSION;
         //LPOLESTR pszCLSID;
         //::StringFromCLSID(clsid, &pszCLSID);

         //LPOLESTR pszUserType;
         //OleRegGetUserType(clsid, USERCLASSTYPE_SHORT, &pszUserType);

         //_stprintf_s(strMsg, sizeof(strMsg)/sizeof(TCHAR), _T("Failed to load %s Add-in (%s).\nError code %#x"), OLE2T(pszUserType), OLE2T(pszCLSID), hr);
         //::MessageBox(nullptr, strMsg, _T("Error"), MB_ICONERROR | MB_OK);

         //::CoTaskMemFree((void*)pszCLSID);
         continue;
      }

      CComPtr<IUnknown> pUnk(pAddin);
      CComVariant var(pUnk);
      m_coll.push_back(var);
   }

   return S_OK;
}

STDMETHODIMP CAddinMgr::get_Item(short index, IBarlistAddin* *pVal)
{
   if ( index < 0 || m_coll.size() <= index )
      return E_INVALIDARG;

   CComVariant& var = m_coll[index];
   return var.punkVal->QueryInterface( pVal );
}
