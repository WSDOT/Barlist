///////////////////////////////////////////////////////////////////////
// Barlist
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

#include "stdafx.h"
#include "Report.h"
#include "BarlistDoc.h"

#include <MfcTools\Text.h>
#include <MfcTools\VersionInfo.h>

CReport::CReport()
{
   m_nHeaderLines = 9;
}


CReport::~CReport()
{
}

CString CReport::GetReportHeader()
{
   CString strHeader;
   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      strHeader =
         CString("                                             +---------- T = Transverse, S = Seismic\r\n") +
         CString("                                             | +-------- L = Lump sum\r\n") +
         CString("                                             | | +------ S = Substructure\r\n") +
         CString("                                             | | | +---- E = Epoxy\r\n") +
         CString("                                             | | | | +-- V = Varies\r\n") +
         CString("                                             | | | | |\r\n") +
         CString("Mark Description                  Si Num  Be | | | | | #     U        W        X       Y       Z    T1  T2   LENGTH     MASS\r\n") +
         CString(" #                                ze Reqd nd T L S E V Ea  METER    METER    METER   METER   METER  DEG DEG   METER      KG\r\n") +
         CString("==== ============================ == ==== == = = = = = == ===.===  ===.===  ===.=== ===.=== ===.=== === ===  ===.===   =====");
   }
   else
   {
      strHeader =
         CString("                                             +---------- T = Transverse, S = Seismic\r\n") +
         CString("                                             | +-------- L = Lump sum\r\n") +
         CString("                                             | | +------ S = Substructure\r\n") +
         CString("                                             | | | +---- E = Epoxy\r\n") +
         CString("                                             | | | | +-- V = Varies\r\n") +
         CString("                                             | | | | |\r\n") +
         CString("Mark Description                  Si Num  Be | | | | | #     U        W         X        Y        Z    T1  T2   LENGTH    WEIGHT\r\n") +
         CString(" #                                ze Reqd nd T L S E V Ea  FT  IN   FT  IN   FT  IN   FT  IN   FT  IN  DEG DEG   FT  IN   LBS\r\n") +
         CString("==== ============================ == ==== == = = = = = == === ==.= === ==.= === ==.= === ==.= === ==.= === ===  === ==.=  =====");
   }
   return strHeader;
}

const std::vector<CString>& CReport::GetReport()
{
   return m_vReportLines;
}

void CReport::BuildReport(IBarlist* pBarlist)
{
   // First capture some infomation for the report cover page
   USES_CONVERSION;
   CComBSTR bstrProject;
   pBarlist->get_Project(&bstrProject);
   m_strProject.Format(_T("Project: %s"), OLE2T(bstrProject));

   CComBSTR bstrJobNumber;
   pBarlist->get_JobNumber(&bstrJobNumber);
   m_strJobNumber.Format(_T("Job Number: %s"), OLE2T(bstrJobNumber));

   CComBSTR bstrEngineer;
   pBarlist->get_Engineer(&bstrEngineer);
   m_strEngineer.Format(_T("Engineer: %s"), OLE2T(bstrEngineer));

   CComBSTR bstrCompany;
   pBarlist->get_Company(&bstrCompany);
   m_strCompany.Format(_T("Company: %s"), OLE2T(bstrCompany));

   CComBSTR bstrComments;
   pBarlist->get_Comments(&bstrComments);
   m_strComments.Format(_T("Comments: %s"),OLE2T(bstrComments));

   // Build the report
   m_vReportLines.clear();
   m_vReportLines.reserve(1000);

   ReportGroups(pBarlist);
   m_vReportLines.push_back(_T("\n"));
   ReportQNI(pBarlist);
   m_vReportLines.push_back(_T("\n"));
   ReportSummary(pBarlist);
}

void CReport::ReportGroups(IBarlist* pBarlist)
{
   USES_CONVERSION;
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
      m_vReportLines.push_back(strGroup);
      
      ReportBarRecords(group);
      
      m_vReportLines.push_back(_T("\n"));
   }
}

void CReport::ReportBarRecords(IGroup* pGroup)
{
   CComPtr<IBarRecordCollection> bars;
   pGroup->get_BarRecords(&bars);
   long nBars;
   bars->get_Count(&nBars);
   for (long barIdx = 0; barIdx < nBars; barIdx++)
   {
      CComPtr<IBarRecord> bar;
      bars->get_Item(CComVariant(barIdx), &bar);
      ReportBarRecord(bar);
   }
}

