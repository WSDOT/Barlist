///////////////////////////////////////////////////////////////////////
// Barlist
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

#pragma once

interface IGroup;
interface IBarRecord;

class CGroupEventHint : public CObject
{
public:
   CGroupEventHint(IGroup* pGroup, long fromIdx = -1, long toIdx = -1) : group(pGroup), fromIdx(fromIdx), toIdx(toIdx)
   {}

   CComPtr<IGroup> group;
   long fromIdx;
   long toIdx;
};

class CBarRecordEventHint : public CGroupEventHint
{
public:
   CBarRecordEventHint(IGroup* pGroup, IBarRecord* pBarRecord, long fromIdx = -1, long toIdx = -1) :
      CGroupEventHint(pGroup, fromIdx, toIdx), pBarRecord(pBarRecord)
   {}

   CComPtr<IBarRecord> pBarRecord;
};

#define HINT_PROJECT_CHANGED 1

#define HINT_GROUP_ADDED   10
#define HINT_GROUP_MOVED   20
#define HINT_GROUP_REMOVED 30
#define HINT_GROUP_CHANGED 40
#define HINT_GROUP_SORTED  50

#define HINT_BAR_ADDED     100
#define HINT_BAR_MOVED     200
#define HINT_BAR_REMOVED   300
#define HINT_BAR_CHANGED   400
