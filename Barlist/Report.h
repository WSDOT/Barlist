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

#pragma once
#include <Bars\Enums.h>
#include <vector>

class CBarlist;
class CGroup;
class CBarRecord;
class CBend;

class CReport
{
public:
   CReport();
   ~CReport();

   static CString GetReportHeader();



   void BuildReport(CBarlist& barlist);

   const std::vector<CString>& GetReport();

   void PrepareForPrinting(CDC* pDC, CPrintInfo* pInfo);
   void Print(CDC* pDC, CPrintInfo* pInfo);
   void EndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
   void PageHeader(CDC* pDC, CPrintInfo* pInfo);
   void PrintCoverPage(CDC* pDC, CPrintInfo* pInfo);
   void ReportGroups(CBarlist& barlist);
   void AddReportHeader();
   void AddMaterialDataToReport(MaterialType material, Float64 sub, Float64 subEpoxy, Float64 super, Float64 superEpoxy);
   void ReportQuantities(CBarlist& barlist);
   void ReportQuantitiesByGroup(CBarlist& barlist);
   void ReportBarRecords(CGroup& group);
   void ReportBarRecord(CBarRecord& barRecord);
   CString ReportBend(CBend& bend, bool bVaries);
   void ReportErrors(CBend* bend);


   TCHAR GetUse(UseType use);
   TCHAR GetFlag(bool bFlag, TCHAR c);
   CString GetMaterial(MaterialType material, bool bEpoxy);



   std::vector<CString> m_vReportLines;

   CString m_strProject;
   CString m_strJobNumber;
   CString m_strEngineer;
   CString m_strCompany;
   CString m_strComments;

   CFont m_Font;
   CFont* m_pOldFont;
   long m_hLine;
   long m_nHeaderLines;
   long m_nLinesPerPage;
   CRect m_Border;
   CRect m_Rect;
};