void CReport::ReportBarRecord(IBarRecord* pBarRecord)
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

   VARIANT_BOOL vbLumpSum;
   pBarRecord->get_LumpSum(&vbLumpSum);

   VARIANT_BOOL vbSubstructure;
   pBarRecord->get_Substructure(&vbSubstructure);

   VARIANT_BOOL vbEpoxy;
   pBarRecord->get_Epoxy(&vbEpoxy);

   VARIANT_BOOL vbVaries;
   pBarRecord->get_Varies(&vbVaries);

   strBarRecord.Format(_T("%4s %-28s %2s %4d %2d %c %c %c %c %c "), OLE2T(bstrMark), OLE2T(bstrLocation), strSize, nReqd, bendType, GetUse(use), GetFlag(vbLumpSum, 'L'), GetFlag(vbSubstructure, 'S'), GetFlag(vbEpoxy, 'E'), GetFlag(vbVaries, 'V'));

   if (vbVaries == VARIANT_TRUE)
   {
      long nEach;
      pBarRecord->get_NumEach(&nEach);
      CString strEach;
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
   strBarRecord += (EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? _T("   ") : _T("  ")) + FormatMass(mass, false);
   strBarRecord += _T("\n");

   m_vReportLines.push_back(strBarRecord);

   CComPtr<IBend> variesBend;
   if (vbVaries == VARIANT_TRUE)
   {
      pBarRecord->get_VariesBend(&variesBend);
      CString strVariesBend = ReportBend(variesBend, true);
      strVariesBend += _T("\n");
      m_vReportLines.push_back(strVariesBend);
   }

   ReportErrors(primaryBend);
   ReportErrors(variesBend);
}

CString CReport::ReportBend(IBend* pBend, bool bVaries)
{
   CString strBend;

   if (bVaries)
   {
      strBend.Format(_T("%58s"), _T(" "));
   }

   Float64 u, w, x, y, z, t1, t2;
   pBend->get_U(&u);
   pBend->get_W(&w);
   pBend->get_X(&x);
   pBend->get_Y(&y);
   pBend->get_Z(&z);
   pBend->get_T1(&t1);
   pBend->get_T2(&t2);

   strBend += FormatLength(u, false);

   CString strLengthSpace(EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? _T("  ") : _T(" "));
   CString strSkipLength(EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? _T("       ") : _T("        "));

   VARIANT_BOOL vbSupported;
   pBend->get_SupportsDimension(dimW, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (strLengthSpace + FormatLength(w, false)) : strLengthSpace + strSkipLength;

   pBend->get_SupportsDimension(dimX, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (strLengthSpace + FormatLength(x, false)) : strLengthSpace + strSkipLength;

   pBend->get_SupportsDimension(dimY, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (_T(" ") + FormatLength(y, false)) : _T(" ") + strSkipLength;

   pBend->get_SupportsDimension(dimZ, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (_T(" ") + FormatLength(z, false)) : _T(" ") + strSkipLength;

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
   strBend += _T("  ") + FormatLength(length, false);

   return strBend;
}

void CReport::ReportErrors(IBend* pBend)
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
         m_vReportLines.push_back(strStatusMessage);
      }
   }
}

TCHAR CReport::GetUse(UseType use)
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

inline TCHAR CReport::GetFlag(VARIANT_BOOL vbFlag, TCHAR c)
{
   return vbFlag == VARIANT_TRUE ? c : ' ';
}

void CReport::ReportQNI(IBarlist* pBarlist)
{
   Float64 traffic, bridge, wall;
   pBarlist->get_TrafficBarrierQuantity(&traffic);
   pBarlist->get_BridgeGrateInletQuantity(&bridge);
   pBarlist->get_RetainingWallQuantity(&wall);

   CString strTraffic;
   strTraffic.Format(_T("Traffic: %s\n"), FormatMass(traffic));

   CString strBridge;
   strBridge.Format( _T("Bridge:  %s\n"), FormatMass(bridge));

   CString strWall;
   strWall.Format(   _T("Wall:    %s\n"), FormatMass(wall));

   m_vReportLines.push_back(_T("Quantities Not Included:\n"));
   m_vReportLines.push_back(strTraffic);
   m_vReportLines.push_back(strBridge);
   m_vReportLines.push_back(strWall);
}

void CReport::ReportSummary(IBarlist* pBarlist)
{
   Float64 sub, subEpoxy, super, superEpoxy;
   pBarlist->get_SubstructureMass(&sub);
   pBarlist->get_SubstructureMassEpoxy(&subEpoxy);
   pBarlist->get_SuperstructureMass(&super);
   pBarlist->get_SuperstructureMassEpoxy(&superEpoxy);
   Float64 total = sub + subEpoxy + super + superEpoxy;

   CString strSub;
   strSub.Format(_T("Substructure Mass:           %s\n"), FormatMass(sub));

   CString strSubEpoxy;
   strSubEpoxy.Format(_T("Substructure w/Epoxy Mass:   %s\n"), FormatMass(subEpoxy));

   CString strSuper;
   strSuper.Format(_T("Superstructure Mass:         %s\n"), FormatMass(super));

   CString strSuperEpoxy;
   strSuperEpoxy.Format(_T("Superstructure w/Epoxy Mass: %s\n"), FormatMass(superEpoxy));

   m_vReportLines.push_back(_T("Summary of Quantities:\n"));
   m_vReportLines.push_back(strSub);
   m_vReportLines.push_back(strSubEpoxy);
   m_vReportLines.push_back(strSuper);
   m_vReportLines.push_back(strSuperEpoxy);
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
   AFX_MANAGE_STATE(AfxGetModuleState());
   
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
