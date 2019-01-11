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


// FabricationConstraints.h: interface for the CFabricationConstraints class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FABRICATIONCONSTRAINTS_H__D6F6D557_1514_11D3_8937_006097C68A9C__INCLUDED_)
#define AFX_FABRICATIONCONSTRAINTS_H__D6F6D557_1514_11D3_8937_006097C68A9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IBarData;

class CFabricationConstraints  
{
public:
	CFabricationConstraints();
	virtual ~CFabricationConstraints();

   static double GetBendDeduction(double OR, // outside radius
                                  double angle);
   static double GetBendDeduction(IBarData* pBarData,UseType use);
   static double GetHookDeduction(IBarData* pBarData,UseType use,HookType hook);
   static double GetHookRadius(IBarData* pBarData,UseType use);
   static double GetTailLength(IBarData* pBarData,UseType use,HookType hook);

   static double GetInsideBendDiameter(IBarData* pBarData,UseType use);
   static double GetOutsideBendDiameter(IBarData* pBarData,UseType use);
   static double GetCenterlineBendDiameter(IBarData* pBarData,UseType use);
   static double GetInsideBendRadius(IBarData* pBarData,UseType use);
   static double GetOutsideBendRadius(IBarData* pBarData,UseType use);
   static double GetCenterlineBendRadius(IBarData* pBarData,UseType use);
};

#endif // !defined(AFX_FABRICATIONCONSTRAINTS_H__D6F6D557_1514_11D3_8937_006097C68A9C__INCLUDED_)
