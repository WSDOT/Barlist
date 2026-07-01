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


// Type75.cpp : Implementation of CType75
#include "Type75.h"
#include <tchar.h>
#include "FabricationConstraints.h"
#include "LineComponent.h"
#include "BendComponent.h"
#include "HookComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType75
void CType75::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   // Error check data

   // Build the bend
   Float64 oRadius; // outside radius
   Float64 cRadius; // centerline radius
   Float64 hRadius; // radius of hook
   Float64 tail;
   Float64 deduct90_1;
   Float64 deduct90_2;
   Float64 deductHook_1;
   Float64 deductHook_2;
   Float64 u, w, x; // reduced dimensions (wx is the hypotenous)

   oRadius = CFabricationConstraints::GetOutsideBendRadius( barData, use );
   cRadius = CFabricationConstraints::GetCenterlineBendRadius( barData, use );

   deduct90_1 = IsZero(GetW()) ? 0.00 : CFabricationConstraints::GetBendDeduction( oRadius, PI_OVER_2 );
   deduct90_2 = IsZero(GetX()) ? 0.00 : CFabricationConstraints::GetBendDeduction( oRadius, PI_OVER_2 );

   hRadius = CFabricationConstraints::GetHookRadius( barData, use );
   tail = CFabricationConstraints::GetTailLength( barData, use, HookType::ht180 );
   deductHook_1 = IsZero(GetW()) ? 0.00 : CFabricationConstraints::GetHookDeduction( barData, use, HookType::ht180 );
   deductHook_2 = IsZero(GetX()) ? 0.00 : CFabricationConstraints::GetHookDeduction( barData, use, HookType::ht180 );

   u = GetU() - deduct90_1 - deduct90_2;
   w = IsZero(GetW()) ? 0.00 : GetW() - deduct90_1 - deductHook_1;
   x = IsZero(GetX()) ? 0.00 : GetX() - deduct90_2 - deductHook_2;

   // Some additional error checking
   if ( u < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("U"), deduct90_1 + deduct90_2);
   }

   if ( w < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("W"), deduct90_1+deductHook_1);
   }

   if ( x < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("X"), deduct90_2+deductHook_2);
   }

   if ( GetStatusLevel() == StatusType::stError )
      return;

   // Assemble the bend components
   AddBarComponent( std::make_unique<CLineComponent>(u) );
   AddBarComponent( std::make_unique<CLineComponent>(w) );
   AddBarComponent( std::make_unique<CLineComponent>(x) );

   if ( !IsZero(GetW()) )
   {
      AddBarComponent( std::make_unique<CBend90>(cRadius) );
      AddBarComponent( std::make_unique<CHook180>(hRadius,tail) );
   }

   if ( !IsZero(GetX()) )
   {
      AddBarComponent( std::make_unique<CBend90>(cRadius) );
      AddBarComponent( std::make_unique<CHook180>(hRadius,tail) );
   }

}

void CType75::PreValidateBend()
{
   CBend::PreValidateBend();

   bool bCase1 =  IsZero(GetW()) &&  IsZero(GetX());
   bool bCase2 = !IsZero(GetW()) &&  IsZero(GetX());
   bool bCase3 =  IsZero(GetW()) && !IsZero(GetX());
   bool bCase4 = !IsZero(GetW()) && !IsZero(GetX());

   if (!bCase1 && !bCase2 && !bCase3 && !bCase4)
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : Dimensions are inconsistant"));
   }
}
