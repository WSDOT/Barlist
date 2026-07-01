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


// BarRecord.cpp : Implementation of CBarRecord

#include "BarRecord.h"
#include "BendImpl.h"
#include "Barlist.h"
#include "BarData.h"
#include <unordered_set>
#include <tchar.h>

CBarRecord::CBarRecord()
#if defined(_DEBUG)
    : m_Mark(_T("XXXX"))
#else
    : m_Mark(_T(""))
#endif
{
}

CBarRecord::~CBarRecord()
{
    // Disconnect from our bends' OnBendChanged so a bend that outlives this
    // record (shared ownership, or simply reused by a caller for another
    // record afterward) doesn't hold a dangling lambda capturing `this`.
    if (m_pPrimaryBend)
    {
        m_pPrimaryBend->OnBendChanged.Disconnect(m_PrimaryBendToken);
    }
    if (m_pVariesBend)
    {
        m_pVariesBend->OnBendChanged.Disconnect(m_VariesBendToken);
    }
}

void CBarRecord::SetBarlist(CBarlist* pBarlist)
{
    m_pBarlist = pBarlist;
}

MaterialType CBarRecord::GetMaterial() const
{
    return m_Material;
}

void CBarRecord::SetMaterial(MaterialType val)
{
    m_Material = val;
    OnBarRecordChanged(*this);
}

const std::_tstring& CBarRecord::GetMark() const
{
    return m_Mark;
}

void CBarRecord::SetMark(std::_tstring val)
{
    if (m_pBarlist)
    {
        m_pBarlist->ReplaceMarkNumber(m_Mark, val);
    }

    m_Mark = std::move(val);
    OnBarRecordChanged(*this);
}

const std::_tstring& CBarRecord::GetLocation() const
{
    return m_Location;
}

void CBarRecord::SetLocation(std::_tstring val)
{
    m_Location = std::move(val);
    OnBarRecordChanged(*this);
}

long CBarRecord::GetNumReqd() const
{
    return m_NumReqd;
}

void CBarRecord::SetNumReqd(long val)
{
    m_NumReqd = val;
    m_StatusMgr.ResetStatusMsgs();
    OnBarRecordChanged(*this);
}

UseType CBarRecord::GetUse() const
{
    return m_Use;
}

void CBarRecord::SetUse(UseType val)
{
    m_Use = val;
    OnBarRecordChanged(*this);
}

bool CBarRecord::GetSubstructure() const
{
    return m_bSubstructure;
}

void CBarRecord::SetSubstructure(bool val)
{
    m_bSubstructure = val;
    OnBarRecordChanged(*this);
}

bool CBarRecord::GetEpoxy() const
{
    return m_bEpoxy;
}

void CBarRecord::SetEpoxy(bool val)
{
    m_bEpoxy = val;
    OnBarRecordChanged(*this);
}

bool CBarRecord::GetVaries() const
{
    return m_pVariesBend != nullptr;
}

long CBarRecord::GetNumEach() const
{
    return m_NumEach;
}

void CBarRecord::SetNumEach(long val)
{
    m_NumEach = val;
    m_StatusMgr.ResetStatusMsgs();
    OnBarRecordChanged(*this);
}

std::shared_ptr<CBend> CBarRecord::GetPrimaryBend() const
{
    return m_pPrimaryBend;
}

void CBarRecord::SetPrimaryBend(std::shared_ptr<CBend> bend)
{
    if (m_pPrimaryBend)
    {
        m_pPrimaryBend->OnBendChanged.Disconnect(m_PrimaryBendToken);
    }

    m_pPrimaryBend = std::move(bend);
    m_pPrimaryBend->SetBarRecord(this);
    m_PrimaryBendToken = m_pPrimaryBend->OnBendChanged.Connect([this] { OnBarRecordChanged(*this); });

    OnBarRecordChanged(*this);
}

std::shared_ptr<CBend> CBarRecord::GetVariesBend() const
{
    return m_pVariesBend;
}

