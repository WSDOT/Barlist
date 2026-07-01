///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2026  Washington State Department of Transportation
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
#include "Report.h"
#include "BarlistDoc.h"
#include "Helpers.h"

#include <Bars\Barlist.h>
#include <Bars\Group.h>
#include <Bars\BarRecord.h>
#include <Bars\BendImpl.h>
#include <Bars\StatusMessageCollection.h>

#include <MfcTools\Text.h>
#include <MfcTools\VersionInfo.h>

CReport::CReport()
{
   m_nHeaderLines = 11;
}


CReport::~CReport()
{
}

CString CReport::GetReportHeader()
{
   CString strHeader;
   if (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI)
   {
      strHeader =
         CString("                                             +-------- T = Transverse, S = Seismic\r\n") +
         CString("                                             | +------ S = Substructure\r\n") +
         CString("                                             | |  +--- E = Epoxy Coated, G = Galvanized, SS = Strainless Steel,\r\n") +
         CString("                                             | |  |    CR = Corrosion Resistant, GF = Glass Fiber Reinf. Polymer,\r\n") +
         CString("                                             | |  |    41, 52, 55, 69, 83 = Grade 413, 517, 552, 690, 827 MPa\r\n") +
         CString("                                             | |  |   +-- V = Varies\r\n") +
         CString("                                             | |  |   |\r\n") +
         CString("Mark Description                  Si Num  Be | |  |   | #     U        W        X       Y       Z    T1  T2   LENGTH     MASS\r\n") +
         CString(" #                                ze Reqd nd T S Type V Ea  METER    METER    METER   METER   METER  DEG DEG   METER      KG\r\n") +
         CString("==== ============================ == ==== == = = ==== = == ===.===  ===.===  ===.=== ===.=== ===.=== === ===  ===.===   =====");
   }
   else
   {
      strHeader =
         CString("                                             +-------- T = Transverse, S = Seismic\r\n") +
         CString("                                             | +------ S = Substructure\r\n") +
         CString("                                             | |  +--- E = Epoxy Coated, G = Galvanized, SS = Strainless Steel,\r\n") +
         CString("                                             | |  |    CR = Corrosion Resistant, GF = Glass Fiber Reinf. Polymer,\r\n") +
         CString("                                             | |  |    75, 80, 1X, 12 = Grade 75, 80, 100, 120\r\n") +
         CString("                                             | |  |   +-- V = Varies\r\n") +
         CString("                                             | |  |   |\r\n") +
         CString("Mark Description                  Si Num  Be | |  |   | #     U        W         X        Y        Z    T1  T2   LENGTH    WEIGHT\r\n") +
         CString(" #                                ze Reqd nd T S Type V Ea  FT  IN   FT  IN   FT  IN   FT  IN   FT  IN  DEG DEG   FT  IN   LBS\r\n") +
         CString("==== ============================ == ==== == = = ==== = == === ==.= === ==.= === ==.= === ==.= === ==.= === ===  === ==.=  =====");
   }
   return strHeader;
}

const std::vector<CString>& CReport::GetReport()
{
   return m_vReportLines;
}

void CReport::BuildReport(CBarlist& barlist)
{
   // First capture some infomation for the report cover page
   m_strProject.Format(_T("Project: %s"), CString(barlist.GetProject().c_str()));
   m_strJobNumber.Format(_T("Job Number: %s"), CString(barlist.GetJobNumber().c_str()));
   m_strEngineer.Format(_T("Engineer: %s"), CString(barlist.GetEngineer().c_str()));
   m_strCompany.Format(_T("Company: %s"), CString(barlist.GetCompany().c_str()));
   m_strComments.Format(_T("Comments: %s"), CString(barlist.GetComments().c_str()));

   // Build the report
   m_vReportLines.clear();
   m_vReportLines.reserve(1000);

   CBarlistDoc* pDoc = (CBarlistDoc*)EAFGetDocument();
   if (pDoc->GetReportOptions() == CBarlistDoc::ReportOptions::REPORT_TOTAL_AND_GROUP_QUANTITIES)
   {
       ReportGroups(barlist);
       ReportQuantitiesByGroup(barlist);
       ReportQuantities(barlist);
   }
   else
   {
       ReportGroups(barlist);
       ReportQuantities(barlist);
   }
}

void CReport::ReportGroups(CBarlist& barlist)
{
   for (auto& group : barlist.GetGroups())
   {
      CString strName(group->GetName().c_str());
      strName.MakeUpper();
      CString strGroup;
      strGroup.Format(_T("%4s %-28s\n"), _T(""), strName);
      m_vReportLines.push_back(strGroup);

      ReportBarRecords(*group);

      m_vReportLines.push_back(_T("\n"));
   }
}

