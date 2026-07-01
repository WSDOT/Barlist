///////////////////////////////////////////////////////////////////////
// Barlist
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

#pragma once

#include <array>
#include <utility>
#include <MfcTools\Format.h>
#include <Bars\StatusMessage.h>

class Formatter
{
public:
   static CString FormatMass(Float64 mass, bool bUnits = true);
   static CString FormatLength(Float64 length, bool bUnits = true);
   static CString FormatLength(Float64 length, bool bFractionInches, bool bUnits);
   static std::pair<bool, Float64> IsValidLength(const CString& strValue);
   static std::pair<bool, Float64> ParseLength(const CString& strValue);
   static CString FormatStatusValue(const StatusValue& var);
   static CString FormatStatusMessage(const CStatusMessage& statusMessage);
   static std::pair<Int32, Float64> USLength(Float64 length); // returns (feet, inches)

private:
   static WBFL::Units::MassData gs_MassUnit;
   static WBFL::Units::ForceData gs_WeightUnit;
   static std::array<WBFL::Units::LengthData, 2> gs_LengthUnit;
};