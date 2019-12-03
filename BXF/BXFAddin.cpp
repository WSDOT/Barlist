///////////////////////////////////////////////////////////////////////
// BXF - Barlist Exchange File
// Copyright © 2009-2019  Washington State Department of Transportation
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

// BXFAddin.cpp : Implementation of CBXFAddin
#include "stdafx.h"

#include "BXF.h"
#include "BXFAddin.h"

#include <EAF\EAFApp.h>
#include <WBFLUnitServer.h>

CBXFApp theApp;
CComModule _Module;
CComPtr<IAnnotatedDisplayUnitFormatter> g_formatter;

inline bool IsA706(MaterialType material) { return (int)A706_Grade60 <= (int)material && (int)material <= (int)A706_Grade80; }
inline bool IsMMFX(MaterialType material) { return (int)A1035_Grade100 <= (int)material && (int)material <= (int)A1035_Grade120; }
inline bool IsGalvanized(MaterialType material) { return (int)A767_A1094_Grade60 <= (int)material && (int)material <= (int)A767_A1094_Grade100; }
inline bool IsStainless(MaterialType material) { return (int)A955_Grade60 <= (int)material && (int)material <= (int)A955_Grade80; }
inline bool IsGFRP(MaterialType materialType) { return (int)D7957 == (int)materialType; }

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


BEGIN_OBJECT_MAP(ObjectMap)
   OBJECT_ENTRY(CLSID_BXFAddin, CBXFAddin)
END_OBJECT_MAP()

BOOL CBXFApp::InitInstance()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   _Module.Init(ObjectMap, m_hInstance);

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
      g_formatter->put_Annotation(_T("'-,\""));
      g_formatter->put_Multiplier(12.0);
      g_formatter->put_OffsetDigits(0);
      g_formatter->FormatSpecifiers(7, 1, tjRight, nftFixed, 0.0001);
   }

   return CWinApp::InitInstance();
}

int CBXFApp::ExitInstance()
{
   _Module.Term();
   return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////
#include <array>
#include <UnitMgt\IndirectMeasure.h>
#include <MfcTools\Format.h>

static std::array<unitmgtMassData, 2> gs_WeightUnit{ unitmgtMassData(unitMeasure::Kilogram,0.001,9,0),unitmgtMassData(unitMeasure::PoundMass,0.001,9,0) };
static std::array<unitmgtLengthData, 2> gs_LengthUnit{ unitmgtLengthData(unitMeasure::Meter,0.001,9,0),unitmgtLengthData(unitMeasure::Feet,0.001,9,0) };

CString FormatMass(Float64 mass, bool bUnits=true)
{
   CString strMass;
   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      if (bUnits)
      {
         strMass.Format(_T("%5.0f %s"), ConvertFromSysUnits(mass, gs_WeightUnit[0].UnitOfMeasure), gs_WeightUnit[0].UnitOfMeasure.UnitTag().c_str());
      }
      else
      {
         strMass.Format(_T("%5.0f"), ConvertFromSysUnits(mass, gs_WeightUnit[0].UnitOfMeasure));
      }
   }
   else
   {
      if (bUnits)
      {
         strMass.Format(_T("%5.0f %s"), ConvertFromSysUnits(mass, gs_WeightUnit[1].UnitOfMeasure), gs_WeightUnit[1].UnitOfMeasure.UnitTag().c_str());
      }
      else
      {
         strMass.Format(_T("%5.0f"), ConvertFromSysUnits(mass, gs_WeightUnit[1].UnitOfMeasure));
      }
   }
   return strMass;
}

CString FormatLength(Float64 length,bool bFractionInches=true,bool bUnits=true)
{
   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
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
         g_formatter->Format(::ConvertFromSysUnits(length, gs_LengthUnit[1].UnitOfMeasure), _T(""), &bstr);
         return OLE2T(bstr);
      }
      else
      {
         // convert from system units
         length = ::ConvertFromSysUnits(length, gs_LengthUnit[1].UnitOfMeasure);
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

bool IsValidLength(const CString& strValue, Float64* pValue)
{
   sysTokenizer tokenizer(_T(" "));
   tokenizer.push_back(strValue);
   auto size = tokenizer.size();
   if (size == 1)
   {
      // there is only one token so strValue must be decimal length
      return sysTokenizer::ParseDouble(tokenizer[0].c_str(), pValue);
   }
   else if (size == 2)
   {
      // there are 2 tokens... feet inch
      Float64 ft, in;
      if (!sysTokenizer::ParseDouble(tokenizer[0].c_str(), &ft))
      {
         return false;
      }

      if (!sysTokenizer::ParseDouble(tokenizer[1].c_str(), &in))
      {
         return false;
      }

      *pValue = ft + in / 12.0;
      return true;
   }
   return false;
}

bool ParseLength(const CString& strValue, Float64* pValue)
{
   Float64 value;
   if (!IsValidLength(strValue, &value))
   {
      return false;
   }

   unitmgtLengthData* pLength;
   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      pLength = &gs_LengthUnit[0];
   }
   else
   {
      pLength = &gs_LengthUnit[1];
   }

   *pValue = ::ConvertToSysUnits(value, pLength->UnitOfMeasure);
   return true;
}