void CReport::ReportBarRecords(CGroup& group)
{
   for (auto& bar : group.GetBarRecords())
   {
      ReportBarRecord(*bar);
   }
}

void CReport::ReportBarRecord(CBarRecord& barRecord)
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

   strBarRecord.Format(_T("%4s %-28s %2s %4d %2d %c %c %4s %c "), (LPCTSTR)strMark, (LPCTSTR)strLocation, (LPCTSTR)strSize, nReqd, bendType, GetUse(use), GetFlag(bSubstructure, 'S'), (LPCTSTR)GetMaterial(material, bEpoxy), GetFlag(bVaries, 'V'));

   if (bVaries)
   {
      long nEach = barRecord.GetNumEach();
      CString strEach;
      strEach.Format(_T("%2d "), nEach);
      strBarRecord += strEach;
   }
   else
   {
      strBarRecord += _T("   ");
   }

   auto primaryBend = barRecord.GetPrimaryBend();
   strBarRecord += ReportBend(*primaryBend, false);

   Float64 mass = barRecord.GetMass();
   strBarRecord += (EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI ? _T("   ") : _T("  ")) + Formatter::FormatMass(mass, false);
   strBarRecord += _T("\n");

   m_vReportLines.push_back(strBarRecord);

   auto variesBend = barRecord.GetVariesBend();
   if (bVaries)
   {
      CString strVariesBend = ReportBend(*variesBend, true);
      strVariesBend += _T("\n");
      m_vReportLines.push_back(strVariesBend);
   }

   ReportErrors(primaryBend.get());
   ReportErrors(variesBend.get());
}

CString CReport::ReportBend(CBend& bend, bool bVaries)
{
   CString strBend;

   if (bVaries)
   {
      strBend.Format(_T("%59s"), _T(" "));
   }

   Float64 u = bend.GetU();
   Float64 w = bend.GetW();
   Float64 x = bend.GetX();
   Float64 y = bend.GetY();
   Float64 z = bend.GetZ();
   Float64 t1 = bend.GetT1();
   Float64 t2 = bend.GetT2();

   strBend += Formatter::FormatLength(u, false);

   CString strLengthSpace(EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI ? _T("  ") : _T(" "));
   CString strSkipLength(EAFGetApp()->GetUnitsMode() == WBFL::EAF::UnitMode::SI ? _T("       ") : _T("        "));

   strBend += bend.SupportsDimension(DimensionType::dimW) ? (strLengthSpace + Formatter::FormatLength(w, false)) : strLengthSpace + strSkipLength;

   strBend += bend.SupportsDimension(DimensionType::dimX) ? (strLengthSpace + Formatter::FormatLength(x, false)) : strLengthSpace + strSkipLength;

   strBend += bend.SupportsDimension(DimensionType::dimY) ? (CString(_T(" ")) + Formatter::FormatLength(y, false)) : CString(_T(" ")) + strSkipLength;

   strBend += bend.SupportsDimension(DimensionType::dimZ) ? (CString(_T(" ")) + Formatter::FormatLength(z, false)) : CString(_T(" ")) + strSkipLength;

   CEAFApp* pApp = EAFGetApp();
   const auto* pDisplayUnits = pApp->GetDisplayUnits();

   if (bend.SupportsDimension(DimensionType::dimT1))
   {
      CString strT1;
      strT1.Format(_T(" %3.0f"), WBFL::Units::ConvertFromSysUnits(t1, pDisplayUnits->Angle.UnitOfMeasure));
      strBend += strT1;
   }
   else
   {
      strBend += _T("    ");
   }

   if (bend.SupportsDimension(DimensionType::dimT2))
   {
      CString strT2;
      strT2.Format(_T(" %3.0f"), WBFL::Units::ConvertFromSysUnits(t2, pDisplayUnits->Angle.UnitOfMeasure));
      strBend += strT2;
   }
   else
   {
      strBend += _T("    ");
   }

   Float64 length = bend.GetLength();
   strBend += _T("  ") + Formatter::FormatLength(length, false);

   return strBend;
}

void CReport::ReportErrors(CBend* pBend)
{
   if (pBend)
   {
      const CStatusMessageCollection& statusMessages = pBend->GetStatusMessages();
      for (const auto& statusMessage : statusMessages)
      {
         CString strStatusMessage = Formatter::FormatStatusMessage(statusMessage);
         strStatusMessage += _T("\n");
         m_vReportLines.push_back(strStatusMessage);
      }
   }
}

TCHAR CReport::GetUse(UseType use)
{
   TCHAR c = ' ';
   switch (use)
   {
   case UseType::utLongitudinal: c = ' '; break;
   case UseType::utSeismic:      c = 'S'; break;
   case UseType::utTransverse:   c = 'T'; break;
   }
   return c;
}

