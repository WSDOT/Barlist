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


// Barlist.cpp : Implementation of CBarlist

#include "Barlist.h"
#include "BarRecord.h"

const std::_tstring CBarlist::AllGroups = _T("");

CBarlist::CBarlist()
{
    m_Substructure[AllGroups].fill(0);
    m_SubstructureEpoxy[AllGroups].fill(0);
    m_Superstructure[AllGroups].fill(0);
    m_SuperstructureEpoxy[AllGroups].fill(0);

    m_Groups.SetBarlist(this);

    m_Groups.OnGroupAdded.Connect([this](CGroup& g)
        {
            m_GroupNames.insert(g.GetName());
            Update();
            OnGroupAdded(g);
        });

    m_Groups.OnGroupRemoved.Connect([this](const std::_tstring& name)
        {
            m_GroupNames.erase(name);
            Update();
            OnGroupRemoved(name);
        });

    m_Groups.OnGroupChanged.Connect([this](CGroup& g)
        {
            Update();
            OnGroupChanged(g);
        });

    m_Groups.OnGroupMoved.Connect([this](CGroup& g, long from, long to)
        {
            OnGroupMoved(g, from, to);
        });

    m_Groups.OnBarRecordAdded.Connect([this](CGroup& g, CBarRecord& r)
        {
            m_Marks.insert(r.GetMark());
            Update();
            OnBarRecordAdded(g, r);
        });

    m_Groups.OnBarRecordChanged.Connect([this](CGroup& g, CBarRecord& r)
        {
            Update();
            OnBarRecordChanged(g, r);
        });

    m_Groups.OnBarRecordRemoved.Connect([this](CGroup& g, const std::_tstring& mark)
        {
            RemoveMarkNumber(mark);
            Update();
            OnBarRecordRemoved(g, mark);
        });

    m_Groups.OnBarRecordsSorted.Connect([this](CGroup& g)
        {
            OnBarRecordsSorted(g);
        });

    m_Groups.OnBarRecordMoved.Connect([this](CGroup& g, CBarRecord& r, long from, long to)
        {
            OnBarRecordMoved(g, r, from, to);
        });
}

bool CBarlist::BarRecordMultiUse(const std::_tstring& mark) const
{
    auto first = m_Marks.lower_bound(mark);
    auto last = m_Marks.upper_bound(mark);

    // Mark wasn't found so it can't be used more than once
    if (first == m_Marks.end())
    {
        return false;
    }

    if (first == last && *first != mark)
    {
        return false;
    }

    // If iterators are at the same location then there is only one occurance
    // of the mark number
    if (++first == last)
    {
        return false;
    }

    // At least two occurances of the mark number were found
    return true;
}

void CBarlist::ReplaceMarkNumber(const std::_tstring& oldMark, const std::_tstring& newMark)
{
    auto found = m_Marks.find(oldMark);
    if (found != m_Marks.end())
    {
        m_Marks.erase(found);
        m_Marks.insert(newMark);
    }
}

void CBarlist::RemoveMarkNumber(const std::_tstring& mark)
{
    auto found = m_Marks.find(mark);
    if (found != m_Marks.end())
    {
        m_Marks.erase(found);
    }
}

bool CBarlist::DoesGroupExist(const std::_tstring& groupName) const
{
    return m_GroupNames.find(groupName) != m_GroupNames.end();
}

void CBarlist::ReplaceGroup(const std::_tstring& oldName, const std::_tstring& newName)
{
    m_GroupNames.erase(oldName);
    m_GroupNames.insert(newName);
}

CGroupCollection& CBarlist::GetGroups()
{
    return m_Groups;
}

const CGroupCollection& CBarlist::GetGroups() const
{
    return m_Groups;
}

void CBarlist::Update()
{
    m_Superstructure.clear();
    m_SuperstructureEpoxy.clear();
    m_Substructure.clear();
    m_SubstructureEpoxy.clear();

    m_Superstructure[AllGroups].fill(0);
    m_SuperstructureEpoxy[AllGroups].fill(0);
    m_Substructure[AllGroups].fill(0);
    m_SubstructureEpoxy[AllGroups].fill(0);

    m_Status = StatusType::stOK;

    for (const auto& group : m_Groups)
    {
        const std::_tstring& groupName = group->GetName();

        m_Superstructure[groupName].fill(0);
        m_SuperstructureEpoxy[groupName].fill(0);
        m_Substructure[groupName].fill(0);
        m_SubstructureEpoxy[groupName].fill(0);

        StatusType status = group->GetStatus();
        if (m_Status < status)
        {
            m_Status = status;
        }

        for (int i = 0; i < MATERIAL_COUNT; i++)
        {
            MaterialType material = static_cast<MaterialType>(i);

            Float64 quantity = group->GetQuantity(material, true, true);
            m_SubstructureEpoxy[groupName][i] = quantity;
            m_SubstructureEpoxy[AllGroups][i] += quantity;

            quantity = group->GetQuantity(material, false, true);
            m_Substructure[groupName][i] = quantity;
            m_Substructure[AllGroups][i] += quantity;

            quantity = group->GetQuantity(material, true, false);
            m_SuperstructureEpoxy[groupName][i] = quantity;
            m_SuperstructureEpoxy[AllGroups][i] += quantity;

            quantity = group->GetQuantity(material, false, false);
            m_Superstructure[groupName][i] = quantity;
            m_Superstructure[AllGroups][i] += quantity;
        }
    }
}

Float64 CBarlist::GetQuantity(MaterialType material, bool bEpoxy, bool bSubstructure) const
{
    return GetQuantityByGroup(AllGroups, material, bEpoxy, bSubstructure);
}

Float64 CBarlist::GetQuantityByGroup(const std::_tstring& groupName, MaterialType material, bool bEpoxy, bool bSubstructure) const
{
    std::size_t materialIdx = +material;

    if (bSubstructure)
    {
        return bEpoxy ? m_SubstructureEpoxy.at(groupName)[materialIdx] : m_Substructure.at(groupName)[materialIdx];
    }
    else
    {
        return bEpoxy ? m_SuperstructureEpoxy.at(groupName)[materialIdx] : m_Superstructure.at(groupName)[materialIdx];
    }
}

StatusType CBarlist::GetStatus() const
{
    return m_Status;
}

const std::_tstring& CBarlist::GetProject() const { return m_Project; }
void CBarlist::SetProject(std::_tstring val) { m_Project = std::move(val); }

const std::_tstring& CBarlist::GetJobNumber() const { return m_JobNumber; }
void CBarlist::SetJobNumber(std::_tstring val) { m_JobNumber = std::move(val); }

const std::_tstring& CBarlist::GetEngineer() const { return m_Engineer; }
void CBarlist::SetEngineer(std::_tstring val) { m_Engineer = std::move(val); }

const std::_tstring& CBarlist::GetCompany() const { return m_Company; }
void CBarlist::SetCompany(std::_tstring val) { m_Company = std::move(val); }

const std::_tstring& CBarlist::GetComments() const { return m_Comments; }
void CBarlist::SetComments(std::_tstring val) { m_Comments = std::move(val); }
