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


// Type80.cpp : Implementation of CType80
#include "Type80.h"
#include <tchar.h>
#include "BarData.h"
#include "FabricationConstraints.h"
#include "LineComponent.h"
#include "BendComponent.h"
#include "HookComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType80
void CType80::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   // Error check data

   // Build the bend
   Float64 db;
   Float64 bendDeduct1;
   Float64 bendDeduct2;
   Float64 u, w, x, y, z, t1, t2;

   db = barData.GetDiameter();

   t1 = fabs(M_PI - GetT1());
   t2 = fabs(M_PI - GetT2());

   bendDeduct1 = IsZero(GetW()) ? 0.00 : CFabricationConstraints::GetBendDeduction( GetY(), t1 );
   bendDeduct2 = IsZero(GetX()) ? 0.00 : CFabricationConstraints::GetBendDeduction( GetZ(), t2 );

   u = GetU() - bendDeduct1 - bendDeduct2;
   w = IsZero(GetW()) ? 0.00 : GetW() - bendDeduct1;
   x = IsZero(GetX()) ? 0.00 : GetX() - bendDeduct2;
   y = IsZero(GetY()) ? 0.00 : GetY() - db/2.0;
   z = IsZero(GetZ()) ? 0.00 : GetZ() - db/2.0;

   // Check minimum bend radius
   Float64 rMin = CFabricationConstraints::GetOutsideBendRadius( barData, use );
   if ( !IsZero(GetW()) && !IsZero(GetT1()) && (GetY() < rMin-0.01) )
   {
      SetStatusLevel( StatusType::stWarning );
      AddStatusMsg(_T("WARNING : %1 is less then the minimum bend radius of %2"), _T("Y"), rMin);
   }

   if ( !IsZero(GetX()) && !IsZero(GetT2()) && (GetZ() < rMin-0.01) )
   {
      SetStatusLevel( StatusType::stWarning );
      AddStatusMsg(_T("WARNING : %1 is less then the minimum bend radius of %2"), _T("Z"), rMin);
   }

   // Some additional error checking
   if ( u < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("U"), bendDeduct1+bendDeduct2);
   }

   if ( w < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("W"), bendDeduct1);
   }

   if ( x < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("X"), bendDeduct2);
   }

   if ( GetStatusLevel() == StatusType::stError )
      return;

   // Assemble the bend components
   AddBarComponent( std::make_unique<CLineComponent>(u) );
   AddBarComponent( std::make_unique<CLineComponent>(w) );
   AddBarComponent( std::make_unique<CLineComponent>(x) );

   if( !IsZero(GetW()) )
   {
      AddBarComponent( std::make_unique<CBendComponent>(y,t1) );
   }

   if( !IsZero(GetX()) )
   {
      AddBarComponent( std::make_unique<CBendComponent>(z,t2) );
   }
}

void CType80::PreValidateBend()
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
