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


// HelixComponent.h: interface for the CHelixComponent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELIXCOMPONENT_H__D6F6D551_1514_11D3_8937_006097C68A9C__INCLUDED_)
#define AFX_HELIXCOMPONENT_H__D6F6D551_1514_11D3_8937_006097C68A9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BarComponent.h"

class CHelixComponent : public CBarComponent  
{
public:
	CHelixComponent(double radius,double numTurns,double pitch = 0);
	virtual ~CHelixComponent();

   double Length();

protected:
   bool BuildComponent();

private:
   double m_Radius;
   double m_NumTurns;
   double m_Pitch;

};

#endif // !defined(AFX_HELIXCOMPONENT_H__D6F6D551_1514_11D3_8937_006097C68A9C__INCLUDED_)
