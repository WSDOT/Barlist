///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2021  Washington State Department of Transportation
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


// Type58.cpp : Implementation of CType58
#include "stdafx.h"
#include "Bars.h"
#include "Type58.h"
#include "LineComponent.h"
#include "HookComponent.h"
#include "FabricationConstraints.h"

/////////////////////////////////////////////////////////////////////////////
// CType58
void CType58::BuildBend()
{
   CBendImpl<CType58,&CLSID_Type58>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   GetBarData(&pBarData);

   UseType use = GetUseType();

   Float64 deduct90 = CFabricationConstraints::GetHookDeduction(pBarData, use, ht90);
   Float64 deduct135 = CFabricationConstraints::GetHookDeduction(pBarData, use, ht135);
   Float64 radius = CFabricationConstraints::GetHookRadius(pBarData, use);
   Float64 tail90 = CFabricationConstraints::GetTailLength(pBarData, use, ht90);
   Float64 tail135 = CFabricationConstraints::GetTailLength(pBarData, use, ht135);

   // Error check data
   if ( (GetU() - (deduct90+deduct135)) < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("U"), CComVariant(deduct90+deduct135));

   }

   if ( GetStatusLevel() == stError )
      return;

   // Build bend
   AddBarComponent( new CLineComponent( GetU() - (deduct90+deduct135) ) );
   AddBarComponent( new CHook135(radius,tail135) );
   AddBarComponent( new CHook90(radius,tail90) );
}
