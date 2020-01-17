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


// BendComponent.cpp: implementation of the CBendComponent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BendComponent.h"
#include "HelixComponent.h"
#include <MathEx.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBendComponent::CBendComponent(double radius,double angle) :
CBarComponent(),
m_Radius(radius),
m_Angle(angle)
{
}

CBendComponent::~CBendComponent()
{

}

bool CBendComponent::BuildComponent()
{
   double nTurns = m_Angle / TWO_PI;

   AddBarComponent( new CHelixComponent(m_Radius,nTurns) );
   return true;
}

///////////////////////////////////////////////////////////////////////
CBend90::CBend90(double radius) :
CBendComponent(radius, PI_OVER_2 )
{
}

CBend90::~CBend90()
{
}
