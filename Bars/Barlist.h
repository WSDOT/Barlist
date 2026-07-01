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


// Barlist.h : Declaration of CBarlist (native, replaces the
// IBarlist/IGroupCollectionEvents ATL/COM coclass -- the root of the
// Bars object model).

#pragma once

#include "BarsExport.h"
#include "Signal.h"
#include "Enums.h"
#include "GroupCollection.h"
#include <WBFLTypes.h>
#include <set>
#include <map>
#include <array>
#include <string>
#include <tchar.h>

class BARS_API CBarlist
{
public:
    CBarlist();

    // The constructor connects [this]-capturing lambdas to m_Groups's signals
    // (to re-fire them as CBarlist's own) -- a copy would carry subscriptions
    // that still call back into the original instance.
    CBarlist(const CBarlist&) = delete;
    CBarlist& operator=(const CBarlist&) = delete;

    bool BarRecordMultiUse(const std::_tstring& mark) const;
    void ReplaceMarkNumber(const std::_tstring& oldMark, const std::_tstring& newMark);
    void RemoveMarkNumber(const std::_tstring& mark);
    bool DoesGroupExist(const std::_tstring& groupName) const;
    void ReplaceGroup(const std::_tstring& oldName, const std::_tstring& newName);

    CGroupCollection& GetGroups();
    const CGroupCollection& GetGroups() const;

    Float64 GetQuantity(MaterialType material, bool bEpoxy, bool bSubstructure) const;
    Float64 GetQuantityByGroup(const std::_tstring& groupName, MaterialType material, bool bEpoxy, bool bSubstructure) const;

    StatusType GetStatus() const;

    const std::_tstring& GetProject() const;
    void SetProject(std::_tstring val);
    const std::_tstring& GetJobNumber() const;
    void SetJobNumber(std::_tstring val);
    const std::_tstring& GetEngineer() const;
    void SetEngineer(std::_tstring val);
    const std::_tstring& GetCompany() const;
    void SetCompany(std::_tstring val);
    const std::_tstring& GetComments() const;
    void SetComments(std::_tstring val);

    // Declared for interface parity with the original IBarlistEvents (which
    // carried this member beyond the publicly-shipped SDK's event set) --
    // nothing inside Bars itself ever fires it today.
    Signal<> OnNotIncludedQuantitiesChanged;

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
    void Update();

    CGroupCollection m_Groups;

    // Sentinel key for "totals across all groups" in the per-group quantity
    // maps below. Group names can never be empty (CGroup::SetName rejects
    // it), so "" can never collide with a real group name.
    static const std::_tstring AllGroups;

    // key is group name; array index is MaterialType. Quantities are
    // mass/weight in all cases except D7957 (GFRP), where the quantity is
    // length.
    std::map<std::_tstring, std::array<Float64, MATERIAL_COUNT>> m_Substructure;
    std::map<std::_tstring, std::array<Float64, MATERIAL_COUNT>> m_SubstructureEpoxy;
    std::map<std::_tstring, std::array<Float64, MATERIAL_COUNT>> m_Superstructure;
    std::map<std::_tstring, std::array<Float64, MATERIAL_COUNT>> m_SuperstructureEpoxy;

    StatusType m_Status = StatusType::stOK;

    std::_tstring m_Project = _T("Barlist");
    std::_tstring m_JobNumber;
    std::_tstring m_Engineer;
    std::_tstring m_Company;
    std::_tstring m_Comments;

    // Collection of mark numbers so duplicate marks can be detected cheaply.
    std::multiset<std::_tstring> m_Marks;
    std::set<std::_tstring> m_GroupNames;
};
