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


// Type60.cpp : Implementation of CType60
#include "Type60.h"
#include <tchar.h>
#include "LineComponent.h"
#include "HookComponent.h"
#include "BendComponent.h"
#include "FabricationConstraints.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType60
void CType60::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   Float64 radius  = CFabricationConstraints::GetOutsideBendRadius( barData, use );
   Float64 deduct  = CFabricationConstraints::GetBendDeduction( radius, PI_OVER_2 );

   // Error check data
   if ( (GetX() - deduct) < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("X"), deduct);

      return;
   }

   Float64 rHook = CFabricationConstraints::GetHookRadius( barData, use );
   Float64 tail  = CFabricationConstraints::GetTailLength( barData, use, HookType::ht90 );
   if ( GetX() < (rHook+tail) )
   {
      SetStatusLevel( StatusType::stWarning );
      AddStatusMsg(_T("WARNING : %1 is smaller than standard hook %2"), _T("X"), rHook+tail);

      return;
   }

   if ( GetU() - 2*deduct < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("U"), 2*deduct);

      return;
   }

   if ( GetW() - 2*deduct < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("W"), 2*deduct);

      return;
   }

   // Build bend
   AddBarComponent( std::make_unique<CLineComponent>( GetU() - 2*deduct ) );
   AddBarComponent( std::make_unique<CLineComponent>( GetU() - 2*deduct ) );
   AddBarComponent( std::make_unique<CLineComponent>( GetW() - 2*deduct ) );
   AddBarComponent( std::make_unique<CLineComponent>( GetW() - 2*deduct ) );
   AddBarComponent( std::make_unique<CLineComponent>( GetX() -   deduct ) );
   AddBarComponent( std::make_unique<CLineComponent>( GetX() -   deduct ) );
   AddBarComponent( std::make_unique<CBend90>(radius) );
   AddBarComponent( std::make_unique<CBend90>(radius) );
   AddBarComponent( std::make_unique<CBend90>(radius) );
   AddBarComponent( std::make_unique<CBend90>(radius) );
   AddBarComponent( std::make_unique<CBend90>(radius) );
}

void CType60::PreValidateBend()
{
   CBend::PreValidateBend();

   if ( GetW() <= 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("W"), 0.00);
   }

   if ( GetX() <= 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("X"), 0.00);
   }
}
