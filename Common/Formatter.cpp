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

#include "stdafx.h"
#include "Formatter.h"
#include <MfcTools\Format.h>
#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

WBFL::Units::MassData Formatter::gs_MassUnit(WBFL::Units::Measure::Kilogram, 0.001, 9, 0);
WBFL::Units::ForceData Formatter::gs_WeightUnit(WBFL::Units::Measure::Pound, 0.001, 9, 0);
std::array<WBFL::Units::LengthData, 2> Formatter::gs_LengthUnit{ WBFL::Units::LengthData(WBFL::Units::Measure::Meter,0.001,9,0),WBFL::Units::LengthData(WBFL::Units::Measure::Feet,0.001,9,0) };
CComPtr<IAnnotatedDisplayUnitFormatter> Formatter::g_formatter;

BOOL Formatter::Init()
{
   if (g_formatter == nullptr)
   {
      HRESULT hr = g_formatter.CoCreateInstance(CLSID_AnnotatedDisplayUnitFormatter);
      if (FAILED(hr))
      {
         CString strMessage;
         strMessage.Format(_T("Failed to initialize unit system (%0#x)"), hr);
         AfxMessageBox(strMessage);
         return FALSE;
      }
      g_formatter->put_Annotation(CComBSTR("'-,\""));
      g_formatter->put_Multiplier(12.0);
      g_formatter->put_OffsetDigits(0);
      g_formatter->FormatSpecifiers(7, 1, tjRight, nftFixed, 0.0001);
   }
   return TRUE;
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
         USES_CONVERSION;
         CComBSTR bstr;
         g_formatter->Format(WBFL::Units::ConvertFromSysUnits(length, gs_LengthUnit[1].UnitOfMeasure), CComBSTR(""), &bstr);
         return OLE2T(bstr);
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
         USES_CONVERSION;
         CComBSTR bstr;
         g_formatter->Format(WBFL::Units::ConvertFromSysUnits(length, gs_LengthUnit[1].UnitOfMeasure), CComBSTR(""), &bstr);
         return OLE2T(bstr);
      }
      else
      {
         // convert from system units
         Int32 feet;
         Float64 inches;
         USLength(length, &feet, &inches);
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

bool Formatter::IsValidLength(const CString& strValue, Float64* pValue)
{
   WBFL::System::Tokenizer tokenizer(_T(" "));
   tokenizer.push_back(strValue);
   auto size = tokenizer.size();
   if (size == 1)
   {
      // there is only one token so strValue must be decimal length
      return WBFL::System::Tokenizer::ParseDouble(tokenizer[0].c_str(), pValue);
   }
   else if (size == 2)
   {
      // there are 2 tokens... feet inch
      Float64 ft, in;
      if (!WBFL::System::Tokenizer::ParseDouble(tokenizer[0].c_str(), &ft))
      {
         return false;
      }

      if (!WBFL::System::Tokenizer::ParseDouble(tokenizer[1].c_str(), &in))
      {
         return false;
      }

      *pValue = ft + in / 12.0;
      return true;
   }
   return false;
}

bool Formatter::ParseLength(const CString& strValue, Float64* pValue)
{
   Float64 value;
   if (!IsValidLength(strValue, &value))
   {
      return false;
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

   *pValue = WBFL::Units::ConvertToSysUnits(value, pLength->UnitOfMeasure);
   return true;
}

CString Formatter::FormatStatusValue(CComVariant& var)
{
   CString strValue;
   if (var.vt == VT_R8)
   {
      strValue.Format(_T("%s"), FormatLength(var.dblVal));
   }
   else
   {
      USES_CONVERSION;
      var.ChangeType(VT_BSTR);
      strValue.Format(_T("%s"), OLE2T(var.bstrVal));
   }
   return strValue;
}

CString Formatter::FormatStatusMessage(IStatusMessage* pStatusMessage)
{
   CComBSTR bstrText;
   pStatusMessage->get_Text(&bstrText);
   CString strMsg(bstrText);

   CComVariant val1, val2;
   pStatusMessage->get_Val1(&val1);
   pStatusMessage->get_Val2(&val2);
   CString strVal1 = FormatStatusValue(val1);
   CString strVal2 = FormatStatusValue(val2);

   strMsg.Replace(_T("%1"), strVal1);
   strMsg.Replace(_T("%2"), strVal2);

   return strMsg;
}

void Formatter::USLength(Float64 length, Int32* pFt, Float64* pInch)
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

   *pFt = sign*feet;
   *pInch = inches;
}