void CBarRecord::SetVariesBend(std::shared_ptr<CBend> bend)
{
    if (m_pVariesBend)
    {
        m_pVariesBend->OnBendChanged.Disconnect(m_VariesBendToken);
    }

    if (!bend)
    {
        // Varies bend has been removed. Clear out status messages because
        // no-each/no-required problems can't exist without a varies bend.
        m_StatusMgr.ResetStatusMsgs();
        m_NumEach = 1;
        m_pVariesBend.reset();
        return;
    }

    m_pVariesBend = std::move(bend);
    m_pVariesBend->SetBarRecord(this);
    m_VariesBendToken = m_pVariesBend->OnBendChanged.Connect([this] { OnBarRecordChanged(*this); });

    OnBarRecordChanged(*this);
}

long CBarRecord::GetBendType() const
{
    return m_pPrimaryBend->GetBendType();
}

const std::_tstring& CBarRecord::GetSize() const
{
    return m_pBarData->GetName();
}

StatusType CBarRecord::GetStatus()
{
    UpdateStatus();

    StatusType recordStatus = m_StatusMgr.GetStatusLevel();
    StatusType primaryStatus = m_pPrimaryBend->GetStatus();
    StatusType variesStatus = StatusType::stOK;
    if (m_pVariesBend)
    {
        variesStatus = m_pVariesBend->GetStatus();
    }

    StatusType status = recordStatus;
    if (status < primaryStatus)
    {
        status = primaryStatus;
    }
    if (status < variesStatus)
    {
        status = variesStatus;
    }

    return status;
}

Float64 CBarRecord::GetMass()
{
    Float64 primaryLength = m_pPrimaryBend->GetLength();
    Float64 variesLength = m_pVariesBend ? m_pVariesBend->GetLength() : primaryLength;

    Float64 length = (primaryLength + variesLength) / 2;
    Float64 unitMass = m_pBarData->GetMass();
    Float64 mass = m_NumReqd * length * unitMass;

    // If status is error, don't figure the mass.
    StatusType status = GetStatus();

    return status == StatusType::stError ? 0.0 : mass;
}

const CBarData* CBarRecord::GetBarData() const
{
    return m_pBarData;
}

void CBarRecord::SetBarData(const CBarData* barData)
{
    m_pBarData = barData;
    OnBarRecordChanged(*this);
}

const CStatusMessageCollection& CBarRecord::GetStatusMessages() const
{
    return m_StatusMgr.GetStatusMessages();
}

void CBarRecord::UpdateStatus()
{
    m_StatusMgr.ResetStatusMsgs();

    // Check if the mark number is in use elsewhere
    if (m_pBarlist->BarRecordMultiUse(m_Mark))
    {
        m_StatusMgr.SetStatusLevel(StatusType::stWarning);
        m_StatusMgr.AddStatusMsg(_T("WARNING : Mark Number %1 is currently in use"), m_Mark);
    }

    // Check if bar is larger than #14 and Grade 75 or higher
    static const std::unordered_set<MaterialType> grade75andHigher = {
        MaterialType::A706_Grade80,
        MaterialType::A1035_Grade100,
        MaterialType::A1035_Grade120,
        MaterialType::A767_A1094_Grade80,
        MaterialType::A767_A1094_Grade100,
        MaterialType::A955_Grade75,
        MaterialType::A955_Grade80
    };

    if (grade75andHigher.find(m_Material) != grade75andHigher.end() && GetSize() == _T("#18"))
    {
        m_StatusMgr.SetStatusLevel(StatusType::stWarning);
        m_StatusMgr.AddStatusMsg(_T("WARNING: Due to safety concerns, CRSI does not recommend bending bars larger than #14 with grade designation of Grade 75 or higher"));
    }

    // Check if the number required is evenly divided by the number each,
    // if this is a varies bend record.
    if (GetVaries())
    {
        long remainder = m_NumReqd % m_NumEach;
        if (remainder != 0)
        {
            m_StatusMgr.SetStatusLevel(StatusType::stError);
            m_StatusMgr.AddStatusMsg(_T("ERROR : # Each must be an even multiple of Num Reqd"));
        }
    }
}
