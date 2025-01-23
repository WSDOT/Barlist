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


// BendComponent.h: interface for the CBendComponent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BENDCOMPONENT_H__D6F6D54E_1514_11D3_8937_006097C68A9C__INCLUDED_)
#define AFX_BENDCOMPONENT_H__D6F6D54E_1514_11D3_8937_006097C68A9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BarComponent.h"

class CBendComponent : public CBarComponent
{
public:
	CBendComponent(Float64 radius,Float64 angle);
	virtual ~CBendComponent();

protected:
   Float64 m_Radius;
   Float64 m_Angle;
   virtual bool BuildComponent();
};

class CBend90 : public CBendComponent
{
public:
   CBend90(Float64 radius);
   ~CBend90();
};

#endif // !defined(AFX_BENDCOMPONENT_H__D6F6D54E_1514_11D3_8937_006097C68A9C__INCLUDED_)