CString FormatStatusValue(CComVariant& var)
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

CString FormatStatusMessage(IStatusMessage* pStatusMessage)
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

TCHAR GetUse(UseType use)
{
   TCHAR c;
   switch (use)
   {
   case utLongitudinal: c = ' '; break;
   case utSeismic:      c = 'S'; break;
   case utTransverse:   c = 'T'; break;
   }
   return c;
}

inline TCHAR GetFlag(VARIANT_BOOL vbFlag, TCHAR c)
{
   return vbFlag == VARIANT_TRUE ? c : ' ';
}

/////////////////////////////////////////////////////////////////////////////
// CBXFAddin
STDMETHODIMP CBXFAddin::Go(IBarlist* pBarlist)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (pBarlist == NULL)
   {
      ::MessageBox(0, _T("An invalid barlist was provided."), _T(""), MB_OK);
      return S_FALSE;
   }

   CString strFile;
   CFileDialog dlg(FALSE, _T("bar"), nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Barlist Exchange File (*.bxf)|*.bxf|"));
   if (dlg.DoModal() == IDOK)
   {
      POSITION pos = dlg.GetStartPosition();
      while (pos)
      {
         strFile = dlg.GetNextPathName(pos);
      }

      CreateBarlistExchangeFile(strFile, pBarlist);
      CString strMsg;
      strMsg.Format(_T("Barlist Exchange File Saved\r\n%s"),strFile);
      AfxMessageBox(strMsg,MB_ICONINFORMATION | MB_OK);
   }
   return S_OK;
}

STDMETHODIMP CBXFAddin::get_MenuItem(BSTR *pVal)
{
   CComBSTR menuItem("Create Barlist Exchange File (BXF)");
   *pVal = menuItem.Copy();
   return S_OK;
}

void CBXFAddin::CreateBarlistExchangeFile(const CString& strFile, IBarlist* pBarlist)
{
   CStdioFile file(strFile,CFile::modeCreate | CFile::modeWrite | CFile::typeText | CFile::shareDenyRead);

   CComPtr<IGroupCollection> groups;
   pBarlist->get_Groups(&groups);
   long nGroups;
   groups->get_Count(&nGroups);
   for (long grpIdx = 0; grpIdx < nGroups; grpIdx++)
   {
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(grpIdx), &group);

      CComBSTR bstrName;
      group->get_Name(&bstrName);

      CString strName(bstrName);
      strName.MakeUpper();

      CString strGroup;
      strGroup.Format(_T("%4s %-28s\n"), _T(""), strName);
      file.WriteString(strGroup);

      ExchangeBarRecords(&file, group);
   }
}

void CBXFAddin::ExchangeBarRecords(CStdioFile* pFile, IGroup* pGroup)
{
   CComPtr<IBarRecordCollection> bars;
   pGroup->get_BarRecords(&bars);
   long nBars;
   bars->get_Count(&nBars);
   for (long barIdx = 0; barIdx < nBars; barIdx++)
   {
      CComPtr<IBarRecord> bar;
      bars->get_Item(CComVariant(barIdx), &bar);
      ExchangeBarRecord(pFile, bar);
   }
}

void CBXFAddin::ExchangeBarRecord(CStdioFile* pFile, IBarRecord* pBarRecord)
{
   USES_CONVERSION;

   CString strBarRecord;

   CComBSTR bstrMark;
   pBarRecord->get_Mark(&bstrMark);

   CComBSTR bstrLocation;
   pBarRecord->get_Location(&bstrLocation);

   CComBSTR bstrSize;
   pBarRecord->get_Size(&bstrSize);
   CString strSize(bstrSize);
   strSize.Remove(_T('#'));

   long nReqd;
   pBarRecord->get_NumReqd(&nReqd);

   long bendType;
   pBarRecord->get_BendType(&bendType);

   UseType use;
   pBarRecord->get_Use(&use);

   VARIANT_BOOL vbSubstructure;
   pBarRecord->get_Substructure(&vbSubstructure);

   VARIANT_BOOL vbEpoxy;
   pBarRecord->get_Epoxy(&vbEpoxy);

   MaterialType material;
   pBarRecord->get_Material(&material);

   VARIANT_BOOL vbVaries;
   pBarRecord->get_Varies(&vbVaries);

   strBarRecord.Format(_T("%4s %-28s %2s "), OLE2T(bstrMark), OLE2T(bstrLocation), strSize);
   CString strNumReqd;
   strNumReqd.Format(_T("%4d "), nReqd);

   CString strRestOfBarRecord;
   strRestOfBarRecord.Format(_T("%2d %c %c %4s %c "), bendType, GetUse(use), GetFlag(vbSubstructure, 'S'), GetMaterial(material, vbEpoxy), GetFlag(vbVaries, 'V'));
   strBarRecord += strNumReqd + strRestOfBarRecord;

   if (vbVaries == VARIANT_TRUE)
   {
      CString strEach;
      long nEach;
      pBarRecord->get_NumEach(&nEach);
      strEach.Format(_T("%2d "), nEach);
      strBarRecord += strEach;
   }
   else
   {
      strBarRecord += _T("   ");
   }

   CComPtr<IBend> primaryBend;
   pBarRecord->get_PrimaryBend(&primaryBend);
   strBarRecord += ReportBend(primaryBend, false);

   Float64 mass;
   pBarRecord->get_Mass(&mass);
   strBarRecord += (EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? _T("   ") : _T("   ")) + FormatMass(mass, false);
   strBarRecord += _T("\n");

   pFile->WriteString(strBarRecord);

   CComPtr<IBend> variesBend;
   if (vbVaries == VARIANT_TRUE)
   {
      pBarRecord->get_VariesBend(&variesBend);
      CString strVariesBend = ReportBend(variesBend, true);
      strVariesBend += _T("\n");
      pFile->WriteString(strVariesBend);
   }

   ReportErrors(pFile,primaryBend);
   ReportErrors(pFile,variesBend);
}

