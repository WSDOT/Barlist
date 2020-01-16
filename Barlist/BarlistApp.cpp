///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2019  Washington State Department of Transportation
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

// BarlistPlugin.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f BarlistPluginps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>

#include <EAF\EAFAppPlugin.h>
#include "Barlist_i.h"
#include "Barlist_i.c"
#include "Plugin.h"

#include <WBFLUnitServer.h>
#include <WBFLUnitServer_i.c>

#include "..\Include\BarlistCATID.h"
#include <BridgeLinkCATID.h>
#include <System\ComCatMgr.h>

#include "BarlistApp.h"

#include "ComponentInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_Plugin, CPlugin)
OBJECT_ENTRY(CLSID_BarlistComponentInfo, CBarlistComponentInfo)
END_OBJECT_MAP()

BEGIN_MESSAGE_MAP(CBarlistPluginApp, CWinApp)
	//{{AFX_MSG_MAP(CBarlistPluginApp)
//	ON_COMMAND(ID_NEW_VIEW, OnNewView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBarlistPluginApp theApp;

BOOL CBarlistPluginApp::InitInstance()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   // Using a shared menu
   // See MSKB Article ID: Q118435, "Sharing Menus Between MDI Child Windows"
   m_hSharedMenu = nullptr; /*::LoadMenu( m_hInstance, MAKEINTRESOURCE(IDR_???) );*/

   _Module.Init(ObjectMap, m_hInstance, &LIBID_BARLISTLib);
   return CWinApp::InitInstance();
}

int CBarlistPluginApp::ExitInstance()
{
   // if the doc template uses a shared menu, destroy it here
   // release the shared menu
   //::DestroyMenu( m_hSharedMenu );

   _Module.Term();
   return CWinApp::ExitInstance();
}

BOOL CBarlistPluginApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
   // in order scan the message map for this plugin, the module state must be set
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CWinApp::OnCmdMsg(nID,nCode,pExtra,pHandlerInfo);
}

void CBarlistPluginApp::OnNewView() 
{
   AfxMessageBox(_T("Handling OnNewView from CBarlistPluginApp"));
}


/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
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

   sysComCatMgr::RegWithCategory(CLSID_Plugin, CATID_BarlistAppPlugin, true);
   sysComCatMgr::RegWithCategory(CLSID_BarlistComponentInfo,CATID_BarlistComponentInfo,true);

   sysComCatMgr::RegWithCategory(CLSID_Plugin, CATID_BridgeLinkAppPlugin, true);
   sysComCatMgr::RegWithCategory(CLSID_BarlistComponentInfo, CATID_BridgeLinkComponentInfo, true);

   return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
   sysComCatMgr::RegWithCategory(CLSID_Plugin, CATID_BarlistAppPlugin, false);
   sysComCatMgr::RegWithCategory(CLSID_BarlistComponentInfo,CATID_BarlistComponentInfo,false);

   sysComCatMgr::RegWithCategory(CLSID_Plugin, CATID_BridgeLinkAppPlugin, false);
   sysComCatMgr::RegWithCategory(CLSID_BarlistComponentInfo, CATID_BridgeLinkComponentInfo, false);

   return _Module.UnregisterServer(TRUE);
}
