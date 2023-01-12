///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2023  Washington State Department of Transportation
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


// Type67.cpp : Implementation of CType67
#include "stdafx.h"
#include "Bars.h"
#include "Type67.h"
#include "LineComponent.h"
#include "HelixComponent.h"
#include <MathEx.h>

/////////////////////////////////////////////////////////////////////////////
// CType67
void CType67::BuildBend()
{
   CBendImpl<CType67,&CLSID_Type67>::BuildBend();

   if ( GetStatusLevel() == stError )
      return;

   CComPtr<IBarData> pBarData;
   GetBarData(&pBarData);

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
   Float64 db;
   Float64 radius;
   Float64 nTurns;
   Float64 pitch;

   pBarData->get_Diameter(&db);
   radius = (GetU() - db)/2;
   pitch = GetX();
   nTurns = GetW()/GetX();

   CHelixComponent* pHelix = new CHelixComponent( radius, nTurns, pitch );
   AddBarComponent( pHelix );

   Float64 length = pHelix->Length();
   Float64 splice_length;
   if ( IsZero(GetZ()) )
      splice_length = 0;
   else
      splice_length = GetY() * length / GetZ();

   AddBarComponent( new CLineComponent(splice_length) );
}

void CType67::PreValidateBend()
{
   CBendImpl<CType67,&CLSID_Type67>::PreValidateBend();

   if ( GetW() <= 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("W"), CComVariant(0.00) );
   }

   if ( GetX() <= 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEGREATERTHAN );
      AddStatusMsg( msg, CComVariant("X"), CComVariant(0.00) );
   }

   if ( IsZero(GetZ()) && GetY() > 0 )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_MUSTBEEQUALTO );
      AddStatusMsg( msg, CComVariant("Y"), CComVariant(0.00) );
   }

   if ( GetX() >= GetW() )
   {
      SetStatusLevel( stError );
      CComBSTR msg;
      msg.LoadString( ERR_XLESSTHANW );
      AddStatusMsg( msg, CComVariant(), CComVariant() );
   }

}

void CType67::PostValidateBend()
{
   // NOTE: Don't call base class PostValidateBend().  We don't want
   //       the maximum lengths to be checked.
   //CBendImpl<CType67,&CLSID_Type67>::PostValidateBend();
}