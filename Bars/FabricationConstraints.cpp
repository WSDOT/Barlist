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


// FabricationConstraints.cpp: implementation of the CFabricationConstraints class.
//
//////////////////////////////////////////////////////////////////////

#include "FabricationConstraints.h"
#include "BarData.h"
#include "BarException.h"
#include <MathEx.h>
#include <tchar.h>

namespace
{
    // Replaces all occurances of strKey with strValue
    void format_msg(std::_tstring& strMsg, const std::_tstring& strKey, const std::_tstring& strValue)
    {
        std::_tstring::size_type pos = 0;
        while ((pos = strMsg.find(strKey)) != std::_tstring::npos)
        {
            strMsg.replace(strMsg.begin() + pos, strMsg.begin() + pos + strKey.size(),
                strValue.begin(), strValue.end());
        }
    }
}

Float64 CFabricationConstraints::GetBendDeduction(Float64 OR, Float64 angle)
{
    Float64 deduction = 0;
    if (IsEqual(angle, M_PI))
        deduction = OR;
    else
        deduction = OR * tan(angle / 2);

    return deduction;
}

Float64 CFabricationConstraints::GetBendDeduction(const CBarData& barData, UseType use)
{
    Float64 OR = GetOutsideBendRadius(barData, use);
    return GetBendDeduction(OR, PI_OVER_2);
}

Float64 CFabricationConstraints::GetHookDeduction(const CBarData& barData, UseType use, HookType hook)
{
    Float64 OR = GetOutsideBendRadius(barData, use);
    Float64 angle = (hook == HookType::ht90 || hook == HookType::ht135) ? PI_OVER_2 : M_PI;

    return GetBendDeduction(OR, angle);
}

Float64 CFabricationConstraints::GetHookRadius(const CBarData& barData, UseType use)
{
    Float64 CR = GetCenterlineBendRadius(barData, use);
    return CR;
}

Float64 CFabricationConstraints::GetTailLength(const CBarData& barData, UseType use, HookType hook)
{
    Float64 db = barData.GetDiameter();

    const CHookData* pHookData = barData.GetHookData(use, hook);

    if (pHookData == nullptr)
    {
        std::_tstring strMsg(_T("ERROR : Bend cannot be designated %use.  %use %hook hooks are not defined per LRFD %spec and/or BDM 5.1.2"));
        std::_tstring strUse(use == UseType::utLongitudinal ? _T("Longitudinal") :
                            use == UseType::utTransverse   ? _T("Transverse")   :
                         /* use == UseType::utSeismic */     _T("Seismic"));
        std::_tstring strHook(hook == HookType::ht90   ?   _T("90\xB0") :
                             hook == HookType::ht135  ?  _T("135\xB0") :
                          /* hook == HookType::ht180 */  _T("180\xB0"));
        std::_tstring strSpec(use == UseType::utSeismic ? _T("5.10.2.2") : _T("5.10.2.1"));

        format_msg(strMsg, std::_tstring(_T("%use")), strUse);
        format_msg(strMsg, std::_tstring(_T("%hook")), strHook);
        format_msg(strMsg, std::_tstring(_T("%spec")), strSpec);

        throw CBarException(strMsg.c_str());
    }

    Float64 T = pHookData->GetTail();
    Float64 Tmin = pHookData->GetMinTail();

    Float64 length = T * db;
    if (length < Tmin)
    {
        length = Tmin;
    }

    return length;
}

Float64 CFabricationConstraints::GetInsideBendDiameter(const CBarData& barData, UseType use)
{
    Float64 db = barData.GetDiameter();

    const CBendData* pBendData = barData.GetBendData(use);

    if (pBendData == nullptr)
    {
        std::_tstring strMsg(_T("ERROR : Bend cannot be designated %use.  %use bends are not defined per LRFD 5.10.2.3"));
        std::_tstring strUse(use == UseType::utLongitudinal ? _T("Longitudinal") :
                            use == UseType::utTransverse   ? _T("Transverse")   :
                         /* use == UseType::utSeismic */     _T("Seismic"));

        format_msg(strMsg, std::_tstring(_T("%use")), strUse);

        throw CBarException(strMsg.c_str());
    }

    Float64 id = pBendData->GetInsideDiameter();
    BendMeasureType bendMeasure = pBendData->GetBendMeasure();

    Float64 ID;
    if (bendMeasure == BendMeasureType::BarDiameter)
    {
        // this is ID/db
        ID = db * id;
    }
    else
    {
        ID = id;
    }

    return ID;
}

Float64 CFabricationConstraints::GetOutsideBendDiameter(const CBarData& barData, UseType use)
{
    Float64 db = barData.GetDiameter();

    Float64 ID = GetInsideBendDiameter(barData, use);
    Float64 OD = ID + 2 * db;

    return OD;
}

Float64 CFabricationConstraints::GetCenterlineBendDiameter(const CBarData& barData, UseType use)
{
    Float64 db = barData.GetDiameter();

    Float64 ID = GetInsideBendDiameter(barData, use);
    Float64 CD = ID + db;

    return CD;
}

Float64 CFabricationConstraints::GetInsideBendRadius(const CBarData& barData, UseType use)
{
    Float64 ID = GetInsideBendDiameter(barData, use);
    return ID / 2;
}

Float64 CFabricationConstraints::GetOutsideBendRadius(const CBarData& barData, UseType use)
{
    Float64 OD = GetOutsideBendDiameter(barData, use);
    return OD / 2;
}

Float64 CFabricationConstraints::GetCenterlineBendRadius(const CBarData& barData, UseType use)
{
    Float64 CD = GetCenterlineBendDiameter(barData, use);
    return CD / 2;
}
