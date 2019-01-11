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


// Type59.cpp : Implementation of CType59
#include "stdafx.h"
#include "Bars.h"
#include "Type59.h"
#include "LineComponent.h"
#include "HookComponent.h"
#include "FabricationConstraints.h"

/////////////////////////////////////////////////////////////////////////////
// CType59
void CType59::BuildBend()
{
   CBendImpl<CType59,&CLSID_Type59>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   double deduct90  = 0;
   double deduct180 = 0;
   double radius    = 0;
   double tail90    = 0;
   double tail180   = 0;

   CComPtr<IBarData> pBarData;
   pBarData.Attach( GetBarData() );

   UseType use = GetUseType();

   // Error check data
   if ( (GetU() - (deduct90+deduct180)) < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg,
                    CComVariant("U"),
                    CComVariant(deduct90+deduct180));
   }

   if ( GetStatusLevel() == stError )
      return;

   deduct90  = CFabricationConstraints::GetHookDeduction( pBarData, use, ht90 );
   deduct180 = CFabricationConstraints::GetHookDeduction( pBarData, use, ht180 );
   radius    = CFabricationConstraints::GetHookRadius( pBarData, use );
   tail90    = CFabricationConstraints::GetTailLength( pBarData, use, ht90 );
   tail180   = CFabricationConstraints::GetTailLength( pBarData, use, ht180 );

   // Build bend
   AddBarComponent( new CLineComponent( GetU() - (deduct90+deduct180) ) );
   AddBarComponent( new CHook180(radius,tail180) );
   AddBarComponent( new CHook90(radius,tail90) );
}
