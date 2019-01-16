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

#pragma once
#include <Bars\Bars.h>
#include <vector>

class CReport
{
public:
   CReport();
   ~CReport();

   static CString GetReportHeader();

   void BuildReport(IBarlist* pBarlist);

   const std::vector<CString>& GetReport();

   void PrepareForPrinting(CDC* pDC, CPrintInfo* pInfo);
   void Print(CDC* pDC, CPrintInfo* pInfo);
   void EndPrinting(CDC* pDC, CPrintInfo* pInfo);

private:
   void PageHeader(CDC* pDC, CPrintInfo* pInfo);
   void PrintCoverPage(CDC* pDC, CPrintInfo* pInfo);
   void ReportGroups(IBarlist* pBarlist);
   void ReportBarRecords(IGroup* pGroup);
   void ReportBarRecord(IBarRecord* pBarRecord);
   CString ReportBend(IBend* pBend, bool bVaries);
   void ReportErrors(IBend* pBend);
   void ReportQNI(IBarlist* pBarlist);
   void ReportSummary(IBarlist* pBarlist);

   TCHAR CReport::GetUse(UseType use);
   TCHAR GetFlag(VARIANT_BOOL vbFlag, TCHAR c);

   std::vector<CString> m_vReportLines;

   CString m_strProject;
   CString m_strJobNumber;
   CString m_strEngineer;
   CString m_strCompany;
   CString m_strComments;

   CFont m_Font;
   CFont* m_pOldFont;
   LONG m_hLine;
   LONG m_nHeaderLines;
   LONG m_nLinesPerPage;
   CRect m_Border;
   CRect m_Rect;
};

