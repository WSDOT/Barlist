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


// Type71.cpp : Implementation of CType71
#include "Type71.h"
#include <tchar.h>
#include "BarData.h"
#include "LineComponent.h"
#include "HelixComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType71
void CType71::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   Float64 db;
   Float64 radius;
   Float64 nTurns;

   db = barData.GetDiameter();
   radius = (GetU() - db)/2.0;
   nTurns = 0.5;

   // Error check data

   // Build the bend
   AddBarComponent( std::make_unique<CHelixComponent>(radius,nTurns) );
   AddBarComponent( std::make_unique<CLineComponent>(GetW()-GetU()/2) );
   AddBarComponent( std::make_unique<CLineComponent>(GetX()-GetU()/2) );
}

void CType71::PreValidateBend()
{
   CBend::PreValidateBend();

   if ( GetW() <= 0)
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("W"), 0.00);
   }

   if ( GetX() <= 0)
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("X"), 0.00);
   }
}
