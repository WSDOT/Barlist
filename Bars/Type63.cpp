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


// Type63.cpp : Implementation of CType63
#include "Type63.h"
#include <tchar.h>
#include "LineComponent.h"
#include "HookComponent.h"
#include "BendComponent.h"
#include "FabricationConstraints.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType63
void CType63::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   Float64 deduct90; // deduction for 90deg bend
   Float64 deductWY; // deduction for bend WY
   Float64 deductUW; // dediction for bend UW
   Float64 deductUX; // deduction for bend UX
   Float64 deductXZ; // deduction for bend XZ;
   Float64 oRadius;  // Outside bend radius
   Float64 cRadius;  // Centerline bend radius
   Float64 u, w, x, y, z; // adjusted dimensions

   oRadius = CFabricationConstraints::GetOutsideBendRadius( barData, use );
   cRadius = CFabricationConstraints::GetCenterlineBendRadius( barData, use );
   deduct90 = CFabricationConstraints::GetBendDeduction( oRadius, PI_OVER_2 );

   deductWY = IsZero(GetY()) ? 0.0 : deduct90;
   deductUW = IsZero(GetW()) ? 0.0 : deduct90;
   deductUX = IsZero(GetX()) ? 0.0 : deduct90;
   deductXZ = IsZero(GetZ()) ? 0.0 : deduct90;

   u = GetU() - deductUW - deductUX;
   w = GetW() - deductWY - deductUW;
   x = GetX() - deductUX - deductXZ;
   y = GetY() - deductWY;
   z = GetZ() - deductXZ;

   // Error check data
   if ( y < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("Y"), deductWY);
   }

   if ( w < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("W"), deductWY+deductUW);
   }

   if ( u < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("U"), deductUW+deductUX);
   }

   if ( x < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("X"), deductUX+deductXZ);
   }

   if ( z < 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be greater than %2"), _T("Z"), deductXZ);
   }

   // If there is an error, don't build the bend
   if ( GetStatusLevel() == StatusType::stError )
      return;

   // Build the bend
   AddBarComponent( std::make_unique<CLineComponent>( u ) );
   AddBarComponent( std::make_unique<CLineComponent>( w ) );
   AddBarComponent( std::make_unique<CLineComponent>( x ) );
   AddBarComponent( std::make_unique<CLineComponent>( y ) );
   AddBarComponent( std::make_unique<CLineComponent>( z ) );

   if ( !IsZero(GetY()) && !IsZero(GetW()) )
      AddBarComponent( std::make_unique<CBend90>(cRadius) );

   if ( !IsZero(GetW()) && !IsZero(GetU()) )
      AddBarComponent( std::make_unique<CBend90>(cRadius) );

   if ( !IsZero(GetU()) && !IsZero(GetX()) )
      AddBarComponent( std::make_unique<CBend90>(cRadius) );

   if ( !IsZero(GetX()) && !IsZero(GetZ()) )
      AddBarComponent( std::make_unique<CBend90>(cRadius) );
}

void CType63::PreValidateBend()
{
   CBend::PreValidateBend();

   bool bCase1 =  IsZero(GetW()) &&  IsZero(GetX()) &&  IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase2 = !IsZero(GetW()) && !IsZero(GetX()) &&  IsZero(GetY()) && !IsZero(GetZ());
   bool bCase3 =  IsZero(GetW()) && !IsZero(GetX()) &&  IsZero(GetY()) && !IsZero(GetZ());
   bool bCase4 =  IsZero(GetW()) && !IsZero(GetX()) &&  IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase5 = !IsZero(GetW()) && !IsZero(GetX()) && !IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase6 = !IsZero(GetW()) &&  IsZero(GetX()) && !IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase7 = !IsZero(GetW()) &&  IsZero(GetX()) &&  IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase8 = !IsZero(GetW()) && !IsZero(GetX()) &&  IsZero(GetY()) &&  IsZero(GetZ());
   bool bCase9 = !IsZero(GetW()) && !IsZero(GetX()) && !IsZero(GetY()) && !IsZero(GetZ());

   if (!bCase1 && !bCase2 && !bCase3 && !bCase4 && !bCase5 && !bCase6 && !bCase7 && !bCase8 && !bCase9)
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : Dimensions are inconsistant"));
   }

}
