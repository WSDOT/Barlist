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


// Type69.cpp : Implementation of CType69
#include "Type69.h"
#include <tchar.h>
#include "BarData.h"
#include "FabricationConstraints.h"
#include "LineComponent.h"
#include "BendComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType69
void CType69::BuildBend()
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
   Float64 deduct;
   Float64 deduct90;
   Float64 angle;
   Float64 u, wx, y, z; // reduced dimensions (wx is the hypotenous)

   db = barData.GetDiameter();
   oRadius = CFabricationConstraints::GetOutsideBendRadius( barData, use );
   cRadius = CFabricationConstraints::GetCenterlineBendRadius( barData, use );
   angle = IsZero(GetX()) ? 0.00 : atan2(GetX(),GetW());

   deduct   = IsZero(GetX()) ? 0.00 : CFabricationConstraints::GetBendDeduction( oRadius, angle );
   deduct90 = IsZero(GetZ()) ? 0.00 : CFabricationConstraints::GetBendDeduction( oRadius, PI_OVER_2 );

   u = IsZero(GetU()) ? 0.00 : GetU() - deduct;
   wx = IsZero(GetX()) ? 0.00 : sqrt(GetW()*GetW() + GetX()*GetX()) - deduct - (IsZero(GetY()) ? 0.00 : deduct);
   y = IsZero(GetY()) ? 0.00 : GetY() - deduct - deduct90;
   z = IsZero(GetZ()) ? 0.00 : GetZ() - deduct90;

   // Some additional error checking
   if ( u < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("U"), deduct);
   }

   if ( wx < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("sqrt(W*W + X*X)"), deduct+deduct90);
   }

   if ( y < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("Y"), deduct+deduct90);
   }

   if ( z < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("Z"), deduct90);
   }

   if ( GetStatusLevel() == StatusType::stError )
      return;

   // Assemble the bend components
   AddBarComponent( std::make_unique<CLineComponent>(u) );
   AddBarComponent( std::make_unique<CLineComponent>(wx) );
   AddBarComponent( std::make_unique<CLineComponent>(y) );
   AddBarComponent( std::make_unique<CLineComponent>(z) );

   if ( wx > 0 )
      AddBarComponent( std::make_unique<CBendComponent>(cRadius,angle) );

   if ( y > 0 )
      AddBarComponent( std::make_unique<CBendComponent>(cRadius,angle) );

   if ( z > 0 )
      AddBarComponent( std::make_unique<CBend90>(cRadius) );
}

void CType69::PreValidateBend()
{
   // NOTE: Don't call base-class PreValidateBend().  U = 0 is
   //       valid for this bend!!!
   //CBend::PreValidateBend();

   bool bCase1 =  !IsZero(GetU()) && IsZero(GetW()) &&  IsZero(GetX()) &&  IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase2 =  !IsZero(GetU()) &&                   !IsZero(GetX()) &&  IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase3 =  !IsZero(GetU()) &&                   !IsZero(GetX()) && !IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase4 =  !IsZero(GetU()) &&                   !IsZero(GetX()) && !IsZero(GetY()) && !IsZero(GetZ());
   bool bCase5 =   IsZero(GetU()) && IsZero(GetW()) &&  IsZero(GetX()) &&  IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase6 =   IsZero(GetU()) &&                   !IsZero(GetX()) &&  IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase7 =   IsZero(GetU()) &&                   !IsZero(GetX()) && !IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase8 =   IsZero(GetU()) &&                   !IsZero(GetX()) && !IsZero(GetY()) && !IsZero(GetZ());

   if (!bCase1 && !bCase2 && !bCase3 && !bCase4 && !bCase5 && !bCase6 && !bCase7 && !bCase8)
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : Dimensions are inconsistant"));
   }

   if (!IsZero(GetZ()) && IsZero(GetY()))
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be zero if %2 is zero"), _T("Y"), _T("Z"));
   }
}
