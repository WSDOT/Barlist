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


// GroupCollection.cpp : Implementation of CGroupCollection

#include "GroupCollection.h"
#include "BarRecord.h"
#include "Barlist.h"
#include "BarException.h"
#include <stdexcept>
#include <tchar.h>

void CGroupCollection::SetBarlist(CBarlist* pBarlist)
{
    m_pBarlist = pBarlist;
}

CBarlist* CGroupCollection::GetBarlist() const
{
    return m_pBarlist;
}

std::size_t CGroupCollection::Count() const
{
    return m_Groups.size();
}

std::shared_ptr<CGroup> CGroupCollection::Item(std::size_t index) const
{
    if (m_Groups.size() <= index)
    {
       return std::shared_ptr<CGroup>(); // return nullptr for out-of-range index
    }
    return m_Groups[index];
}

std::shared_ptr<CGroup> CGroupCollection::Find(const std::_tstring& name) const
{
    for (const auto& group : m_Groups)
    {
        if (group->GetName() == name)
        {
            return group;
        }
    }
    return nullptr;
}

long CGroupCollection::IndexOf(const CGroup* group) const
{
    for (std::size_t i = 0; i < m_Groups.size(); ++i)
    {
        if (m_Groups[i].get() == group)
        {
            return static_cast<long>(i);
        }
    }
    return -1;
}

std::shared_ptr<CGroup> CGroupCollection::Add(std::_tstring name)
{
    if (m_pBarlist->DoesGroupExist(name))
    {
        throw CBarException(_T("DUPLICATE GROUP : Group already exists"));
    }

    auto group = std::make_shared<CGroup>();

    // SetName before SetBarlist: an unattached group's name isn't
    // duplicate/empty-checked by CGroup::SetName itself -- the check above
    // already covers it for this, the group's very first name assignment.
    group->SetName(name);
    group->SetBarlist(m_pBarlist);

    group->OnGroupChanged.Connect([this](CGroup& g) { OnGroupChanged(g); });
    group->OnBarRecordAdded.Connect([this](CGroup& g, CBarRecord& r) { OnBarRecordAdded(g, r); });
    group->OnBarRecordChanged.Connect([this](CGroup& g, CBarRecord& r) { OnBarRecordChanged(g, r); });
    group->OnBarRecordRemoved.Connect([this](CGroup& g, const std::_tstring& mark) { OnBarRecordRemoved(g, mark); });
    group->OnBarRecordsSorted.Connect([this](CGroup& g) { OnBarRecordsSorted(g); });
    group->OnBarRecordMoved.Connect([this](CGroup& g, CBarRecord& r, long from, long to) { OnBarRecordMoved(g, r, from, to); });

    m_Groups.push_back(group);

    OnGroupAdded(*group);
    return group;
}

void CGroupCollection::Remove(long index)
{
    if (index < 0 || static_cast<std::size_t>(index) >= m_Groups.size())
    {
        throw std::out_of_range("CGroupCollection::Remove: index out of range");
    }

    std::shared_ptr<CGroup> group = m_Groups[index];

    // Remove each bar in the group before the group itself is removed --
    // this needs to happen while the group's event connections are still
    // live so the owning CBarlist hears the removals and keeps its mark
    // number bookkeeping correct.
    CBarRecordCollection& bars = group->GetBarRecords();
    for (long i = static_cast<long>(bars.Count()) - 1; i >= 0; --i)
    {
        bars.Remove(i);
    }

    std::_tstring name = group->GetName();
    m_Groups.erase(m_Groups.begin() + index);

    OnGroupRemoved(name);
}

void CGroupCollection::Move(long index, MoveType mt, long targetIndex)
{
    std::shared_ptr<CGroup> group = m_Groups.at(index);

    m_Groups.erase(m_Groups.begin() + index);

    // NOTE: this must come after the erase above -- erase shifts every
    // subsequent index down by one.
    if (mt == MoveType::mtAfter && targetIndex < index)
    {
        targetIndex++;
    }

    m_Groups.insert(m_Groups.begin() + targetIndex, group);

    OnGroupMoved(*group, index, targetIndex);
}

void CGroupCollection::MoveUp(long index)
{
    if (index == 0)
    {
        return; // already at the top
    }

    Move(index, MoveType::mtBefore, index - 1);
}

void CGroupCollection::MoveDown(long index)
{
    if (static_cast<std::size_t>(index) == m_Groups.size() - 1)
    {
        return; // already at the bottom
    }

    Move(index, MoveType::mtAfter, index + 1);
}

void CGroupCollection::RenameGroup(long index, std::_tstring newName)
{
    if (index < 0 || static_cast<std::size_t>(index) >= m_Groups.size())
    {
        throw std::out_of_range("CGroupCollection::RenameGroup: index out of range");
    }

    m_Groups[index]->SetName(std::move(newName)); // throws CBarException on empty/duplicate name
}

CGroupCollection::const_iterator CGroupCollection::begin() const
{
    return m_Groups.begin();
}

CGroupCollection::const_iterator CGroupCollection::end() const
{
    return m_Groups.end();
}
