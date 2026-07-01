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


// Type58.cpp : Implementation of CType58
#include "Type58.h"
#include <tchar.h>
#include "LineComponent.h"
#include "HookComponent.h"
#include "FabricationConstraints.h"

/////////////////////////////////////////////////////////////////////////////
// CType58
void CType58::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   Float64 deduct90 = CFabricationConstraints::GetHookDeduction(barData, use, HookType::ht90);
   Float64 deduct135 = CFabricationConstraints::GetHookDeduction(barData, use, HookType::ht135);
   Float64 radius = CFabricationConstraints::GetHookRadius(barData, use);
   Float64 tail90 = CFabricationConstraints::GetTailLength(barData, use, HookType::ht90);
   Float64 tail135 = CFabricationConstraints::GetTailLength(barData, use, HookType::ht135);

   // Error check data
   if ( (GetU() - (deduct90+deduct135)) < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("U"), deduct90+deduct135);

   }

   if ( GetStatusLevel() == StatusType::stError )
      return;

   // Build bend
   AddBarComponent( std::make_unique<CLineComponent>( GetU() - (deduct90+deduct135) ) );
   AddBarComponent( std::make_unique<CHook135>(radius,tail135) );
   AddBarComponent( std::make_unique<CHook90>(radius,tail90) );
}
