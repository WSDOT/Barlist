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


// Type54.cpp : Implementation of CType54
#include "stdafx.h"
#include "Bars.h"
#include "Type54.h"
#include "LineComponent.h"
#include "HookComponent.h"
#include "FabricationConstraints.h"

/////////////////////////////////////////////////////////////////////////////
// CType54
void CType54::BuildBend()
{
   CBendImpl<CType54,&CLSID_Type54>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   GetBarData(&pBarData);

   UseType use = GetUseType();

   Float64 deduct = CFabricationConstraints::GetHookDeduction(pBarData, use, ht90);
   Float64 radius = CFabricationConstraints::GetHookRadius(pBarData, use);
   Float64 tail = CFabricationConstraints::GetTailLength(pBarData, use, ht90);

   // Error check data
   if ( (GetU() - deduct) < 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("U"), CComVariant(deduct));

      return;
   }


   // Build bend
   AddBarComponent( new CLineComponent( GetU() - deduct ) );
   AddBarComponent( new CHook90(radius,tail) );
}
