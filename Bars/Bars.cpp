///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2024  Washington State Department of Transportation
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


// Bars.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f Barsps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "Bars.h"

#include "BarlistAddinCatid.h"

#include "Bars_i.c"
#include "Barlist_i.c"
#include "BarlistAddin_i.c"
#include "AddinMgr_i.c"

#include "BarInfoMgr.h"
#include "BarRecord.h"
#include "BarRecordCollection.h"
#include "Barlist.h"
#include "AddinMgrImp.h"
#include "Type50.h"
#include "Type51.h"
#include "Type52.h"
#include "Type53.h"
#include "Type54.h"
#include "Type55.h"
#include "Type56.h"
#include "Type57.h"
#include "Type58.h"
#include "Type59.h"
#include "Type60.h"
#include "Type61.h"
#include "Type62.h"
#include "Type63.h"
#include "Type66.h"
#include "Type67.h"
#include "Type68.h"
#include "Type69.h"
#include "Type70.h"
#include "Type71.h"
#include "Type72.h"
#include "Type73.h"
#include "Type74.h"
#include "Type75.h"
#include "Type76.h"
#include "Type77.h"
#include "Type78.h"
#include "Type79.h"
#include "Type80.h"
#include "Type81.h"
#include "Type82.h"
#include "Type83.h"
#include "Type84.h"

#include "Type89.h"
#include "Type90.h"
#include "Type91.h"
#include "Type92.h"
#include "Type93.h"
#include "Type94.h"
#include "Type95.h"
#include "Type96.h"
#include "Type97.h"
#include "Type98.h"
#include "Type99.h"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_BarInfoMgr, CBarInfoMgr)
OBJECT_ENTRY(CLSID_BarRecord, CBarRecord)
OBJECT_ENTRY(CLSID_BarRecordCollection, CBarRecordCollection)
OBJECT_ENTRY(CLSID_Barlist, CBarlist)
OBJECT_ENTRY(CLSID_AddinMgr, CAddinMgr)
OBJECT_ENTRY(CLSID_Type50, CType50)
OBJECT_ENTRY(CLSID_Type51, CType51)
OBJECT_ENTRY(CLSID_Type52, CType52)
OBJECT_ENTRY(CLSID_Type53, CType53)
OBJECT_ENTRY(CLSID_Type54, CType54)
OBJECT_ENTRY(CLSID_Type55, CType55)
OBJECT_ENTRY(CLSID_Type56, CType56)
OBJECT_ENTRY(CLSID_Type57, CType57)
OBJECT_ENTRY(CLSID_Type58, CType58)
OBJECT_ENTRY(CLSID_Type59, CType59)
OBJECT_ENTRY(CLSID_Type60, CType60)
OBJECT_ENTRY(CLSID_Type61, CType61)
OBJECT_ENTRY(CLSID_Type62, CType62)
OBJECT_ENTRY(CLSID_Type63, CType63)
OBJECT_ENTRY(CLSID_Type66, CType66)
OBJECT_ENTRY(CLSID_Type67, CType67)
OBJECT_ENTRY(CLSID_Type68, CType68)
OBJECT_ENTRY(CLSID_Type69, CType69)
OBJECT_ENTRY(CLSID_Type70, CType70)
OBJECT_ENTRY(CLSID_Type71, CType71)
OBJECT_ENTRY(CLSID_Type72, CType72)
OBJECT_ENTRY(CLSID_Type73, CType73)
OBJECT_ENTRY(CLSID_Type74, CType74)
OBJECT_ENTRY(CLSID_Type75, CType75)
OBJECT_ENTRY(CLSID_Type76, CType76)
OBJECT_ENTRY(CLSID_Type77, CType77)
OBJECT_ENTRY(CLSID_Type78, CType78)
OBJECT_ENTRY(CLSID_Type79, CType79)
OBJECT_ENTRY(CLSID_Type80, CType80)
OBJECT_ENTRY(CLSID_Type81, CType81)
OBJECT_ENTRY(CLSID_Type82, CType82)
OBJECT_ENTRY(CLSID_Type83, CType83)
OBJECT_ENTRY(CLSID_Type84, CType84)

OBJECT_ENTRY(CLSID_Type89, CType89)
OBJECT_ENTRY(CLSID_Type90, CType90)
OBJECT_ENTRY(CLSID_Type91, CType91)
OBJECT_ENTRY(CLSID_Type92, CType92)
OBJECT_ENTRY(CLSID_Type93, CType93)
OBJECT_ENTRY(CLSID_Type94, CType94)
OBJECT_ENTRY(CLSID_Type95, CType95)
OBJECT_ENTRY(CLSID_Type96, CType96)
OBJECT_ENTRY(CLSID_Type97, CType97)
OBJECT_ENTRY(CLSID_Type98, CType98)
OBJECT_ENTRY(CLSID_Type99, CType99)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_BARSLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _Module.RegisterServer(TRUE);
    if ( FAILED(hr) )
       return hr;

   // Register the component category for Barlist Add-ins since the VB progam
   // will have trouble doing it.
   
   // Create the standard COM Category Manager
   ICatRegister* pICatReg = NULL;
   hr = ::CoCreateInstance( CLSID_StdComponentCategoriesMgr,
                            NULL,
                            CLSCTX_ALL,
                            IID_ICatRegister,
                            (void**)&pICatReg );
   if ( FAILED(hr) )
      return hr;

   // Fill in CATEGORYINFO struct
   CATEGORYINFO CatInfo;
   CatInfo.catid = CATID_BarlistAddin;
   CatInfo.lcid = LOCALE_SYSTEM_DEFAULT;
   wcscpy_s(CatInfo.szDescription,_countof(CatInfo.szDescription),L"BarList Addin Component");

   // Register the category
   hr = pICatReg->RegisterCategories(1,&CatInfo);
   pICatReg->Release();
   
   return hr;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
   HRESULT hr = _Module.UnregisterServer(TRUE);
   if ( FAILED(hr) )
      return hr;

   // Create the standard COM Category Manager
   CComPtr<ICatRegister> pICatReg = NULL;
   hr = ::CoCreateInstance( CLSID_StdComponentCategoriesMgr,
                            NULL,
                            CLSCTX_ALL,
                            IID_ICatRegister,
                            (void**)&pICatReg );
   if ( FAILED(hr) )
      return hr;

   const int nID = 1;
   CATID ID[nID];
   ID[0] = CATID_BarlistAddin;

   CComPtr<ICatInformation> pICatInfo = NULL;
   hr = pICatReg->QueryInterface( IID_ICatInformation, (void**)&pICatInfo );

   CComPtr<IEnumCLSID> pIEnumCLSID = NULL;
   hr = pICatInfo->EnumClassesOfCategories( nID, ID, 0, NULL, &pIEnumCLSID );

   CLSID clsid;
   while( pIEnumCLSID->Next(1,&clsid,NULL) != S_FALSE )
   {
      pICatReg->UnRegisterCategories(nID,ID);
   }
   
   return S_OK;
}

