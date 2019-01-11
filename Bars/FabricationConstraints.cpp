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


// FabricationConstraints.cpp: implementation of the CFabricationConstraints class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bars.h"
#include "FabricationConstraints.h"
#include "BarException.h"
#include <MathEx.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFabricationConstraints::CFabricationConstraints()
{

}

CFabricationConstraints::~CFabricationConstraints()
{

}

double CFabricationConstraints::GetBendDeduction(double OR, double angle)
{
   double deduction = 0;
   if ( IsEqual(angle,M_PI) )
      deduction = OR;
   else
      deduction = OR * tan(angle/2);

   return deduction;
}

double CFabricationConstraints::GetBendDeduction(IBarData* pBarData,UseType use)
{
   double OR = GetOutsideBendRadius(pBarData,use);
   return GetBendDeduction(OR,PI_OVER_2);
}

double CFabricationConstraints::GetHookDeduction(IBarData* pBarData,UseType use,HookType hook)
{
   double OR = GetOutsideBendRadius(pBarData,use);
   double angle = (hook == ht90 || hook == ht135) ? PI_OVER_2 : M_PI;

   return GetBendDeduction(OR,angle);
}

double CFabricationConstraints::GetHookRadius(IBarData* pBarData,UseType use)
{
   double CR = GetCenterlineBendRadius(pBarData,use);
   return CR;
}

// Replaces all occurances of strKey with strValue
void format_msg(std::string& strMsg,const std::string& strKey,const std::string& strValue)
{
   std::string::size_type pos = 0;
   while ( (pos = strMsg.find(strKey))  !=  std::string::npos )
   {
      strMsg.replace( strMsg.begin() + pos, strMsg.begin() + pos + strKey.size(),
                      strValue.begin(), strValue.end() );
   }
}

double CFabricationConstraints::GetTailLength(IBarData* pBarData,UseType use,HookType hook)
{
   double db;
   pBarData->get_Diameter(&db);

   CComPtr<IHookData> pHookData;
   pBarData->get_HookData(use,hook,&pHookData);

   if ( pHookData == NULL )
   {
      std::string strMsg( "ERROR : Bend cannot be designated %use.  %use %hook hooks are not defined per LRFD %spec");
      std::string strUse( use == utLongitudinal ? "Longitudinal" :
                          use == utTransverse   ? "Transverse"   :
                       /* use == utSeismic */     "Seismic" );
      std::string strHook( hook == ht90   ?   "90°" :
                           hook == ht135  ?  "135°" :
                        /* hook == ht180 */  "180°" );
      std::string strSpec( use == utSeismic ? "5.10.2.2" : "5.10.2.1" );

      format_msg(strMsg,std::string("%use"), strUse );
      format_msg(strMsg,std::string("%hook"), strHook );
      format_msg(strMsg,std::string("%spec"), strSpec );

      throw CBarException(strMsg.c_str());
   }

   double T;
   pHookData->get_Tail(&T);

   double Tmin;
   pHookData->get_MinTail(&Tmin);

   double length =  T*db;
   length = max(length,Tmin);

   return length;
}

double CFabricationConstraints::GetInsideBendDiameter(IBarData* pBarData,UseType use)
{
   double db;
   pBarData->get_Diameter(&db);

   CComPtr<IBendData> pBendData;
   pBarData->get_BendData(use,&pBendData);

   if ( pBendData == NULL )
   {
      std::string strMsg( "ERROR : Bend cannot be designated %use.  %use bends are not defined per LRFD 5.10.2.3");
      std::string strUse( use == utLongitudinal ? "Longitudinal" :
                          use == utTransverse   ? "Transverse"   :
                       /* use == utSeismic */     "Seismic" );

      format_msg(strMsg,std::string("%use"), strUse );

      throw CBarException(strMsg.c_str());
   }

   double id;
   pBendData->get_InsideDiameter(&id);  // this is ID/db

   double ID = db * id;
   return ID;
}

double CFabricationConstraints::GetOutsideBendDiameter(IBarData* pBarData,UseType use)
{
   double db;
   pBarData->get_Diameter(&db);

   double ID = GetInsideBendDiameter(pBarData,use);
   double OD = ID + 2*db;

   return OD;
}

double CFabricationConstraints::GetCenterlineBendDiameter(IBarData* pBarData,UseType use)
{
   double db;
   pBarData->get_Diameter(&db);

   double ID = GetInsideBendDiameter(pBarData,use);
   double CD = ID + db;

   return CD;
}

double CFabricationConstraints::GetInsideBendRadius(IBarData* pBarData,UseType use)
{
   double ID = GetInsideBendDiameter(pBarData,use);
   return ID/2;
}

double CFabricationConstraints::GetOutsideBendRadius(IBarData* pBarData,UseType use)
{
   double OD = GetOutsideBendDiameter(pBarData,use);
   return OD/2;
}

double CFabricationConstraints::GetCenterlineBendRadius(IBarData* pBarData,UseType use)
{
   double CD = GetCenterlineBendDiameter(pBarData,use);
   return CD/2;
}

