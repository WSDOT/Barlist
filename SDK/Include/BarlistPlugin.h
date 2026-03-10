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

#pragma once

#include <initguid.h>
#include <afxstr.h>

// {88FF9D4E-1F47-11d3-8941-006097C68A9C}
DEFINE_GUID(CATID_BarlistPlugin, 
0x88ff9d4e, 0x1f47, 0x11d3, 0x89, 0x41, 0x0, 0x60, 0x97, 0xc6, 0x8a, 0x9c);

interface IBarlist;
class CEAFDocument;

class IBarlistPlugin
{
public:
   virtual void Init(CEAFDocument* pDoc) = 0;
   virtual void Terminate() = 0;
   virtual void Go(IBarlist* pBarlist) = 0;
   virtual CString GetMenuItem() const = 0;
};