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


// FabricationConstraints.h: interface for the CFabricationConstraints class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BarsExport.h"
#include "Enums.h"
#include <WBFLTypes.h>

class CBarData;

class BARS_API CFabricationConstraints
{
public:
    static Float64 GetBendDeduction(Float64 OR, // outside radius
                                     Float64 angle);
    static Float64 GetBendDeduction(const CBarData& barData, UseType use);
    static Float64 GetHookDeduction(const CBarData& barData, UseType use, HookType hook);
    static Float64 GetHookRadius(const CBarData& barData, UseType use);
    static Float64 GetTailLength(const CBarData& barData, UseType use, HookType hook); // throws CBarException

    static Float64 GetInsideBendDiameter(const CBarData& barData, UseType use); // throws CBarException
    static Float64 GetOutsideBendDiameter(const CBarData& barData, UseType use);
    static Float64 GetCenterlineBendDiameter(const CBarData& barData, UseType use);
    static Float64 GetInsideBendRadius(const CBarData& barData, UseType use);
    static Float64 GetOutsideBendRadius(const CBarData& barData, UseType use);
    static Float64 GetCenterlineBendRadius(const CBarData& barData, UseType use);
};
