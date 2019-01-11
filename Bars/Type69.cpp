///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 2009-2019, Washington State Department of Transportation
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


// Type69.cpp : Implementation of CType69
#include "stdafx.h"
#include "Bars.h"
#include "Type69.h"
#include "FabricationConstraints.h"
#include "LineComponent.h"
#include "BendComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType69
void CType69::BuildBend()
{
   CBendImpl<CType69,&CLSID_Type69>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   pBarData.Attach( GetBarData() );

   UseType use = GetUseType();

   // Error check data

   // Build the bend
   double db;
   double oRadius; // outside radius
   double cRadius; // centerline radius
   double deduct;
   double deduct90;
   double angle;
   double u, wx, y, z; // reduced dimensions (wx is the hypotenous)

   pBarData->get_Diameter( &db );
   oRadius = CFabricationConstraints::GetOutsideBendRadius( pBarData, use );
   cRadius = CFabricationConstraints::GetCenterlineBendRadius( pBarData, use );
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
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("U"), CComVariant(deduct) );
   }

   if ( wx < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("sqrt(W*W + X*X)"), CComVariant(deduct+deduct90) );
   }

   if ( y < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("Y"), CComVariant(deduct+deduct90) );
   }

   if ( z < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("Z"), CComVariant(deduct90) );
   }

   if ( GetStatusLevel() == stError )
      return;

   // Assemble the bend components
   AddBarComponent( new CLineComponent(u) );
   AddBarComponent( new CLineComponent(wx) );
   AddBarComponent( new CLineComponent(y) );
   AddBarComponent( new CLineComponent(z) );

   if ( wx > 0 )
      AddBarComponent( new CBendComponent(cRadius,angle) );

   if ( y > 0 )
      AddBarComponent( new CBendComponent(cRadius,angle) );

   if ( z > 0 )
      AddBarComponent( new CBend90(cRadius) );
}

void CType69::PreValidateBend()
{
   // NOTE: Don't call base-class PreValidateBend().  U = 0 is
   //       valid for this bend!!!
   //CBendImpl<CType69,&CLSID_Type69>::PreValidateBend();

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
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_ILLFORMEDBEND );
      AddStatusMsg( msg, CComVariant(), CComVariant() );
   }

   if (!IsZero(GetZ()) && IsZero(GetY()))
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEZEROIFOTHERISZERO );
      AddStatusMsg( msg, CComVariant("Y"), CComVariant("Z") );
   }
}
