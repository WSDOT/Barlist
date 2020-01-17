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


// Type57.cpp : Implementation of CType57
#include "stdafx.h"
#include "Bars.h"
#include "Type57.h"
#include "LineComponent.h"
#include "HookComponent.h"
#include "FabricationConstraints.h"

/////////////////////////////////////////////////////////////////////////////
// CType57
void CType57::BuildBend()
{
   CBendImpl<CType57,&CLSID_Type57>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   double deduct = 0;
   double radius = 0;
   double tail = 0;

   CComPtr<IBarData> pBarData;
   pBarData.Attach( GetBarData() );

   UseType use = GetUseType();

   // Error check data
   if ( (GetU() - deduct) < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg,
                    CComVariant("U"),
                    CComVariant(deduct));

   }

   if ( GetStatusLevel() == stError )
      return;


   deduct = CFabricationConstraints::GetHookDeduction( pBarData, use, ht135 );
   radius = CFabricationConstraints::GetHookRadius( pBarData, use );
   tail   = CFabricationConstraints::GetTailLength( pBarData, use, ht135 );

   // Build bend
   AddBarComponent( new CLineComponent( GetU() - deduct ) );
   AddBarComponent( new CHook135(radius,tail) );
}
