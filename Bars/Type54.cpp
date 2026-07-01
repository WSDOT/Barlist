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


// Type54.cpp : Implementation of CType54
#include "Type54.h"
#include <tchar.h>
#include "LineComponent.h"
#include "HookComponent.h"
#include "FabricationConstraints.h"

/////////////////////////////////////////////////////////////////////////////
// CType54
void CType54::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   Float64 deduct = CFabricationConstraints::GetHookDeduction(barData, use, HookType::ht90);
   Float64 radius = CFabricationConstraints::GetHookRadius(barData, use);
   Float64 tail = CFabricationConstraints::GetTailLength(barData, use, HookType::ht90);

   // Error check data
   if ( (GetU() - deduct) < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("U"), deduct);

      return;
   }


   // Build bend
   AddBarComponent( std::make_unique<CLineComponent>( GetU() - deduct ) );
   AddBarComponent( std::make_unique<CHook90>(radius,tail) );
}
