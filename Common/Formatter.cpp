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

#include "stdafx.h"
#include "Formatter.h"
#include <MfcTools\Format.h>
#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>
#include <Units\AnnotatedDisplayUnitFormatter.h>

WBFL::Units::MassData Formatter::gs_MassUnit(WBFL::Units::Measure::Kilogram, 0.001, 9, 0);
WBFL::Units::ForceData Formatter::gs_WeightUnit(WBFL::Units::Measure::Pound, 0.001, 9, 0);
std::array<WBFL::Units::LengthData, 2> Formatter::gs_LengthUnit{ WBFL::Units::LengthData(WBFL::Units::Measure::Meter,0.001,9,0),WBFL::Units::LengthData(WBFL::Units::Measure::Feet,0.001,9,0) };

namespace
{
   const WBFL::Units::AnnotatedDisplayUnitFormatter& GetLengthFormatter()
   {
      static WBFL::Units::AnnotatedDisplayUnitFormatter formatter = []
      {
         WBFL::Units::AnnotatedDisplayUnitFormatter f;
         f.SetAnnotation(_T("'-,\""));
         f.SetMultiplier(12.0);
         f.SetOffsetDigits(0);
         f.SetFormatSpecifiers(7, 1, WBFL::Units::AnnotatedDisplayUnitFormatter::Justify::Right, WBFL::System::NumericFormatTool::Format::Fixed, 0.0001);
         return f;
      }();
      return formatter;
   }
}

CString Formatter::FormatMass(Float64 mass, bool bUnits)
{
   CString strMass;
   if (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI)
   {
      if (bUnits)
      {
         strMass.Format(_T("%5.0f %s"), ConvertFromSysUnits(mass, gs_MassUnit.UnitOfMeasure), gs_MassUnit.UnitOfMeasure.UnitTag().c_str());
      }
      else
      {
         strMass.Format(_T("%5.0f"), ConvertFromSysUnits(mass, gs_MassUnit.UnitOfMeasure));
      }
   }
   else
   {
      if (bUnits)
      {
         strMass.Format(_T("%5.0f %s"), ConvertFromSysUnits(mass, gs_WeightUnit.UnitOfMeasure), gs_WeightUnit.UnitOfMeasure.UnitTag().c_str());
      }
      else
      {
         strMass.Format(_T("%5.0f"), ConvertFromSysUnits(mass, gs_WeightUnit.UnitOfMeasure));
      }
   }
   return strMass;
}

CString Formatter::FormatLength(Float64 length, bool bUnits)
{
   if (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI)
   {
      CString strLength;
      if (bUnits)
      {
         strLength.Format(_T("%7.3f %s"), ConvertFromSysUnits(length, gs_LengthUnit[0].UnitOfMeasure), gs_LengthUnit[0].UnitOfMeasure.UnitTag().c_str());
      }
      else
      {
         strLength.Format(_T("%7.3f"), ConvertFromSysUnits(length, gs_LengthUnit[0].UnitOfMeasure));
      }
      return strLength;
   }
   else
   {
      if (bUnits)
      {
         return CString(GetLengthFormatter().AsString(WBFL::Units::ConvertFromSysUnits(length, gs_LengthUnit[1].UnitOfMeasure)).c_str());
      }
      else
      {
         // convert from system units
         length = WBFL::Units::ConvertFromSysUnits(length, gs_LengthUnit[1].UnitOfMeasure);
         int sign = BinarySign(length);
         length = fabs(length);
         long feet = (long)floor(length);
         Float64 inches = (length - feet) * 12;
         if (IsEqual(inches, 12.0))
         {
            // don't want 6'-12"... make it 7'-0"
            feet++;
            inches = 0;
         }

         CString strBuffer;
         strBuffer.Format(_T("%s%3d %4.1f"), sign < 0 ? _T("-") : _T(""), feet, inches);

         CString strLength;
         strLength.Format(_T("%-s"), strBuffer);
         return strLength;
      }
   }
}

