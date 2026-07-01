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


// Type66.cpp : Implementation of CType66
#include "Type66.h"
#include <tchar.h>
#include "BarData.h"
#include "LineComponent.h"
#include "HelixComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType66
void CType66::BuildBend()
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

   db = barData.GetDiameter();
   radius = (GetU() - db)/2;
   nTurns = 1.0 + GetW()/(radius*TWO_PI);

   AddBarComponent( std::make_unique<CHelixComponent>( radius, nTurns ) );
}

void CType66::PreValidateBend()
{
   CBend::PreValidateBend();
}
