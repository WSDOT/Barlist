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


// Type66.cpp : Implementation of CType66
#include "stdafx.h"
#include "Bars.h"
#include "Type66.h"
#include "LineComponent.h"
#include "HelixComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType66
void CType66::BuildBend()
{
   CBendImpl<CType66,&CLSID_Type66>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   pBarData.Attach( GetBarData() );

   UseType use = GetUseType();

   // Error check data
   if ( use == utTransverse )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_NOTTRANSVERSE );
      AddStatusMsg( msg, CComVariant(), CComVariant() );
   }

   if ( use == utSeismic )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_NOTSEISMIC );
      AddStatusMsg( msg, CComVariant(), CComVariant() );
   }

   if ( GetStatusLevel() == stError )
      return;

   // Build the bend
   double db;
   double radius;
   double nTurns;

   pBarData->get_Diameter(&db);
   radius = (GetU() - db)/2;
   nTurns = 1.0 + GetW()/(radius*TWO_PI);

   CHelixComponent* pHelix = new CHelixComponent( radius, nTurns );
   AddBarComponent( pHelix );
}

void CType66::PreValidateBend()
{
   CBendImpl<CType66,&CLSID_Type66>::PreValidateBend();
}
