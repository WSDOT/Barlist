///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2023  Washington State Department of Transportation
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

#include "StdAfx.h"
#include "Helpers.h"

#include <Units\Units.h>
#include <EAF\EAFApp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static LPCTSTR gs_Material_US[] =
{
   _T("A706 Grade 60"),
   _T("A706 Grade 80"),
   _T("A1035 Grade 100"),
   _T("A1035 Grade 120"),
   _T("A767/A1094 Grade 60"),
   _T("A767/A1094 Grade 80"),
   _T("A767/A1094 Grade 100"),
   _T("A955 Grade 60"),
   _T("A955 Grade 75"),
   _T("A955 Grade 80"),
   _T("D7957")
};

static LPCTSTR gs_Material_SI[] =
{
   _T("A706 Grade 413 MPa"),
   _T("A706 Grade 552 MPa"),
   _T("A1035 Grade 690"),
   _T("A1035 Grade 827"),
   _T("A767/A1094 Grade 413"),
   _T("A767/A1094 Grade 552"),
   _T("A767/A1094 Grade 690"),
   _T("A955 Grade 413"),
   _T("A955 Grade 517"),
   _T("A955 Grade 552"),
   _T("D7957")
};

LPCTSTR GetMaterialSpecification(MaterialType material)
{
   return (EAFGetApp()->GetUnitsMode() == eafTypes::umUS ? gs_Material_US[material] : gs_Material_SI[material]);
}

LPCTSTR GetMaterialCommonName(MaterialType material)
{
   if (IsA706(material))
   {
      return _T("Low Alloy Carbon Steel");
   }
   else if (IsMMFX(material))
   {
      return _T("Chromium/MMFX");
   }
   else if (IsGalvanized(material))
   {
      return _T("Galvanized");
   }
   else if (IsStainless(material))
   {
      return _T("Stainless Steel");
   }
   else if (IsGFRP(material))
   {
      return _T("GFRP");
   }

   ATLASSERT(false);// is there a new type?
   return _T("Unknown");
}

CString GetMaterialDesignation(MaterialType material)
{
   if (IsA706(material))
   {
      return _T("  ");
   }
   else if (IsMMFX(material))
   {
      return _T("CR");
   }
   else if (IsGalvanized(material))
   {
      return _T("G ");
   }
   else if (IsStainless(material))
   {
      return _T("SS");
   }
   else if (IsGFRP(material))
   {
      return _T("GF");
   }

   ATLASSERT(false);// is there a new type?
   return _T("??");
}

CString GetMaterialGrade(MaterialType material)
{
   eafTypes::UnitMode unitMode = EAFGetApp()->GetUnitsMode();
   CString strGrade;
   switch (material)
   {
   case A706_Grade60:
   case A767_A1094_Grade60:
   case A955_Grade60:
      strGrade = (unitMode == eafTypes::umUS ? _T("  ") : _T("41"));
      break;

   case A955_Grade75: 
      strGrade = (unitMode == eafTypes::umUS ? _T("75") : _T("52"));
      break;

   case A706_Grade80:
   case A767_A1094_Grade80:
   case A955_Grade80:
      strGrade = (unitMode == eafTypes::umUS ? _T("80") : _T("55"));
      break;

   case A1035_Grade100:
   case A767_A1094_Grade100:
      strGrade = (unitMode == eafTypes::umUS ? _T("1X") : _T("69"));
      break;

   case A1035_Grade120:
      strGrade = (unitMode == eafTypes::umUS ? _T("12") : _T("83"));
      break;

   case D7957:
      strGrade = _T("  ");
      break;

   default:
      ATLASSERT(false); // is there a new material
      strGrade = _T("??");
   }

   return strGrade;
}