CString Formatter::FormatLength(Float64 length, bool bFractionInches, bool bUnits)
{
   if (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI)
   {
      CString strLength;
      if (bUnits)
      {
         strLength.Format(_T("%7.3f %s"), ConvertFromSysUnits(length, gs_LengthUnit[0].UnitOfMeasure), gs_LengthUnit[0].UnitOfMeasure.UnitTag().c_str());
      }
      else
      {
         strLength.Format(_T("%7.3f"), ConvertFromSysUnits(length, gs_LengthUnit[0].UnitOfMeasure));
      }
      return strLength;
   }
   else
   {
      if (bUnits)
      {
         return CString(GetLengthFormatter().AsString(WBFL::Units::ConvertFromSysUnits(length, gs_LengthUnit[1].UnitOfMeasure)).c_str());
      }
      else
      {
         // convert from system units
         auto [feet, inches] = USLength(length);
         int sign = BinarySign(length);
         feet = abs(feet);

         CString strBuffer;
         if (bFractionInches)
         {
            strBuffer.Format(_T("%s%3d %4.1f"), sign < 0 ? _T("-") : _T(""), feet, inches);
         }
         else
         {
            strBuffer.Format(_T("%s%3d %2.0f"), sign < 0 ? _T("-") : _T(""), feet, inches);
         }

         CString strLength;
         strLength.Format(_T("%-s"), strBuffer);
         return strLength;
      }
   }
}

std::pair<bool, Float64> Formatter::IsValidLength(const CString& strValue)
{
   WBFL::System::Tokenizer tokenizer(_T(" "));
   tokenizer.push_back(strValue);
   auto size = tokenizer.size();
   if (size == 1)
   {
      // there is only one token so strValue must be decimal length
      Float64 value = 0.0;
      bool bValid = WBFL::System::Tokenizer::ParseDouble(tokenizer[0].c_str(), &value);
      return { bValid, value };
   }
   else if (size == 2)
   {
      // there are 2 tokens... feet inch
      Float64 ft, in;
      if (!WBFL::System::Tokenizer::ParseDouble(tokenizer[0].c_str(), &ft))
      {
         return { false, 0.0 };
      }

      if (!WBFL::System::Tokenizer::ParseDouble(tokenizer[1].c_str(), &in))
      {
         return { false, 0.0 };
      }

      return { true, ft + in / 12.0 };
   }
   return { false, 0.0 };
}

std::pair<bool, Float64> Formatter::ParseLength(const CString& strValue)
{
   auto [bValid, value] = IsValidLength(strValue);
   if (!bValid)
   {
      return { false, 0.0 };
   }

   WBFL::Units::LengthData* pLength;
   if (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI)
   {
      pLength = &gs_LengthUnit[0];
   }
   else
   {
      pLength = &gs_LengthUnit[1];
   }

   return { true, WBFL::Units::ConvertToSysUnits(value, pLength->UnitOfMeasure) };
}

CString Formatter::FormatStatusValue(const StatusValue& var)
{
   CString strValue;
   if (std::holds_alternative<double>(var))
   {
      strValue.Format(_T("%s"), (LPCTSTR)FormatLength(std::get<double>(var)));
   }
   else if (std::holds_alternative<long>(var))
   {
      // matches the original VariantChangeType(..., VT_BSTR) behavior for
      // a numeric value -- stringify as decimal text
      strValue.Format(_T("%d"), std::get<long>(var));
   }
   else if (std::holds_alternative<std::_tstring>(var))
   {
      strValue = CString(std::get<std::_tstring>(var).c_str());
   }
   return strValue;
}

CString Formatter::FormatStatusMessage(const CStatusMessage& statusMessage)
{
   CString strMsg(statusMessage.GetText().c_str());

   CString strVal1 = FormatStatusValue(statusMessage.GetVal1());
   CString strVal2 = FormatStatusValue(statusMessage.GetVal2());

   strMsg.Replace(_T("%1"), strVal1);
   strMsg.Replace(_T("%2"), strVal2);

   return strMsg;
}

std::pair<Int32, Float64> Formatter::USLength(Float64 length)
{
   length = WBFL::Units::ConvertFromSysUnits(length, gs_LengthUnit[1].UnitOfMeasure);
   int sign = BinarySign(length);
   length = fabs(length);
   long feet = (long)floor(length);
   double inches = (length - feet) * 12;
   if (IsEqual(inches, 12.0))
   {
      // don't want 6'-12"... make it 7'-0"
      feet++;
      inches = 0;
   }

   return { sign * feet, inches };
}
