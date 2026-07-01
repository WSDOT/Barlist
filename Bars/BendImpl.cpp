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


// BendImpl.cpp: implementation of the CBend class.

#include "BendImpl.h"
#include "BarComponent.h"
#include "BarRecord.h"
#include "BarData.h"
#include "BarException.h"
#include <stdexcept>
#include <tchar.h>

CBend::CBend(long bendType, bool bU, bool bW, bool bX, bool bY, bool bZ, bool bT1, bool bT2) :
    m_bU(bU), m_bW(bW), m_bX(bX), m_bY(bY), m_bZ(bZ), m_bT1(bT1), m_bT2(bT2),
    m_BendType(bendType)
{
    ResetStatusMsgs();
}

CBend::~CBend()
{
    Clear();
}

CBarRecord* CBend::GetBarRecord() const
{
    return m_pBarRecord;
}

void CBend::SetBarRecord(CBarRecord* pBarRecord)
{
    m_pBarRecord = pBarRecord;
    m_bIsDirty = true;
    OnBendChanged();
}

Float64 CBend::GetU() const { return m_U; }
void CBend::SetU(Float64 val) { m_U = val; m_bIsDirty = true; OnBendChanged(); }
Float64 CBend::GetW() const { return m_W; }
void CBend::SetW(Float64 val) { m_W = val; m_bIsDirty = true; OnBendChanged(); }
Float64 CBend::GetX() const { return m_X; }
void CBend::SetX(Float64 val) { m_X = val; m_bIsDirty = true; OnBendChanged(); }
Float64 CBend::GetY() const { return m_Y; }
void CBend::SetY(Float64 val) { m_Y = val; m_bIsDirty = true; OnBendChanged(); }
Float64 CBend::GetZ() const { return m_Z; }
void CBend::SetZ(Float64 val) { m_Z = val; m_bIsDirty = true; OnBendChanged(); }
Float64 CBend::GetT1() const { return m_T1; }
void CBend::SetT1(Float64 val) { m_T1 = val; m_bIsDirty = true; OnBendChanged(); }
Float64 CBend::GetT2() const { return m_T2; }
void CBend::SetT2(Float64 val) { m_T2 = val; m_bIsDirty = true; OnBendChanged(); }

StatusType CBend::GetStatus()
{
    ComputeLength(); // forces status to be updated
    return m_StatusMgr.GetStatusLevel();
}

const CStatusMessageCollection& CBend::GetStatusMessages()
{
    return m_StatusMgr.GetStatusMessages();
}

Float64 CBend::GetLength()
{
    if (m_bIsDirty)
    {
        ComputeLength();
    }
    return m_Length;
}

long CBend::GetBendType() const
{
    return m_BendType;
}

bool CBend::SupportsDimension(DimensionType type) const
{
    switch (type)
    {
    case DimensionType::dimU:  return m_bU;
    case DimensionType::dimW:  return m_bW;
    case DimensionType::dimX:  return m_bX;
    case DimensionType::dimY:  return m_bY;
    case DimensionType::dimZ:  return m_bZ;
    case DimensionType::dimT1: return m_bT1;
    case DimensionType::dimT2: return m_bT2;
    default:
        throw std::invalid_argument("CBend::SupportsDimension: invalid DimensionType");
    }
}

void CBend::SetStatusLevel(StatusType status)
{
    m_StatusMgr.SetStatusLevel(status);
}

void CBend::AddStatusMsg(std::_tstring msg, StatusValue v1, StatusValue v2)
{
    m_StatusMgr.AddStatusMsg(std::move(msg), std::move(v1), std::move(v2));
}

void CBend::PreValidateBend()
{
    if (m_U <= 0)
    {
        SetStatusLevel(StatusType::stError);
        AddStatusMsg(_T("ERROR : %1 must be greater than %2"), std::_tstring(_T("U")), 0.0);
    }

    // GFRP Bars can only be bent into Type 50, 51, 54, 57, 66, 67, 71, 80 (w/X=0), and 89.
    // Type 74 was added 3/25/2020 for Satus Creek Project per project requirements with
    // concurrence from THE concrete specialist
    MaterialType material = m_pBarRecord->GetMaterial();
    if (material == MaterialType::D7957) // GFRP
    {
        int is_bend_type_supported = 1; // 0 = supported, 1 = unsupported, 2 = special case of unsupported for Type 80 bends
        switch (m_BendType)
        {
        case 50:
        case 51:
        case 54:
        case 57:
        case 66:
        case 67:
        case 71:
        case 74:
        case 89:
            is_bend_type_supported = 0;
            break;

        case 80:
            is_bend_type_supported = (m_X == 0.0 ? 0 : 2);
            break;

        default:
            is_bend_type_supported = 1;
        }

        if (0 < is_bend_type_supported)
        {
            SetStatusLevel(StatusType::stError);
            if (is_bend_type_supported == 1)
            {
                AddStatusMsg(_T("ERROR: Type %1 bend cannot be used for D7957 - GFRP bars"), static_cast<long>(m_BendType));
            }
            else
            {
                AddStatusMsg(_T("ERROR: Type 80 bend cannot be used for D7957 - GFRP bars unless X is 0"));
            }
        }
    }
}

void CBend::Clear()
{
    m_Components.clear();
}

void CBend::BuildBend()
{
    Clear();
}

void CBend::CheckNormalLength()
{
    const CBarData& barData = GetBarData();
    Float64 length = barData.GetNormalLength();

    if (length < m_Length)
    {
        SetStatusLevel(StatusType::stWarning);
        AddStatusMsg(_T("WARNING : Bar Length %1, Exceeds normal bar length of %2"), m_Length, length);
    }
}

void CBend::CheckMaxLength()
{
    const CBarData& barData = GetBarData();
    Float64 maxlength = barData.GetMaxLength();

    if (0 < maxlength && maxlength < m_Length)
    {
        SetStatusLevel(StatusType::stError);
        AddStatusMsg(_T("ERROR : Bar Length %1, Exceeds maximum bar length of %2"), m_Length, maxlength);
    }
}

void CBend::PostValidateBend()
{
    CheckNormalLength();
    CheckMaxLength();
}

void CBend::AddBarComponent(std::unique_ptr<CBarComponent> pComponent)
{
    m_Components.push_back(std::move(pComponent));
}

const CBarData& CBend::GetBarData() const
{
    return *m_pBarRecord->GetBarData();
}

UseType CBend::GetUseType() const
{
    return m_pBarRecord->GetUse();
}

StatusType CBend::GetStatusLevel() const
{
    return m_StatusMgr.GetStatusLevel();
}

void CBend::ResetStatusMsgs()
{
    m_StatusMgr.ResetStatusMsgs();
}

void CBend::ComputeLength()
{
    if (m_bIsDirty)
    {
        ResetStatusMsgs();

        m_Length = 0;

        try
        {
            PreValidateBend();
            BuildBend();

            for (const auto& pComponent : m_Components)
            {
                m_Length += pComponent->Length();
            }
        }
        catch (const CBarException& e)
        {
            SetStatusLevel(StatusType::stError);
            AddStatusMsg(e.GetReason());
        }

        m_bIsDirty = false;

        PostValidateBend();
    }
}
