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


// Type93.cpp : Implementation of CType93
#include "stdafx.h"
#include "Bars.h"
#include "Type93.h"
#include "LineComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType93
void CType93::BuildBend()
{
   CBendImpl<CType93,&CLSID_Type93>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   pBarData.Attach( GetBarData() );

   UseType use = GetUseType();

   // Error check data

   // Build the bend

   // Assemble the bend components
   AddBarComponent( new CLineComponent(GetU()) );
}