CString CBXFAddin::ReportBend(IBend* pBend, bool bVaries)
{
   CString strBend;

   if (bVaries)
   {
      strBend.Format(_T("%59s"), _T(" "));
   }

   long bendType;
   pBend->get_BendType(&bendType);

   Float64 u, w, x, y, z, t1, t2;
   pBend->get_U(&u);
   pBend->get_W(&w);
   pBend->get_X(&x);
   pBend->get_Y(&y);
   pBend->get_Z(&z);
   pBend->get_T1(&t1);
   pBend->get_T2(&t2);

   CString strLengthSpace(EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? _T("  ") : _T(" "));
   CString strSkipLength(EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? _T("       ") : _T("        "));

   if (90 <= bendType && bendType <= 99)
   {
      // leave U field blank for Type 90-99 bends
      strBend += strLengthSpace + strSkipLength;
   }
   else
   {
      strBend += FormatLength(u, true, false);
   }


   VARIANT_BOOL vbSupported;
   pBend->get_SupportsDimension(dimW, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (strLengthSpace + FormatLength(w, true, false)) : strLengthSpace + strSkipLength;

   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      strBend += _T("  ");
   }

   pBend->get_SupportsDimension(dimX, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? FormatLength(x, true, false) : strSkipLength;

   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      strBend += _T(" ");
   }

   pBend->get_SupportsDimension(dimY, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? FormatLength(y, true, false) : strSkipLength;

   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      strBend += _T(" ");
   }

   pBend->get_SupportsDimension(dimZ, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? FormatLength(z, true, false) : strSkipLength;

   CEAFApp* pApp = EAFGetApp();
   const auto* pDisplayUnits = pApp->GetDisplayUnits();

   pBend->get_SupportsDimension(dimT1, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      CString strT1;
      strT1.Format(_T(" %3.0f"), ::ConvertFromSysUnits(t1, pDisplayUnits->Angle.UnitOfMeasure));
      strBend += strT1;
   }
   else
   {
      strBend += _T("    ");
   }

   pBend->get_SupportsDimension(dimT2, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      CString strT2;
      strT2.Format(_T(" %3.0f"), ::ConvertFromSysUnits(t2, pDisplayUnits->Angle.UnitOfMeasure));
      strBend += strT2;
   }
   else
   {
      strBend += _T("    ");
   }

   Float64 length;
   pBend->get_Length(&length);
   strBend += (EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? _T(" ") : _T("  ")) + FormatLength(length, false, false);

   return strBend;
}

void CBXFAddin::ReportErrors(CStdioFile* pFile,IBend* pBend)
{
   if (pBend)
   {
      CComPtr<IStatusMessageCollection> statusMessages;
      pBend->get_StatusMessages(&statusMessages);
      long nMessages;
      statusMessages->get_Count(&nMessages);
      for (long i = 0; i < nMessages; i++)
      {
         CComPtr<IStatusMessage> statusMessage;
         statusMessages->get_Item(i, &statusMessage);

         CString strStatusMessage = FormatStatusMessage(statusMessage);
         strStatusMessage += _T("\n");

         if (strStatusMessage.Left(5).CompareNoCase(_T("Error")) == 0)
         {
            // only report errors... warnings don't go into the barlist drawing
            pFile->WriteString(strStatusMessage);
         }
      }
   }
}

CString CBXFAddin::GetMaterial(MaterialType material, VARIANT_BOOL vbEpoxy)
{
   CString strMaterial;
   if (vbEpoxy == VARIANT_TRUE)
   {
      strMaterial.Format(_T("%2s%2s"), GetMaterialDesignation(material), GetMaterialGrade(material));
      strMaterial.SetAt(0, _T('E'));
   }
   else
   {
      strMaterial.Format(_T("%2s%2s"), GetMaterialDesignation(material), GetMaterialGrade(material));
   }
   return strMaterial;
}
