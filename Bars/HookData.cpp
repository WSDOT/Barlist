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


// HookData.cpp : Implementation of CHookData

#include "HookData.h"

void CHookData::SetData(HookType hook, UseType use, Float64 T, BendMeasureType bm, Float64 Tmin)
{
    m_HookType = hook;
    m_UseType = use;
    m_Tail = T;
    m_BendMeasure = bm;
    m_MinTail = Tmin;
}

HookType CHookData::GetType() const
{
    return m_HookType;
}

UseType CHookData::GetUse() const
{
    return m_UseType;
}

Float64 CHookData::GetTail() const
{
    return m_Tail;
}

BendMeasureType CHookData::GetBendMeasure() const
{
    return m_BendMeasure;
}

Float64 CHookData::GetMinTail() const
{
    return m_MinTail;
}
