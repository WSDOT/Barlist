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


// HookData.h : Declaration of CHookData (native, replaces the IHookData
// ATL/COM coclass). Immutable reference data.

#pragma once

#include "BarsExport.h"
#include "Enums.h"
#include <WBFLTypes.h>

class BARS_API CHookData
{
public:
    CHookData() = default;

    void SetData(HookType hook, UseType use, Float64 T, BendMeasureType bm, Float64 Tmin);

    HookType GetType() const;
    UseType GetUse() const;
    Float64 GetTail() const;
    BendMeasureType GetBendMeasure() const;
    Float64 GetMinTail() const;

private:
    HookType m_HookType = HookType::ht90;
    UseType m_UseType = UseType::utLongitudinal;
    Float64 m_Tail = 0;
    BendMeasureType m_BendMeasure = BendMeasureType::BarDiameter;
    Float64 m_MinTail = 0;
};
