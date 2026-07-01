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


// Group.h : Declaration of CGroup (native, replaces the
// IGroup/IBarRecordCollectionEvents ATL/COM coclass).

#pragma once

#include "BarsExport.h"
#include "Signal.h"
#include "Enums.h"
#include "BarRecordCollection.h"
#include <WBFLTypes.h>
#include <array>
#include <string>

class CBarlist;

class BARS_API CGroup
{
public:
    CGroup();

    // The constructor connects [this]-capturing lambdas to m_Bars's signals
    // (to re-fire them as CGroup's own) -- a copy would carry subscriptions
    // that still call back into the original instance.
    CGroup(const CGroup&) = delete;
    CGroup& operator=(const CGroup&) = delete;

    // Non-owning back-reference.
    void SetBarlist(CBarlist* pBarlist);
    CBarlist* GetBarlist() const;

    const std::_tstring& GetName() const;
    void SetName(std::_tstring val); // throws CBarException: empty name, or a duplicate within the owning CBarlist

    CBarRecordCollection& GetBarRecords();
    const CBarRecordCollection& GetBarRecords() const;

    Float64 GetQuantity(MaterialType material, bool bEpoxy, bool bSubstructure) const;
    StatusType GetStatus();

    Signal<CGroup&> OnGroupChanged;
    Signal<CGroup&, CBarRecord&> OnBarRecordAdded;
    Signal<CGroup&, CBarRecord&> OnBarRecordChanged;
    Signal<CGroup&, const std::_tstring&> OnBarRecordRemoved;
    Signal<CGroup&> OnBarRecordsSorted;
    Signal<CGroup&, CBarRecord&, long, long> OnBarRecordMoved;

private:
    void UpdateStatus();

    std::_tstring m_Name;
    CBarRecordCollection m_Bars;
    CBarlist* m_pBarlist = nullptr;

    // array index is MaterialType; quantities are mass/weight in all cases
    // except D7957 (GFRP), where the quantity is length.
    std::array<Float64, MATERIAL_COUNT> m_Superstructure{};
    std::array<Float64, MATERIAL_COUNT> m_SuperstructureEpoxy{};
    std::array<Float64, MATERIAL_COUNT> m_Substructure{};
    std::array<Float64, MATERIAL_COUNT> m_SubstructureEpoxy{};

    StatusType m_Status = StatusType::stOK;
};
