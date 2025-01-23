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


// HelixComponent.cpp: implementation of the CHelixComponent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HelixComponent.h"
#include <MathEx.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelixComponent::CHelixComponent(Float64 radius,Float64 numTurns,Float64 pitch) :
CBarComponent(),
m_Radius(radius),
m_NumTurns(numTurns),
m_Pitch(pitch)
{
}

CHelixComponent::~CHelixComponent()
{
}

bool CHelixComponent::BuildComponent()
{
   if ( m_Radius < 0 || m_NumTurns <= 0 )
      return false;
   
   return true;
}

Float64 CHelixComponent::Length()
{
   Float64 circumference = 2*m_Radius*M_PI;
   Float64 length = CBarComponent::Length();

   length += m_NumTurns*sqrt(circumference*circumference + m_Pitch*m_Pitch);
   return length;
}