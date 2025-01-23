///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2025  Washington State Department of Transportation
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
#include "stdafx.h"
#include "Bars.h"
#include "Type60.h"
#include "LineComponent.h"
#include "HookComponent.h"
#include "BendComponent.h"
#include "FabricationConstraints.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType60
void CType60::BuildBend()
{
   CBendImpl<CType60,&CLSID_Type60>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   GetBarData(&pBarData);

   UseType use = GetUseType();

   Float64 radius  = CFabricationConstraints::GetOutsideBendRadius( pBarData, use );
   Float64 deduct  = CFabricationConstraints::GetBendDeduction( radius, PI_OVER_2 );

   // Error check data
   if ( (GetX() - deduct) < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg,
                    CComVariant("X"),
                    CComVariant(deduct));

      return;
   }

   Float64 rHook = CFabricationConstraints::GetHookRadius( pBarData, use );
   Float64 tail  = CFabricationConstraints::GetTailLength( pBarData, use, ht90 );
   if ( GetX() < (rHook+tail) )
   {
      SetStatusLevel( stWarning );
      CComBSTR msg;
      msg.LoadString( WARN_DIMLESSTHANHOOK );
      AddStatusMsg( msg,
                    CComVariant("X"),
                    CComVariant(rHook+tail));

      return;
   }

   if ( GetU() - 2*deduct < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg,
                    CComVariant("U"),
                    CComVariant(2*deduct));

      return;
   }

   if ( GetW() - 2*deduct < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg,
                    CComVariant("W"),
                    CComVariant(2*deduct));

      return;
   }

   // Build bend
   AddBarComponent( new CLineComponent( GetU() - 2*deduct ) );
   AddBarComponent( new CLineComponent( GetU() - 2*deduct ) );
   AddBarComponent( new CLineComponent( GetW() - 2*deduct ) );
   AddBarComponent( new CLineComponent( GetW() - 2*deduct ) );
   AddBarComponent( new CLineComponent( GetX() -   deduct ) );
   AddBarComponent( new CLineComponent( GetX() -   deduct ) );
   AddBarComponent( new CBend90(radius) );
   AddBarComponent( new CBend90(radius) );
   AddBarComponent( new CBend90(radius) );
   AddBarComponent( new CBend90(radius) );
   AddBarComponent( new CBend90(radius) );
}

void CType60::PreValidateBend()
{
   CBendImpl<CType60,&CLSID_Type60>::PreValidateBend();

   if ( GetW() <= 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("W"), CComVariant(0.00) );
   }

   if ( GetX() <= 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("X"), CComVariant(0.00) );
   }
}