inline TCHAR CReport::GetFlag(bool bFlag, TCHAR c)
{
   return bFlag ? c : ' ';
}

CString CReport::GetMaterial(MaterialType material, bool bEpoxy)
{
   CString strMaterial;
   if (bEpoxy)
   {
      strMaterial.Format(_T("%2s%2s"), (LPCTSTR)GetMaterialDesignation(material), (LPCTSTR)GetMaterialGrade(material));
      strMaterial.SetAt(0, _T('E'));
   }
   else
   {
      strMaterial.Format(_T("%2s%2s"), (LPCTSTR)GetMaterialDesignation(material), (LPCTSTR)GetMaterialGrade(material));
   }
   return strMaterial;
}


void CReport::AddReportHeader()
{
    CString columnHeader;
    columnHeader.Format(_T("%-30s %24s %26s %23s %31s\n"),
        _T("Material"), _T("Substructure"), _T("Substructure w/Epoxy"),
        _T("Superstructure"), _T("Superstructure w/Epoxy"));
    m_vReportLines.push_back(columnHeader);
}



void CReport::AddMaterialDataToReport(MaterialType material, Float64 sub, Float64 subEpoxy, Float64 super, Float64 superEpoxy)
{
    CString strMaterial;
    strMaterial.Format(_T("%-30s "), GetMaterialSpecification(material));

    if (material == MaterialType::D7957)
    {
        strMaterial.AppendFormat(_T("%23s %25s %25s %25s "), (LPCTSTR)Formatter::FormatLength(sub), _T(""), (LPCTSTR)Formatter::FormatLength(super), _T(""));
    }
    else
    {
        strMaterial.AppendFormat(_T("%22s "), (LPCTSTR)Formatter::FormatMass(sub));

        if (CanBeEpoxyCoated(material))
        {
            strMaterial.AppendFormat(_T("%22s "), (LPCTSTR)Formatter::FormatMass(subEpoxy));
        }
        else
        {
            strMaterial.AppendFormat(_T("%22s "), _T(""));
        }

        strMaterial.AppendFormat(_T("%27s "), (LPCTSTR)Formatter::FormatMass(super));

        if (CanBeEpoxyCoated(material))
        {
            strMaterial.AppendFormat(_T("%25s "), (LPCTSTR)Formatter::FormatMass(superEpoxy));
        }
        else
        {
            strMaterial.AppendFormat(_T("%25s "), _T(""));
        }
    }
    m_vReportLines.push_back(strMaterial);
}



void CReport::ReportQuantitiesByGroup(CBarlist& barlist)
{
    for (auto& group : barlist.GetGroups())
    {
        CString strGroupName(group->GetName().c_str());

        CString strGroupHeading;
        strGroupHeading.Format(_T("\nGroup : %s\n"), (LPCTSTR)strGroupName);
        m_vReportLines.push_back(strGroupHeading);

        AddReportHeader();

        for (int j = 0; j < MATERIAL_COUNT; j++)
        {
            if (j != 0)
            {
                m_vReportLines.push_back(_T("\n"));
            }

            MaterialType material = static_cast<MaterialType>(j);

            Float64 subEpoxy = barlist.GetQuantityByGroup(group->GetName(), material, true /*epoxy*/, true /*substructure*/);
            Float64 sub = barlist.GetQuantityByGroup(group->GetName(), material, false/*epoxy*/, true /*substructure*/);
            Float64 superEpoxy = barlist.GetQuantityByGroup(group->GetName(), material, true /*epoxy*/, false/*substructure*/);
            Float64 super = barlist.GetQuantityByGroup(group->GetName(), material, false/*epoxy*/, false/*substructure*/);

            AddMaterialDataToReport(material, sub, subEpoxy, super, superEpoxy);
        }
        m_vReportLines.push_back(_T("\n"));
    }
}


void CReport::ReportQuantities(CBarlist& barlist)
{
    CString strSummaryHeading{ _T("\nSummary of Quantities:\n") };
    m_vReportLines.push_back(strSummaryHeading);

    AddReportHeader();

    for (int i = 0; i < MATERIAL_COUNT; i++)
    {
        if (i != 0)
        {
            m_vReportLines.push_back(_T("\n"));
        }

        MaterialType material = static_cast<MaterialType>(i);

        Float64 subEpoxy = barlist.GetQuantity(material, true /*epoxy*/, true /*substructure*/);
        Float64 sub = barlist.GetQuantity(material, false/*epoxy*/, true /*substructure*/);
        Float64 superEpoxy = barlist.GetQuantity(material, true /*epoxy*/, false/*substructure*/);
        Float64 super = barlist.GetQuantity(material, false/*epoxy*/, false/*substructure*/);

        AddMaterialDataToReport(material, sub, subEpoxy, super, superEpoxy);
    }
}

