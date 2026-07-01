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


// BarInfoMgr.cpp : Implementation of CBarInfoMgr

#include "BarInfoMgr.h"

CBarInfoMgr::CBarInfoMgr()
{
    m_SteelBars.InitSteelBars();
    m_GalvanizedBars.InitGalvanizedBars();
    m_GFRPBars.InitGFRPBars();
}

const CBarCollection& CBarInfoMgr::GetBars(MaterialType material) const
{
    if (material == MaterialType::D7957)
    {
        return m_GFRPBars;
    }
    else if (material == MaterialType::A767_A1094_Grade60 || material == MaterialType::A767_A1094_Grade80 || material == MaterialType::A767_A1094_Grade100)
    {
        return m_GalvanizedBars;
    }
    else
    {
        return m_SteelBars;
    }
}
