///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright � 2009-2019, Washington State Department of Transportation
//                     Bridge and Structures Office
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


// Type81.cpp : Implementation of CType81
#include "stdafx.h"
#include "Bars.h"
#include "Type81.h"
#include "FabricationConstraints.h"
#include "LineComponent.h"
#include "BendComponent.h"
#include "HookComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType81
void CType81::BuildBend()
{
   CBendImpl<CType81,&CLSID_Type81>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   pBarData.Attach( GetBarData() );

   UseType use = GetUseType();

   // Error check data

   // Build the bend
   double db;
   double hRadius; // radius of hook
   double tail;
   double bendDeduct1;
   double bendDeduct2;
   double hookDeduct1;
   double hookDeduct2;
   double u, w, x, y, z, t1, t2;

   pBarData->get_Diameter( &db );

   t1 = fabs(M_PI - GetT1());
   t2 = fabs(M_PI - GetT2());

   bendDeduct1 = IsZero(GetW()) ? 0.00 : CFabricationConstraints::GetBendDeduction( GetY(), t1 );
   bendDeduct2 = IsZero(GetX()) ? 0.00 : CFabricationConstraints::GetBendDeduction( GetZ(), t2 );

   hRadius = CFabricationConstraints::GetHookRadius( pBarData, use );
   tail    = CFabricationConstraints::GetTailLength( pBarData, use, ht180 );
   hookDeduct1 = IsZero( GetW() ) ? 0.00 : CFabricationConstraints::GetHookDeduction( pBarData, use, ht180 );
   hookDeduct2 = IsZero( GetX() ) ? 0.00 : CFabricationConstraints::GetHookDeduction( pBarData, use, ht180 );

   u = GetU() - bendDeduct1 - bendDeduct2;
   w = IsZero(GetW()) ? 0.00 : GetW() - bendDeduct1 - hookDeduct1;
   x = IsZero(GetX()) ? 0.00 : GetX() - bendDeduct2 - hookDeduct2;
   y = IsZero(GetY()) ? 0.00 : GetY() - db/2.0;
   z = IsZero(GetZ()) ? 0.00 : GetZ() - db/2.0;

   // Some additional error checking
   // Check minimum bend radius
   double rMin = CFabricationConstraints::GetOutsideBendRadius( pBarData, use );
   if ( !IsZero(GetW()) && !IsZero(GetT1()) && (GetY() < rMin-0.01) )
   {
      SetStatusLevel( stWarning );
      CComBSTR msg;
      msg.LoadString( WARN_MINRADIUS );
      AddStatusMsg( msg, CComVariant("Y"), CComVariant(rMin) );
   }

   if ( !IsZero(GetX()) && !IsZero(GetT2()) && (GetZ() < rMin-0.01) )
   {
      SetStatusLevel( stWarning );
      CComBSTR msg;
      msg.LoadString( WARN_MINRADIUS );
      AddStatusMsg( msg, CComVariant("Z"), CComVariant(rMin) );
   }

   if ( u < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("U"), CComVariant(bendDeduct1+bendDeduct2) );
   }

   if ( w < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("W"), CComVariant(bendDeduct1+hookDeduct1) );
   }

   if ( x < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("X"), CComVariant(bendDeduct2+hookDeduct2) );
   }

   if ( GetStatusLevel() == stError )
      return;

   // Assemble the bend components
   AddBarComponent( new CLineComponent(u) );
   AddBarComponent( new CLineComponent(w) );
   AddBarComponent( new CLineComponent(x) );

   if( !IsZero(GetW()) )
   {
      AddBarComponent( new CBendComponent(y,t1) );
      AddBarComponent( new CHook180(hRadius,tail) );
   }

   if( !IsZero(GetX()) )
   {
      AddBarComponent( new CBendComponent(z,t2) );
      AddBarComponent( new CHook180(hRadius,tail) );
   }
}

void CType81::PreValidateBend()
{
   CBendImpl<CType81,&CLSID_Type81>::PreValidateBend();

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