void CReport::PrepareForPrinting(CDC* pDC,CPrintInfo* pInfo)
{
   m_Font.CreatePointFont(90, _T("Courier New"), pDC);
   m_pOldFont = pDC->SelectObject(&m_Font);
   CSize szText = pDC->GetTextExtent(_T("M"));
   m_hLine = szText.cy;

   int sx = pDC->GetDeviceCaps(LOGPIXELSX); // pixels per inch (horizontal)
   int sy = pDC->GetDeviceCaps(LOGPIXELSY); // pixels per inch (vertical)

   m_Border = pInfo->m_rectDraw;
   m_Border.left += 5 * sx / 10;   // 0.5" left margin
   m_Border.right -= 5 * sx / 10;  // 0.5" right margin
   m_Border.top += sy;             // 1" top margin
   m_Border.bottom -= 5 * sy / 10; // 0.5" bottom margin

   m_Rect = m_Border;
   m_Rect.DeflateRect(10, 10);

   m_nLinesPerPage = m_Rect.Height() / m_hLine; // number of total lines per page
   m_nLinesPerPage -= m_nHeaderLines; // subtracting off the number of lines in the header leaves us with number of report lines per page
   WORD nPages = (WORD)(m_vReportLines.size() / m_nLinesPerPage) + 1;
   nPages++; // add one page for the cover page
   pInfo->m_pPD->m_pd.nMaxPage = nPages;
   pInfo->m_pPD->m_pd.nToPage = nPages;
}

void CReport::EndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
   pDC->SelectObject(m_pOldFont);
   m_Font.DeleteObject();
}

void CReport::Print(CDC* pDC, CPrintInfo* pInfo)
{
   ATLASSERT(m_Font.GetSafeHandle()); // did you forget to call PrepareForPrinting

   PageHeader(pDC, pInfo);

   if (pInfo->m_nCurPage == 1)
   {
      PrintCoverPage(pDC, pInfo);
   }
   else
   {
      CString strHeader = GetReportHeader();
      strHeader.Replace(_T("\r\n"), _T("\n"));
      MultiLineTextOut(pDC, m_Rect.left, m_Rect.top, strHeader);

      long pageLine = m_nHeaderLines;
      long startLine = m_nLinesPerPage*(pInfo->m_nCurPage - 2);
      long endLine = Min(startLine + m_nLinesPerPage - 1,(long)(m_vReportLines.size()-1));
      for (long line = startLine; line <= endLine; line++, pageLine++)
      {
         pDC->TextOut(m_Rect.left, m_Rect.top + m_hLine*pageLine, m_vReportLines[line]);
      }
   }
}

void CReport::PageHeader(CDC* pDC, CPrintInfo* pInfo)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   pDC->Rectangle(m_Border);

   CString strExe = AfxGetApp()->m_pszExeName;
   strExe += _T(".dll");

   CVersionInfo verInfo;
   verInfo.Load(strExe);

   CString strCopyright = verInfo.GetLegalCopyright();
   CString strVersion;
   strVersion.Format(_T("Barlist Version %s"),verInfo.GetFileVersionAsString());

   CEAFDocument* pDoc = EAFGetDocument();
   CString strDocument = pDoc->GetPathName();

   long y = m_Border.top - 35 * m_hLine / 10;
   pDC->TextOut(m_Border.left, y, strVersion);
   pDC->TextOut(m_Border.left, y + m_hLine, strDocument);
   pDC->TextOut(m_Border.left, y + 2 * m_hLine, m_strProject);

   CSize size;
   CString strPage;
   strPage.Format(_T("Page %d of %d"), pInfo->m_nCurPage, pInfo->GetMaxPage());
   size = pDC->GetTextExtent(strPage);
   pDC->TextOut(m_Border.right - size.cx, y, strPage);

   CTime now(CTime::GetCurrentTime());
   CString strTime = now.Format(_T("%D %I:%M:%S %p"));
   size = pDC->GetTextExtent(strTime);
   pDC->TextOut(m_Border.right - size.cx, y + m_hLine, strTime);

   // footer
   pDC->TextOut(m_Border.left, m_Border.bottom + 5*m_hLine/10, strCopyright);
}

void CReport::PrintCoverPage(CDC* pDC, CPrintInfo* pInfo)
{
   pDC->TextOut(m_Rect.left, m_Rect.top, m_strProject);
   pDC->TextOut(m_Rect.left, m_Rect.top + m_hLine, m_strJobNumber);
   pDC->TextOut(m_Rect.left, m_Rect.top + 2 * m_hLine, m_strEngineer);
   pDC->TextOut(m_Rect.left, m_Rect.top + 3 * m_hLine, m_strCompany);
   pDC->TextOut(m_Rect.left, m_Rect.top + 4 * m_hLine, m_strComments);
}
