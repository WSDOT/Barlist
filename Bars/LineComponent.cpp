///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2024  Washington State Department of Transportation
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


// LineComponent.cpp: implementation of the CLineComponent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LineComponent.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineComponent::CLineComponent(Float64 dx, Float64 dy, Float64 dz) :
CBarComponent(),
m_Dx(dx),
m_Dy(dy),
m_Dz(dz)
{
}

CLineComponent::~CLineComponent()
{
}

Float64 CLineComponent::Length()
{
   Float64 length = CBarComponent::Length();

   length += sqrt(m_Dx*m_Dx + m_Dy*m_Dy + m_Dz*m_Dz);
   return length;
}

bool CLineComponent::BuildComponent()
{
   if ( m_Dx < 0 || m_Dy < 0 || m_Dz < 0 )
      return false;

   return true;
}
