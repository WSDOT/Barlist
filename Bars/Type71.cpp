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


// Type71.cpp : Implementation of CType71
#include "stdafx.h"
#include "Bars.h"
#include "Type71.h"
#include "LineComponent.h"
#include "HelixComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType71
void CType71::BuildBend()
{
   CBendImpl<CType71,&CLSID_Type71>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   pBarData.Attach( GetBarData() );

   UseType use = GetUseType();

   double db;
   double radius;
   double nTurns;

   pBarData->get_Diameter( &db );
   radius = (GetU() - db)/2.0;
   nTurns = 0.5;

   // Error check data

   // Build the bend
   AddBarComponent( new CHelixComponent(radius,nTurns) );
   AddBarComponent( new CLineComponent(GetW()-GetU()/2) );
   AddBarComponent( new CLineComponent(GetX()-GetU()/2) );
}

void CType71::PreValidateBend()
{
   CBendImpl<CType71,&CLSID_Type71>::PreValidateBend();

   if ( GetW() <= 0)
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("W"), CComVariant(0.00) );
   }

   if ( GetX() <= 0)
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("X"), CComVariant(0.00) );
   }
}
