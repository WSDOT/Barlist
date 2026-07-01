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


// Group.cpp : Implementation of CGroup

#include "Group.h"
#include "BarRecord.h"
#include "BendImpl.h"
#include "Barlist.h"
#include "BarException.h"
#include <tchar.h>

CGroup::CGroup()
{
    m_Bars.OnBarRecordAdded.Connect([this](CBarRecord& r) { OnBarRecordAdded(*this, r); UpdateStatus(); });
    m_Bars.OnBarRecordRemoved.Connect([this](const std::_tstring& mark) { OnBarRecordRemoved(*this, mark); UpdateStatus(); });
    m_Bars.OnBarRecordChanged.Connect([this](CBarRecord& r) { OnBarRecordChanged(*this, r); UpdateStatus(); });
    m_Bars.OnBarRecordsSorted.Connect([this] { OnBarRecordsSorted(*this); });
    m_Bars.OnBarRecordMoved.Connect([this](CBarRecord& r, long from, long to) { OnBarRecordMoved(*this, r, from, to); });
}

void CGroup::SetBarlist(CBarlist* pBarlist)
{
    m_pBarlist = pBarlist;
    m_Bars.SetBarlist(pBarlist);
}

CBarlist* CGroup::GetBarlist() const
{
    return m_pBarlist;
}

const std::_tstring& CGroup::GetName() const
{
    return m_Name;
}

void CGroup::SetName(std::_tstring val)
{
    // Only validate once this group is attached to a CBarlist -- CGroupCollection::Add
    // sets the initial name before attaching the group (it already pre-checks for a
    // duplicate name itself), so an unattached group's name is never validated here.
    if (m_pBarlist)
    {
        if (val.empty())
        {
            throw CBarException(_T("INVALID GROUP NAME : Group name cannot be empty"));
        }

        if (m_pBarlist->DoesGroupExist(val))
        {
            throw CBarException(_T("DUPLICATE GROUP : Group already exists"));
        }

        m_pBarlist->ReplaceGroup(m_Name, val);
    }

    m_Name = std::move(val);
    OnGroupChanged(*this);
}

CBarRecordCollection& CGroup::GetBarRecords()
{
    return m_Bars;
}

const CBarRecordCollection& CGroup::GetBarRecords() const
{
    return m_Bars;
}

Float64 CGroup::GetQuantity(MaterialType material, bool bEpoxy, bool bSubstructure) const
{
    std::size_t materialIdx = static_cast<std::size_t>(material);

    if (bSubstructure)
    {
        return bEpoxy ? m_SubstructureEpoxy[materialIdx] : m_Substructure[materialIdx];
    }
    else
    {
        return bEpoxy ? m_SuperstructureEpoxy[materialIdx] : m_Superstructure[materialIdx];
    }
}

StatusType CGroup::GetStatus()
{
    UpdateStatus();
    return m_Status;
}

void CGroup::UpdateStatus()
{
    m_Superstructure.fill(0);
    m_SuperstructureEpoxy.fill(0);
    m_Substructure.fill(0);
    m_SubstructureEpoxy.fill(0);

    m_Status = StatusType::stOK;

    if (m_Name.empty())
    {
        m_Status = StatusType::stError;
    }

    for (const auto& bar : m_Bars)
    {
        StatusType status = bar->GetStatus();
        if (m_Status < status)
        {
            m_Status = status;
        }

        MaterialType material = bar->GetMaterial();
        bool bEpoxy = bar->GetEpoxy();
        bool bSubstructure = bar->GetSubstructure();
        bool bVaries = bar->GetVaries();

        Float64 quantity = 0;
        if (material == MaterialType::D7957)
        {
            // the quantity for D7957/GFRP bars is length
            Float64 length = bar->GetPrimaryBend()->GetLength();

            if (bVaries)
            {
                Float64 variesLength = bar->GetVariesBend()->GetLength();
                // average length of primary and varies bends
                length = (length + variesLength) / 2;
            }

            quantity = bar->GetNumReqd() * length;
        }
        else
        {
            quantity = bar->GetMass();
        }

        std::size_t materialIdx = static_cast<std::size_t>(material);
        if (bSubstructure)
        {
            if (bEpoxy)
            {
                m_SubstructureEpoxy[materialIdx] += quantity;
            }
            else
            {
                m_Substructure[materialIdx] += quantity;
            }
        }
        else
        {
            if (bEpoxy)
            {
                m_SuperstructureEpoxy[materialIdx] += quantity;
            }
            else
            {
                m_Superstructure[materialIdx] += quantity;
            }
        }
    }
}
