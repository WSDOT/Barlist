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


// BarCollection.h : Declaration of CBarCollection (native, replaces the
// IBarCollection ATL/COM coclass, which used ICollectionOnSTLImpl<...>/
// CComEnumOnSTL over std::vector<CComVariant>).
//
// Stores CBarData by value: this is immutable static reference data
// (standard bar size tables) built once and never removed/reordered,
// so no shared ownership is needed for its elements.

#pragma once

#include "BarsExport.h"
#include "BarData.h"
#include <WBFLTypes.h>
#include <vector>
#include <cstddef>

class BARS_API CBarCollection
{
public:
    CBarCollection() = default;

    void InitSteelBars();
    void InitGalvanizedBars();
    void InitGFRPBars();

    std::size_t Count() const;
    const CBarData* Item(std::size_t index) const; // nullptr if index out of range
    const CBarData* Find(const std::_tstring& name) const; // nullptr if not found, e.g. "#5"

    using const_iterator = std::vector<CBarData>::const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

    // Implementation detail, public only so the static reference-data tables
    // in BarCollection.cpp can be declared at file scope.
    struct BarInfo;
    struct BendInfo;
    struct HookInfo;

private:
    void Init(const BarInfo barInfo[], int cBars, const BendInfo bendInfo[], int cBends, const HookInfo hookInfo[], int cHooks);

    std::vector<CBarData> m_Bars;
};
