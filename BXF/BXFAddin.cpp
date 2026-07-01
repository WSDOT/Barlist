///////////////////////////////////////////////////////////////////////
// BXF - Barlist Exchange File
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

// BXFAddin.cpp : Implementation of CBXFAddin
#include "stdafx.h"

#include "BXFAddin.h"
#include "CLSID.h"

#include <EAF\EAFApp.h>

#include "..\Common\Formatter.h"
#include <EAF\ComponentModule.h>

#include <Bars\Barlist.h>
#include <Bars\Group.h>
#include <Bars\GroupCollection.h>
#include <Bars\BarRecordCollection.h>
#include <Bars\BarRecord.h>
#include <Bars\BendImpl.h>
#include <Bars\StatusMessageCollection.h>

CBXFApp theApp;
WBFL::EAF::ComponentModule _Module;

inline bool IsA706(MaterialType material) { return (int)MaterialType::A706_Grade60 <= (int)material && (int)material <= (int)MaterialType::A706_Grade80; }
inline bool IsMMFX(MaterialType material) { return (int)MaterialType::A1035_Grade100 <= (int)material && (int)material <= (int)MaterialType::A1035_Grade120; }
inline bool IsGalvanized(MaterialType material) { return (int)MaterialType::A767_A1094_Grade60 <= (int)material && (int)material <= (int)MaterialType::A767_A1094_Grade100; }
inline bool IsStainless(MaterialType material) { return (int)MaterialType::A955_Grade60 <= (int)material && (int)material <= (int)MaterialType::A955_Grade80; }
inline bool IsGFRP(MaterialType materialType) { return (int)MaterialType::D7957 == (int)materialType; }

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
   WBFL::EAF::UnitMode unitMode = EAFGetApp()->GetUnitsMode();
   CString strGrade;
   switch (material)
   {
   case MaterialType::A706_Grade60:
   case MaterialType::A767_A1094_Grade60:
   case MaterialType::A955_Grade60:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("  ") : _T("41"));
      break;

   case MaterialType::A955_Grade75:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("75") : _T("52"));
      break;

   case MaterialType::A706_Grade80:
   case MaterialType::A767_A1094_Grade80:
   case MaterialType::A955_Grade80:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("80") : _T("55"));
      break;

   case MaterialType::A1035_Grade100:
   case MaterialType::A767_A1094_Grade100:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("1X") : _T("69"));
      break;

   case MaterialType::A1035_Grade120:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("12") : _T("83"));
      break;

   case MaterialType::D7957:
      strGrade = _T("  ");
      break;

   default:
      ATLASSERT(false); // is there a new material
      strGrade = _T("??");
   }

   return strGrade;
}


EAF_BEGIN_OBJECT_MAP(ObjectMap)
   EAF_OBJECT_ENTRY(CLSID_BXFAddin, CBXFAddin)
EAF_END_OBJECT_MAP()

BOOL CBXFApp::InitInstance()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   _Module.Init(ObjectMap);

   return CWinApp::InitInstance();
}

int CBXFApp::ExitInstance()
{
   _Module.Term();
   return CWinApp::ExitInstance();
}

TCHAR GetUse(UseType use)
{
   TCHAR c;
   switch (use)
   {
   case UseType::utLongitudinal: c = ' '; break;
   case UseType::utSeismic:      c = 'S'; break;
   case UseType::utTransverse:   c = 'T'; break;
   }
   return c;
}

inline TCHAR GetFlag(bool bFlag, TCHAR c)
{
   return bFlag ? c : ' ';
}

/////////////////////////////////////////////////////////////////////////////
// CBXFAddin

void CBXFAddin::Init(CEAFDocument* pDoc)
{
}

void CBXFAddin::Terminate()
{
}

void CBXFAddin::Go(CBarlist& barlist)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   CString strFile;
   CFileDialog dlg(FALSE, _T("bar"), nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Barlist Exchange File (*.bxf)|*.bxf|"));
   if (dlg.DoModal() == IDOK)
   {
      POSITION pos = dlg.GetStartPosition();
      while (pos)
      {
         strFile = dlg.GetNextPathName(pos);
      }

      CreateBarlistExchangeFile(strFile, barlist);
      CString strMsg;
      strMsg.Format(_T("Barlist Exchange File Saved\r\n%s"),strFile);
      AfxMessageBox(strMsg,MB_ICONINFORMATION | MB_OK);
   }
   return;
}

CString CBXFAddin::GetMenuItem() const
{
   return CString("Create Barlist Exchange File (BXF)");
}

void CBXFAddin::CreateBarlistExchangeFile(const CString& strFile, CBarlist& barlist)
{
   CStdioFile file(strFile,CFile::modeCreate | CFile::modeWrite | CFile::typeText | CFile::shareDenyRead);

   CGroupCollection& groups = barlist.GetGroups();
   for (auto& group : groups)
   {
      CString strName(group->GetName().c_str());
      strName.MakeUpper();

      CString strGroup;
      strGroup.Format(_T("%4s %-28s\n"), _T(""), strName);
      file.WriteString(strGroup);

      ExchangeBarRecords(&file, *group);
   }
}

void CBXFAddin::ExchangeBarRecords(CStdioFile* pFile, CGroup& group)
{
   CBarRecordCollection& bars = group.GetBarRecords();
   for (auto& bar : bars)
   {
      ExchangeBarRecord(pFile, *bar);
   }
}

