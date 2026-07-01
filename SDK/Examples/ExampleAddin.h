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

// ExampleAddin.h : Declaration of the CExampleAddin
//
// This is the minimal, current example of a native Barlist plugin: a plain
// C++ class implementing IBarlistPlugin (SDK/Include/BarlistPlugin.h),
// created and owned by the host application through WBFL's EAF component
// module/manager -- there is no COM registration, no CComModule/ATL object
// map, and no IDL/type library involved.
//
// This DLL still requires MFC (UseOfMfc=Dynamic): IBarlistPlugin itself
// takes and returns MFC types (CEAFDocument, CString), so any plugin
// implementing it needs MFC regardless of whether the plugin has any UI
// of its own.

#pragma once

#include "BarlistPlugin.h"
#include <EAF\ComponentObject.h>

class CBarlist;

class CExampleApp : public CWinApp
{
public:
   virtual BOOL InitInstance() override;
   virtual int ExitInstance() override;
};

/////////////////////////////////////////////////////////////////////////////
// CExampleAddin
//
// Counts the groups and bar records in the barlist and reports them in a
// message box -- enough to show the shape of the native API (CBarlist,
// CGroupCollection, CGroup, CBarRecordCollection) without requiring any
// WBFL::Units-dependent formatting.
class CExampleAddin : public WBFL::EAF::ComponentObject,
   public IBarlistPlugin
{
public:
   CExampleAddin()
   {
   }

// IBarlistPlugin
public:
   void Init(CEAFDocument* pDoc) override;
   void Terminate() override;
   void Go(CBarlist& barlist) override;
   CString GetMenuItem() const override;
};
