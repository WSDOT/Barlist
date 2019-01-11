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

class CReport
{
public:
   CReport();
   ~CReport();

   CString GetReport(IBarlist* pBarlist);

private:
   CString ReportGroups(IBarlist* pBarlist);
   CString ReportBarRecords(IGroup* pGroup);
   CString ReportBarRecord(IBarRecord* pBarRecord);
   CString ReportQNI(IBarlist* pBarlist);
   CString ReportSummary(IBarlist* pBarlist);

   TCHAR CReport::GetUse(UseType use);
   TCHAR GetFlag(VARIANT_BOOL vbFlag, TCHAR c);
   CString ReportBend(IBend* pBend,bool bVaries);
};

