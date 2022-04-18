///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2022  Washington State Department of Transportation
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


// Type83.cpp : Implementation of CType83
#include "stdafx.h"
#include "Bars.h"
#include "Type83.h"
#include "FabricationConstraints.h"
#include "LineComponent.h"
#include "BendComponent.h"
#include "HookComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType83
void CType83::BuildBend()
{
   CBendImpl<CType83,&CLSID_Type83>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   GetBarData(&pBarData);

   UseType use = GetUseType();

   // Error check data

   // Build the bend
   Float64 oRadius; // outside radius;
   Float64 cRadius; // centerline radius;
   Float64 hRadius; // radius of hook
   Float64 tail;
   Float64 deduct90;
   Float64 deduct135;
   Float64 u1, u2, w1, w2;

   oRadius   = CFabricationConstraints::GetOutsideBendRadius( pBarData, use );
   cRadius   = CFabricationConstraints::GetCenterlineBendRadius( pBarData, use );
   deduct90  = CFabricationConstraints::GetBendDeduction( oRadius, PI_OVER_2 );
   deduct135 = CFabricationConstraints::GetHookDeduction( pBarData, use, ht135 );
   hRadius   = CFabricationConstraints::GetHookRadius( pBarData, use );
   tail      = CFabricationConstraints::GetTailLength( pBarData, use, ht135 );

   u1 = GetU() - 2*deduct90;
   u2 = GetU() - deduct90 - deduct135;
   w1 = GetW() - 2*deduct90;
   w2 = GetW() - deduct90 - deduct135;

   // Some additional error checking
   if ( u1 < 0 || u2 < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("U"), CComVariant(max(2*deduct90,deduct90+deduct135)) );
   }

   if ( w1 < 0 || w2 < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("W"), CComVariant(max(2*deduct90,deduct90+deduct135)) );
   }

   if ( GetStatusLevel() == stError )
      return;

   // Assemble the bend components
   AddBarComponent( new CLineComponent(u1) );
   AddBarComponent( new CLineComponent(u2) );
   AddBarComponent( new CLineComponent(w1) );
   AddBarComponent( new CLineComponent(w2) );
   AddBarComponent( new CBend90(cRadius) );
   AddBarComponent( new CBend90(cRadius) );
   AddBarComponent( new CBend90(cRadius) );
   AddBarComponent( new CHook135(hRadius,tail) );
   AddBarComponent( new CHook135(hRadius,tail) );
}

void CType83::PreValidateBend()
{
   CBendImpl<CType83,&CLSID_Type83>::PreValidateBend();

   if ( GetW() <= 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("W"), CComVariant(0.00) );
   }
}
