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


// Type68.cpp : Implementation of CType68
#include "Type68.h"
#include <tchar.h>
#include "BarData.h"
#include "FabricationConstraints.h"
#include "LineComponent.h"
#include "BendComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType68
void CType68::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   // Error check data

   // Build the bend
   Float64 db;
   Float64 oRadius; // outside radius
   Float64 cRadius; // centerline radius
   Float64 deduct1;
   Float64 deduct2;
   Float64 u, w, x, y; // reduced dimensions

   db = barData.GetDiameter();
   oRadius = CFabricationConstraints::GetOutsideBendRadius( barData, use );
   cRadius = CFabricationConstraints::GetCenterlineBendRadius( barData, use );

   deduct1 = IsZero(GetW()) ? 0.00 : CFabricationConstraints::GetBendDeduction( oRadius, PI_OVER_2 );

   x = IsZero(GetX()) ? 0.00 : GetX() - db;
   deduct2 = IsZero(GetX()) ? 0.00 : CFabricationConstraints::GetBendDeduction( x/2, M_PI );

   u = IsZero(GetU()) ? 0.00 : GetU() - deduct1;
   w = IsZero(GetW()) ? 0.00 : GetW() - deduct1 - deduct2;
   y = IsZero(GetY()) ? 0.00 : GetY() - deduct2;

   // Some additional error checking
   if ( u < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("U"), deduct1);
   }

   if ( w < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("W"), deduct1+deduct2);
   }

   if ( y < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("Y"), deduct2);
   }

   if ( GetStatusLevel() == StatusType::stError )
      return;

   // Assemble the bend components
   AddBarComponent( std::make_unique<CLineComponent>(u) );
   AddBarComponent( std::make_unique<CLineComponent>(w) );
   AddBarComponent( std::make_unique<CLineComponent>(y) );

   if ( !IsZero(GetW()) )
      AddBarComponent( std::make_unique<CBend90>(cRadius) );

   AddBarComponent( std::make_unique<CBendComponent>( x/2, M_PI ) );
}

void CType68::PreValidateBend()
{
   // NOTE: Don't call base-class PreValidateBend().  U = 0 is
   //       valid for this bend!!!
   //CBend::PreValidateBend();

   bool bCase1 = !IsZero(GetU()) &&  IsZero(GetW()) &&  IsZero(GetX()) &&  IsZero(GetY());
   bool bCase2 = !IsZero(GetU()) && !IsZero(GetW()) &&  IsZero(GetX()) &&  IsZero(GetY());
   bool bCase3 = !IsZero(GetU()) && !IsZero(GetW()) && !IsZero(GetX()) && !IsZero(GetY());
   bool bCase4 =  IsZero(GetU()) &&  IsZero(GetW()) &&  IsZero(GetX()) &&  IsZero(GetY());
   bool bCase5 =  IsZero(GetU()) && !IsZero(GetW()) &&  IsZero(GetX()) &&  IsZero(GetY());
   bool bCase6 =  IsZero(GetU()) && !IsZero(GetW()) && !IsZero(GetX()) && !IsZero(GetY());

   if (!bCase1 && !bCase2 && !bCase3 && !bCase4 && !bCase5 && !bCase6)
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : Dimensions are inconsistant"));
   }

   if (IsZero(GetX()) && !IsZero(GetY()))
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be zero if %2 is zero"), _T("Y"), _T("X"));
   }
   else if (!IsZero(GetX()) && IsZero(GetY()))
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be zero if %2 is zero"), _T("X"), _T("Y"));
   }
   else if (GetY() < GetX()/2.0)
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("Y"), _T("X/2"));
   }
}
