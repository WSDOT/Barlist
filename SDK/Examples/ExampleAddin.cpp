///////////////////////////////////////////////////////////////////////
// Barlist SDK Example - ExampleAddin
// Copyright � 1999-2026  Washington State Department of Transportation
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

// ExampleAddin.cpp : Implementation of CExampleAddin
#include "stdafx.h"

#include "ExampleAddin.h"
#include "CLSID.h"

#include <System\Logger.h>
#include <EAF\ComponentModule.h>

#include <Bars\Barlist.h>
#include <Bars\Group.h>
#include <Bars\GroupCollection.h>
#include <Bars\BarRecordCollection.h>
#include <Bars\BarRecord.h>

CExampleApp theApp;
WBFL::EAF::ComponentModule _Module;

void CExampleAddin::Init(CEAFDocument* pDoc)
{
}

void CExampleAddin::Terminate()
{
}

void CExampleAddin::Go(CBarlist& barlist)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   std::size_t nBarRecords = 0;
   CGroupCollection& groups = barlist.GetGroups();
   for (const auto& group : groups)
   {
      nBarRecords += group->GetBarRecords().Count();
   }

   CString strMsg;
   strMsg.Format(_T("This barlist has %d group(s) and %d bar record(s)."),
      (int)groups.Count(), (int)nBarRecords);
   AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
}

CString CExampleAddin::GetMenuItem() const
{
   return CString("SDK Example Add-in");
}

// The object map tells WBFL::EAF::ComponentModule which CLSIDs this DLL can
// create and which native class to construct for each. ComponentModule.h
// itself uses BOOST_DLL_ALIAS to export the well-known factory symbol
// (create_class_object) that the host application looks up by name when it
// loads this DLL -- there is no DllGetClassObject/DllRegisterServer and
// nothing to register in the Windows registry.
EAF_BEGIN_OBJECT_MAP(ObjectMap)
   EAF_OBJECT_ENTRY(CLSID_ExampleAddin, CExampleAddin)
EAF_END_OBJECT_MAP()

BOOL CExampleApp::InitInstance()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   _Module.Init(ObjectMap);

   return CWinApp::InitInstance();
}

int CExampleApp::ExitInstance()
{
   _Module.Term();
   return CWinApp::ExitInstance();
}
