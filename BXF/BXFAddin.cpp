///////////////////////////////////////////////////////////////////////
// BXF - Barlist Exchange File
// Copyright © 1999-2020  Washington State Department of Transportation
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

#include "..\Common\Formatter.h"

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

   if (!Formatter::Init())
   {
      return FALSE;
   }

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
   strBarRecord += (EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? _T("   ") : _T("   ")) + Formatter::FormatMass(mass, false);
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
      strBend += Formatter::FormatLength(u, true, false);
   }


   VARIANT_BOOL vbSupported;
   pBend->get_SupportsDimension(dimW, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (strLengthSpace + Formatter::FormatLength(w, true, false)) : strLengthSpace + strSkipLength;

   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      strBend += _T("  ");
   }

   pBend->get_SupportsDimension(dimX, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? Formatter::FormatLength(x, true, false) : strSkipLength;

   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      strBend += _T(" ");
   }

   pBend->get_SupportsDimension(dimY, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? Formatter::FormatLength(y, true, false) : strSkipLength;

   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      strBend += _T(" ");
   }

   pBend->get_SupportsDimension(dimZ, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? Formatter::FormatLength(z, true, false) : strSkipLength;

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
   strBend += (EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? _T(" ") : _T("  ")) + Formatter::FormatLength(length, false, false);

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
