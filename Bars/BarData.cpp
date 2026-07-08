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


// BarData.cpp : Implementation of CBarData

#include "BarData.h"

void CBarData::SetBarData(const std::_tstring& name, Float64 db, Float64 mass, Float64 length, Float64 maxLength)
{
    m_Name = name;
    m_db = db;
    m_Mass = mass;
    m_Length = length;
    m_MaxLength = maxLength;
}

void CBarData::AddBendData(UseType use, Float64 ID, BendMeasureType bendMeasure)
{
    CBendData bendData;
    bendData.SetData(bendMeasure, ID, use);
    m_Bends[+use] = bendData;
}

void CBarData::AddHookData(UseType use, HookType hook, Float64 T, BendMeasureType bendMeasure, Float64 Tmin)
{
    CHookData hookData;
    hookData.SetData(hook, use, T, bendMeasure, Tmin);
    m_Hooks[+use][+hook] = hookData;
}

const std::_tstring& CBarData::GetName() const
{
    return m_Name;
}

Float64 CBarData::GetDiameter() const
{
    return m_db;
}

Float64 CBarData::GetMass() const
{
    return m_Mass;
}

Float64 CBarData::GetNormalLength() const
{
    return m_Length;
}

Float64 CBarData::GetMaxLength() const
{
    return m_MaxLength;
}

const CBendData* CBarData::GetBendData(UseType use) const
{
    const auto& bend = m_Bends[+use];
    return bend.has_value() ? &bend.value() : nullptr;
}

const CHookData* CBarData::GetHookData(UseType use, HookType hook) const
{
    const auto& hookData = m_Hooks[+use][+hook];
    return hookData.has_value() ? &hookData.value() : nullptr;
}
