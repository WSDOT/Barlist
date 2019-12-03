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


// HookComponent.cpp: implementation of the CHookComponent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HookComponent.h"
#include "LineComponent.h"
#include "HelixComponent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHookComponent::CHookComponent(Float64 radius,Float64 tail) :
CBarComponent(),
m_Radius(radius),
m_Tail(tail)
{
}

CHookComponent::~CHookComponent()
{
}

////////////////////////////////////////////////////////////////////
CHook90::CHook90(Float64 radius,Float64 tail) :
CHookComponent(radius,tail)
{
}

CHook90::~CHook90()
{
}

bool CHook90::BuildComponent()
{
   Float64 nTurns = 0.25;
   AddBarComponent(new CLineComponent(m_Tail));
   AddBarComponent(new CHelixComponent(m_Radius,nTurns));
   return true;
}

////////////////////////////////////////////////////////////////////
CHook135::CHook135(Float64 radius,Float64 tail) :
CHookComponent(radius,tail)
{
}

CHook135::~CHook135()
{
}

bool CHook135::BuildComponent()
{
   Float64 nTurns = 0.375;
   AddBarComponent(new CLineComponent(m_Tail));
   AddBarComponent(new CHelixComponent(m_Radius,nTurns));
   return true;
}

////////////////////////////////////////////////////////////////////
CHook180::CHook180(Float64 radius,Float64 tail) :
CHookComponent(radius,tail)
{
}

CHook180::~CHook180()
{
}

bool CHook180::BuildComponent()
{
   Float64 nTurns = 0.50;
   AddBarComponent(new CLineComponent(m_Tail));
   AddBarComponent(new CHelixComponent(m_Radius,nTurns));
   return true;
}