void CBXFAddin::ExchangeBarRecord(CStdioFile* pFile, CBarRecord& barRecord)
{
   CString strBarRecord;

   CString strMark(barRecord.GetMark().c_str());
   CString strLocation(barRecord.GetLocation().c_str());

   CString strSize(barRecord.GetSize().c_str());
   strSize.Remove(_T('#'));

   long nReqd = barRecord.GetNumReqd();
   long bendType = barRecord.GetBendType();
   UseType use = barRecord.GetUse();
   bool bSubstructure = barRecord.GetSubstructure();
   bool bEpoxy = barRecord.GetEpoxy();
   MaterialType material = barRecord.GetMaterial();
   bool bVaries = barRecord.GetVaries();

   strBarRecord.Format(_T("%4s %-28s %2s "), (LPCTSTR)strMark, (LPCTSTR)strLocation, (LPCTSTR)strSize);
   CString strNumReqd;
   strNumReqd.Format(_T("%4d "), nReqd);

   CString strRestOfBarRecord;
   strRestOfBarRecord.Format(_T("%2d %c %c %4s %c "), bendType, GetUse(use), GetFlag(bSubstructure, 'S'), GetMaterial(material, bEpoxy), GetFlag(bVaries, 'V'));
   strBarRecord += strNumReqd + strRestOfBarRecord;

   if (bVaries)
   {
      CString strEach;
      long nEach = barRecord.GetNumEach();
      strEach.Format(_T("%2d "), nEach);
      strBarRecord += strEach;
   }
   else
   {
      strBarRecord += _T("   ");
   }

   auto primaryBend = barRecord.GetPrimaryBend();
   strBarRecord += ReportBend(primaryBend.get(), false);

   Float64 mass = barRecord.GetMass();
   strBarRecord += (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI ? _T("   ") : _T("   ")) + Formatter::FormatMass(mass, false);
   strBarRecord += _T("\n");

   pFile->WriteString(strBarRecord);

   std::shared_ptr<CBend> variesBend;
   if (bVaries)
   {
      variesBend = barRecord.GetVariesBend();
      CString strVariesBend = ReportBend(variesBend.get(), true);
      strVariesBend += _T("\n");
      pFile->WriteString(strVariesBend);
   }

   ReportErrors(pFile, primaryBend.get());
   ReportErrors(pFile, variesBend.get());
}

CString CBXFAddin::ReportBend(CBend* pBend, bool bVaries)
{
   CString strBend;

   if (bVaries)
   {
      strBend.Format(_T("%59s"), _T(" "));
   }

   long bendType = pBend->GetBendType();

   Float64 u = pBend->GetU();
   Float64 w = pBend->GetW();
   Float64 x = pBend->GetX();
   Float64 y = pBend->GetY();
   Float64 z = pBend->GetZ();
   Float64 t1 = pBend->GetT1();
   Float64 t2 = pBend->GetT2();

   CString strLengthSpace(EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI ? _T("  ") : _T(" "));
   CString strSkipLength(EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI ? _T("       ") : _T("        "));

   if (90 <= bendType && bendType <= 99)
   {
      // leave U field blank for Type 90-99 bends
      strBend += strLengthSpace + strSkipLength;
   }
   else
   {
      strBend += Formatter::FormatLength(u, true, false);
   }


   bool bSupported = pBend->SupportsDimension(DimensionType::dimW);
   strBend += bSupported ? (strLengthSpace + Formatter::FormatLength(w, true, false)) : strLengthSpace + strSkipLength;

   if (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI)
   {
      strBend += _T("  ");
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimX);
   strBend += bSupported ? Formatter::FormatLength(x, true, false) : strSkipLength;

   if (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI)
   {
      strBend += _T(" ");
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimY);
   strBend += bSupported ? Formatter::FormatLength(y, true, false) : strSkipLength;

   if (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI)
   {
      strBend += _T(" ");
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimZ);
   strBend += bSupported ? Formatter::FormatLength(z, true, false) : strSkipLength;

   CEAFApp* pApp = EAFGetApp();
   const auto* pDisplayUnits = pApp->GetDisplayUnits();

   bSupported = pBend->SupportsDimension(DimensionType::dimT1);
   if (bSupported)
   {
      CString strT1;
      strT1.Format(_T(" %3.0f"), WBFL::Units::ConvertFromSysUnits(t1, pDisplayUnits->Angle.UnitOfMeasure));
      strBend += strT1;
   }
   else
   {
      strBend += _T("    ");
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimT2);
   if (bSupported)
   {
      CString strT2;
      strT2.Format(_T(" %3.0f"), WBFL::Units::ConvertFromSysUnits(t2, pDisplayUnits->Angle.UnitOfMeasure));
      strBend += strT2;
   }
   else
   {
      strBend += _T("    ");
   }

   Float64 length = pBend->GetLength();
   strBend += (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI ? _T(" ") : _T("  ")) + Formatter::FormatLength(length, false, false);

   return strBend;
}

void CBXFAddin::ReportErrors(CStdioFile* pFile, CBend* pBend)
{
   if (pBend)
   {
      for (const auto& statusMessage : pBend->GetStatusMessages())
      {
         CString strStatusMessage = Formatter::FormatStatusMessage(statusMessage);
         strStatusMessage += _T("\n");

         if (strStatusMessage.Left(5).CompareNoCase(_T("Error")) == 0)
         {
            // only report errors... warnings don't go into the barlist drawing
            pFile->WriteString(strStatusMessage);
         }
      }
   }
}

CString CBXFAddin::GetMaterial(MaterialType material, bool bEpoxy)
{
   CString strMaterial;
   if (bEpoxy)
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
