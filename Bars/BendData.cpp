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


// BendData.cpp : Implementation of CBendData

#include "BendData.h"

void CBendData::SetData(BendMeasureType bendMeasure, Float64 ID, UseType use)
{
    m_BendMeasure = bendMeasure;
    m_ID = ID;
    m_Use = use;
}

UseType CBendData::GetUse() const
{
    return m_Use;
}

Float64 CBendData::GetInsideDiameter() const
{
    return m_ID;
}

BendMeasureType CBendData::GetBendMeasure() const
{
    return m_BendMeasure;
}
