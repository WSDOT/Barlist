///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright � 1999-2026  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This software was developed as part of the Alternate Route Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as
// published by the Washington State Department of Transportation,
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful,
// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Open Source License
// along with this program; if not, write to the Washington State
// Department of Transportation, Bridge and Structures Office,
// 4500 3rd Ave SE, P.O. Box 47340, Olympia, WA 98503, USA or e-mail
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////


// BarRecordCollection.h : Declaration of CBarRecordCollection (native,
// replaces the IBarRecordCollection/IBarRecordEvents ATL/COM coclass,
// which used ICollectionOnSTLImpl<...>/CComEnumOnSTL over
// std::vector<CComVariant>).
//
// The COM Item(VARIANT) dual-mode index (ordinal or mark) becomes two
// calls: Item(index) and Find(mark). Move/MoveUp/MoveDown drop the
// VARIANT-typed "index, mark, or object identity" ambiguity entirely --
// callers resolve a mark to an index via Find()/IndexOf() first.

#pragma once

#include "BarsExport.h"
#include "Signal.h"
#include "Enums.h"
#include <WBFLTypes.h>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <cstddef>

class CBarRecord;
class CBarlist;

class BARS_API CBarRecordCollection
{
public:
    CBarRecordCollection() = default;
    ~CBarRecordCollection();

    // Insert()/Replace() connect a [this]-capturing lambda to each record's
    // OnBarRecordChanged, tracked in m_ChangeTokens -- a copy would carry
    // subscriptions that still call back into the original instance.
    CBarRecordCollection(const CBarRecordCollection&) = delete;
    CBarRecordCollection& operator=(const CBarRecordCollection&) = delete;

    // Non-owning back-reference to the CBarlist this collection ultimately
    // belongs to (via its owning CGroup) -- used to detect duplicate mark
    // numbers when a record is inserted/replaced.
    void SetBarlist(CBarlist* pBarlist);
    CBarlist* GetBarlist() const;

    std::size_t Count() const;
    std::shared_ptr<CBarRecord> Item(std::size_t index) const; // nullptr if index out of range
    std::shared_ptr<CBarRecord> Find(const std::_tstring& mark) const; // nullptr if not found
    long IndexOf(const CBarRecord* record) const; // -1 if not found

    void Add(std::shared_ptr<CBarRecord> record); // append
    void Insert(long index, std::shared_ptr<CBarRecord> record); // index<0 => append
    void Replace(long index, std::shared_ptr<CBarRecord> record);
    void Remove(long index);
    void Move(long recIndex, MoveType mt, long targetIndex);
    void MoveUp(long index);
    void MoveDown(long index);
    void Sort(); // numeric-then-alpha by mark

    using const_iterator = std::vector<std::shared_ptr<CBarRecord>>::const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

    Signal<CBarRecord&> OnBarRecordAdded;
    Signal<const std::_tstring&> OnBarRecordRemoved; // mark
    Signal<CBarRecord&> OnBarRecordChanged;
    Signal<> OnBarRecordsSorted;
    Signal<CBarRecord&, long, long> OnBarRecordMoved; // record, idxFrom, idxTo

private:
    std::vector<std::shared_ptr<CBarRecord>> m_Records;
    CBarlist* m_pBarlist = nullptr;

    // Tracks this collection's subscription to each record's own
    // OnBarRecordChanged (fired on e.g. Mark/Material/NumReqd changes),
    // which this collection re-fires as its own OnBarRecordChanged --
    // keyed by pointer identity so it survives Move()/Sort() reordering.
    std::map<CBarRecord*, Signal<CBarRecord&>::Token> m_ChangeTokens;
};
