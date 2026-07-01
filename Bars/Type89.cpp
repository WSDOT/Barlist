///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2026  Washington State Department of Transportation
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


// Type89.cpp : Implementation of CType89
#include "Type89.h"
#include <tchar.h>
#include "BarData.h"
#include "FabricationConstraints.h"
#include "LineComponent.h"
#include "BendComponent.h"
#include "HookComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType89
void CType89::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   // Error check data

   // Build the bend
   long nSplices;
   Float64 barLength;
   Float64 totalSpliceLength;

   barLength = barData.GetNormalLength();
   nSplices = (long)(GetU() / barLength);
   totalSpliceLength = nSplices*GetW();

   // Assemble the bend components
   AddBarComponent( std::make_unique<CLineComponent>(GetU()) );
   AddBarComponent( std::make_unique<CLineComponent>(totalSpliceLength) );
}

void CType89::PostValidateBend()
{
   // NOTE: Don't call base class PostValidateBend().  We don't want
   //       the maximum lengths to be checked.
   //CBend::PostValidateBend();
}
