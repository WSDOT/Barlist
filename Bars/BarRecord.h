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


// BarRecord.h : Declaration of CBarRecord (native, replaces the
// IBarRecord/IBendEvents ATL/COM coclass).

#pragma once

#include "BarsExport.h"
#include "Enums.h"
#include "Signal.h"
#include "StatusMgr.h"
#include <WBFLTypes.h>
#include <string>
#include <memory>

class CBend;
class CBarlist;
class CBarData;

class BARS_API CBarRecord
{
public:
    CBarRecord();
    ~CBarRecord();

    // Always referenced via shared_ptr<CBarRecord> -- not copyable (the
    // Signal subscription token below is only meaningful for the one
    // instance that registered it) and not needed to be movable either.
    CBarRecord(const CBarRecord&) = delete;
    CBarRecord& operator=(const CBarRecord&) = delete;

    // Non-owning back-reference, set by whichever CBarRecordCollection this
    // record is inserted into.
    void SetBarlist(CBarlist* pBarlist);

    MaterialType GetMaterial() const;
    void SetMaterial(MaterialType val);

    const std::_tstring& GetMark() const;
    void SetMark(std::_tstring val);

    const std::_tstring& GetLocation() const;
    void SetLocation(std::_tstring val);

    long GetNumReqd() const;
    void SetNumReqd(long val);

    UseType GetUse() const;
    void SetUse(UseType val);

    bool GetSubstructure() const;
    void SetSubstructure(bool val);

    bool GetEpoxy() const;
    void SetEpoxy(bool val);

    bool GetVaries() const;

    long GetNumEach() const;
    void SetNumEach(long val);

    std::shared_ptr<CBend> GetPrimaryBend() const;
    void SetPrimaryBend(std::shared_ptr<CBend> bend);

    std::shared_ptr<CBend> GetVariesBend() const;
    void SetVariesBend(std::shared_ptr<CBend> bend); // nullptr clears the varies bend

    long GetBendType() const;
    const std::_tstring& GetSize() const;
    StatusType GetStatus();
    Float64 GetMass();

    // Non-owning: points into one of CBarInfoMgr's process-lifetime static
    // bar-size tables.
    const CBarData* GetBarData() const;
    void SetBarData(const CBarData* barData);

    const CStatusMessageCollection& GetStatusMessages() const;

    Signal<CBarRecord&> OnBarRecordChanged;

private:
    void UpdateStatus();

    CBarlist* m_pBarlist = nullptr;

    MaterialType m_Material = MaterialType::A706_Grade60;
    std::_tstring m_Mark;
    std::_tstring m_Location;
    long m_NumEach = 1;
    long m_NumReqd = 1;
    bool m_bEpoxy = false;
    bool m_bSubstructure = false;
    UseType m_Use = UseType::utLongitudinal;

    std::shared_ptr<CBend> m_pPrimaryBend;
    std::shared_ptr<CBend> m_pVariesBend;
    Signal<>::Token m_PrimaryBendToken{};
    Signal<>::Token m_VariesBendToken{};

    const CBarData* m_pBarData = nullptr;

    CStatusMgr m_StatusMgr;
};
