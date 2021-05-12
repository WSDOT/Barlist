///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2021  Washington State Department of Transportation
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

#include <array>
#include <MfcTools\Format.h>
#include "..\Bars\Barlst.h"

class Formatter
{
public:
   static BOOL Init();
   static CString FormatMass(Float64 mass, bool bUnits = true);
   static CString FormatLength(Float64 length, bool bUnits = true);
   static CString FormatLength(Float64 length, bool bFractionInches, bool bUnits);
   static bool IsValidLength(const CString& strValue, Float64* pValue);
   static bool ParseLength(const CString& strValue, Float64* pValue);
   static CString FormatStatusValue(CComVariant& var);
   static CString FormatStatusMessage(IStatusMessage* pStatusMessage);

private:
   static unitmgtMassData gs_MassUnit;
   static unitmgtForceData gs_WeightUnit;
   static std::array<unitmgtLengthData, 2> gs_LengthUnit;
   static CComPtr<IAnnotatedDisplayUnitFormatter> g_formatter;
};