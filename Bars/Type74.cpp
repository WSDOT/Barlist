///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2020  Washington State Department of Transportation
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


// Type74.cpp : Implementation of CType74
#include "stdafx.h"
#include "Bars.h"
#include "Type74.h"
#include "FabricationConstraints.h"
#include "LineComponent.h"
#include "BendComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType74
void CType74::BuildBend()
{
   CBendImpl<CType74,&CLSID_Type74>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   GetBarData(&pBarData);

   UseType use = GetUseType();

   Float64 oRadius;
   Float64 cRadius;
   Float64 deduct1;
   Float64 deduct2;
   Float64 u, w, x;

   oRadius = CFabricationConstraints::GetOutsideBendRadius(pBarData,use);
   cRadius = CFabricationConstraints::GetCenterlineBendRadius(pBarData,use);
   deduct1 = IsZero(GetW()) ? 0.00 : CFabricationConstraints::GetBendDeduction( oRadius, PI_OVER_2 );
   deduct2 = IsZero(GetX()) ? 0.00 : CFabricationConstraints::GetBendDeduction( oRadius, PI_OVER_2 );

   u = GetU() - deduct1 - deduct2;
   w = IsZero(GetW()) ? 0.00 : GetW() - deduct1;
   x = IsZero(GetX()) ? 0.00 : GetX() - deduct2;

   // Error check data
   if ( u < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("U"), CComVariant(deduct1 + deduct2) );
   }

   if ( w < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("W"), CComVariant(deduct1) );
   }

   if ( x < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("X"), CComVariant(deduct2) );
   }

   if ( GetStatusLevel() == stError )
      return;

   // Build the bend
   AddBarComponent( new CLineComponent(u) );

   if ( !IsZero(GetW()) )
   {
      AddBarComponent( new CLineComponent(w) );
      AddBarComponent( new CBend90(cRadius) );
   }

   if ( !IsZero(GetX()) )
   {
      AddBarComponent( new CLineComponent(x) );
      AddBarComponent( new CBend90(cRadius) );
   }
}

void CType74::PreValidateBend()
{
   CBendImpl<CType74,&CLSID_Type74>::PreValidateBend();

   bool bCase1 =  IsZero(GetW()) &&  IsZero(GetX());
   bool bCase2 = !IsZero(GetW()) &&  IsZero(GetX());
   bool bCase3 =  IsZero(GetW()) && !IsZero(GetX());
   bool bCase4 = !IsZero(GetW()) && !IsZero(GetX());

   if (!bCase1 && !bCase2 && !bCase3 && !bCase4)
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_ILLFORMEDBEND );
      AddStatusMsg( msg, CComVariant(), CComVariant() );
   }
}
