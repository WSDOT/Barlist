///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright � 1999-2026  Washington State Department of Transportation
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


// Type67.cpp : Implementation of CType67
#include "Type67.h"
#include <tchar.h>
#include "BarData.h"
#include "LineComponent.h"
#include "HelixComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType67
void CType67::BuildBend()
{
   CBend::BuildBend();

   if ( GetStatusLevel() == StatusType::stError )
      return;

   const CBarData& barData = GetBarData();

   UseType use = GetUseType();

   // Error check data
   if ( use == UseType::utTransverse )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : Bend cannot be designated as Transverse"));
   }

   if ( use == UseType::utSeismic )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : Bend cannot be designated as Seismic"));
   }

   if ( GetStatusLevel() == StatusType::stError )
      return;

   // Build the bend
   Float64 db;
   Float64 radius;
   Float64 nTurns;
   Float64 pitch;

   db = barData.GetDiameter();
   radius = (GetU() - db)/2;
   pitch = GetX();
   nTurns = GetW()/GetX();

   auto pHelix = std::make_unique<CHelixComponent>( radius, nTurns, pitch );
   Float64 length = pHelix->Length();
   AddBarComponent( std::move(pHelix) );

   Float64 splice_length;
   if ( IsZero(GetZ()) )
      splice_length = 0;
   else
      splice_length = GetY() * length / GetZ();

   AddBarComponent( std::make_unique<CLineComponent>(splice_length) );
}

void CType67::PreValidateBend()
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

   if ( IsZero(GetZ()) && GetY() > 0 )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : %1 must be equal to %2"), _T("Y"), 0.00);
   }

   if ( GetX() >= GetW() )
   {
      SetStatusLevel( StatusType::stError );
      AddStatusMsg(_T("ERROR : X must be less than W"));
   }

}

void CType67::PostValidateBend()
{
   // NOTE: Don't call base class PostValidateBend().  We don't want
   //       the maximum lengths to be checked.
   //CBend::PostValidateBend();
}