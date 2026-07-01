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

class CGroup;
class CBarRecord;

// Non-owning: these hints are constructed, passed synchronously through
// UpdateAllViews, and discarded within the same call -- the underlying
// CGroup/CBarRecord is always kept alive by its owning collection for the
// duration of the (synchronous) event dispatch that produced this hint.
class CGroupEventHint : public CObject
{
public:
   CGroupEventHint(CGroup* pGroup, long fromIdx = -1, long toIdx = -1) : group(pGroup), fromIdx(fromIdx), toIdx(toIdx)
   {}

   CGroup* group;
   long fromIdx;
   long toIdx;
};

class CBarRecordEventHint : public CGroupEventHint
{
public:
   CBarRecordEventHint(CGroup* pGroup, CBarRecord* pBarRecord, long fromIdx = -1, long toIdx = -1) :
      CGroupEventHint(pGroup, fromIdx, toIdx), pBarRecord(pBarRecord)
   {}

   CBarRecord* pBarRecord;
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
