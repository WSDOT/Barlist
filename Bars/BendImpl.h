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


// BendImpl.h: declaration of CBend (native, replaces the CBendImpl<T,pclsid>
// ATL template base -- bend type is now a plain constructor argument
// instead of a CLSID template parameter, so this is a normal (non-template)
// polymorphic base class for the 46 TypeNN bend classes).

#pragma once

#include "BarsExport.h"
#include "Enums.h"
#include "Signal.h"
#include "StatusMgr.h"
#include <WBFLTypes.h>
#include <vector>
#include <memory>

class CBarComponent;
class CBarRecord;
class CBarData;

class BARS_API CBend
{
public:
    CBend(long bendType,
          bool bU,
          bool bW = false,
          bool bX = false,
          bool bY = false,
          bool bZ = false,
          bool bT1 = false,
          bool bT2 = false);
    virtual ~CBend();

    // Always referenced via shared_ptr<CBend> -- not copyable (m_Components
    // owns its CBarComponents via unique_ptr).
    CBend(const CBend&) = delete;
    CBend& operator=(const CBend&) = delete;

    // Non-owning back-reference -- CBarRecord owns its CBend(s) via shared_ptr,
    // this is the reverse edge, same raw-pointer relationship as before.
    CBarRecord* GetBarRecord() const;
    void SetBarRecord(CBarRecord* pBarRecord);

    Float64 GetU() const;
    void SetU(Float64 val);
    Float64 GetW() const;
    void SetW(Float64 val);
    Float64 GetX() const;
    void SetX(Float64 val);
    Float64 GetY() const;
    void SetY(Float64 val);
    Float64 GetZ() const;
    void SetZ(Float64 val);
    Float64 GetT1() const;
    void SetT1(Float64 val);
    Float64 GetT2() const;
    void SetT2(Float64 val);

    StatusType GetStatus(); // forces ComputeLength()
    const CStatusMessageCollection& GetStatusMessages();
    Float64 GetLength(); // lazy: forces ComputeLength() if dirty
    long GetBendType() const;
    bool SupportsDimension(DimensionType type) const; // throws std::invalid_argument for an out-of-range type

    Signal<> OnBendChanged;

protected:
    bool m_bU;
    bool m_bW;
    bool m_bX;
    bool m_bY;
    bool m_bZ;
    bool m_bT1;
    bool m_bT2;

    void SetStatusLevel(StatusType status);
    void AddStatusMsg(std::_tstring msg, StatusValue v1 = {}, StatusValue v2 = {});

    virtual void PreValidateBend();
    virtual void BuildBend();
    virtual void PostValidateBend();

    void AddBarComponent(std::unique_ptr<CBarComponent> pComponent); // takes ownership
    const CBarData& GetBarData() const;
    UseType GetUseType() const;
    StatusType GetStatusLevel() const;

private:
    void Clear();
    void CheckNormalLength();
    void CheckMaxLength();
    void ResetStatusMsgs();
    void ComputeLength();

    CBarRecord* m_pBarRecord = nullptr;

    Float64 m_U = 0;
    Float64 m_W = 0;
    Float64 m_X = 0;
    Float64 m_Y = 0;
    Float64 m_Z = 0;
    Float64 m_T1 = 0;
    Float64 m_T2 = 0;

    long m_BendType;

    bool m_bIsDirty = true;
    Float64 m_Length = 0;

    std::vector<std::unique_ptr<CBarComponent>> m_Components;

    CStatusMgr m_StatusMgr;
};
