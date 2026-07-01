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


// BarData.h : Declaration of CBarData (native, replaces the IBarData
// ATL/COM coclass). Immutable per-bar-size reference data, owned and
// vended by CBarCollection's static tables.

#pragma once

#include "BarsExport.h"
#include "BendData.h"
#include "HookData.h"
#include <WBFLTypes.h>
#include <string>
#include <array>
#include <optional>

class BARS_API CBarData
{
public:
    CBarData() = default;

    void SetBarData(const std::_tstring& name, Float64 db, Float64 mass, Float64 length, Float64 maxLength);
    void AddBendData(UseType use, Float64 ID, BendMeasureType bendMeasure);
    void AddHookData(UseType use, HookType hook, Float64 T, BendMeasureType bendMeasure, Float64 Tmin);

    const std::_tstring& GetName() const;
    Float64 GetDiameter() const;
    Float64 GetMass() const;
    Float64 GetNormalLength() const;
    Float64 GetMaxLength() const;

    // Returns nullptr if no bend/hook data was registered for the given use/hook,
    // matching the original COM getters' silent-null behavior.
    const CBendData* GetBendData(UseType use) const;
    const CHookData* GetHookData(UseType use, HookType hook) const;

private:
    std::_tstring m_Name;
    Float64 m_db = 0;
    Float64 m_Mass = 0;
    Float64 m_Length = 0;
    Float64 m_MaxLength = 0;

    std::array<std::optional<CBendData>, 3> m_Bends;
    std::array<std::array<std::optional<CHookData>, 3>, 3> m_Hooks;
};
