///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright � 1999-2026  Washington State Department of Transportation
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


// HelixComponent.h: interface for the CHelixComponent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BarComponent.h"

class CHelixComponent : public CBarComponent  
{
public:
	CHelixComponent(Float64 radius,Float64 numTurns,Float64 pitch = 0);
	virtual ~CHelixComponent();

   Float64 Length();

protected:
   bool BuildComponent();

private:
   Float64 m_Radius;
   Float64 m_NumTurns;
   Float64 m_Pitch;

};

