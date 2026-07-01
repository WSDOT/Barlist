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


// GroupCollection.h : Declaration of CGroupCollection (native, replaces
// the IGroupCollection/IGroupEvents ATL/COM coclass).
//
// The per-group name-keyed cookie map (std::map<std::string,DWORD>
// m_Cookies) used to Advise/Unadvise each CGroup's connection point is
// gone entirely -- CGroupCollection connects a lambda (capturing the
// group's shared_ptr) to each CGroup's Signals when the group is added,
// and since the group's Signals are destroyed along with the group
// itself, there is nothing to explicitly unsubscribe on removal.

#pragma once

#include "BarsExport.h"
#include "Signal.h"
#include "Enums.h"
#include "Group.h"
#include <WBFLTypes.h>
#include <vector>
#include <memory>
#include <string>
#include <cstddef>

class CBarlist;

class BARS_API CGroupCollection
{
public:
    CGroupCollection() = default;

    // Add() connects [this]-capturing lambdas to each new group's signals
    // (to re-fire them as this collection's own) -- a copy would carry
    // subscriptions that still call back into the original instance.
    CGroupCollection(const CGroupCollection&) = delete;
    CGroupCollection& operator=(const CGroupCollection&) = delete;

    void SetBarlist(CBarlist* pBarlist);
    CBarlist* GetBarlist() const;

    std::size_t Count() const;
    std::shared_ptr<CGroup> Item(std::size_t index) const; // nullptr if index out of range
    std::shared_ptr<CGroup> Find(const std::_tstring& name) const; // nullptr if not found
    long IndexOf(const CGroup* group) const; // -1 if not found

    std::shared_ptr<CGroup> Add(std::_tstring name); // throws CBarException: empty/duplicate name
    void Remove(long index);
    void Move(long index, MoveType mt, long targetIndex);
    void MoveUp(long index);
    void MoveDown(long index);
    void RenameGroup(long index, std::_tstring newName); // throws CBarException

    using const_iterator = std::vector<std::shared_ptr<CGroup>>::const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

    Signal<CGroup&> OnGroupAdded;
    Signal<const std::_tstring&> OnGroupRemoved;
    Signal<CGroup&> OnGroupChanged;
    Signal<CGroup&, long, long> OnGroupMoved;
    Signal<CGroup&, CBarRecord&> OnBarRecordAdded;
    Signal<CGroup&, CBarRecord&> OnBarRecordChanged;
    Signal<CGroup&, const std::_tstring&> OnBarRecordRemoved;
    Signal<CGroup&> OnBarRecordsSorted;
    Signal<CGroup&, CBarRecord&, long, long> OnBarRecordMoved;

private:
    std::vector<std::shared_ptr<CGroup>> m_Groups;
    CBarlist* m_pBarlist = nullptr;
};
