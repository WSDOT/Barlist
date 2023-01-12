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


// BarCollection.cpp : Implementation of CBarCollection
#include "stdafx.h"
#include "Bars.h"
#include "BarData.h"
#include "BarCollection.h"

struct BarInfo
{
   BarInfo(const char* name, Float64 db, Float64 mass, Float64 length, Float64 max_length) :
      Name(name), db(db), mass(mass), length(length), max_length(max_length) {}

   std::string Name;
   Float64 db;
   Float64 mass;
   Float64 length;
   Float64 max_length;
};

struct BendInfo
{
   BendInfo(const char* name, UseType use, Float64 ID, BendMeasure bm = DiameterRatio) :
      Name(name), use(use), ID(ID), bendMeasure(bm) {}

   std::string Name;
   UseType use;
   Float64 ID;
   BendMeasure bendMeasure;
};

struct HookInfo
{
   HookInfo(const char* name, UseType use, HookType hook, Float64 tail, Float64 min_tail, BendMeasure bm = DiameterRatio) :
      Name(name), use(use), hook(hook), tail(tail), min_tail(min_tail), bendMeasure(bm) {}

   std::string Name;
   UseType use;
   HookType hook;
   Float64 tail;
   BendMeasure bendMeasure;
   Float64 min_tail;
};

#pragma warning(disable : 4251)
#include <Units\Units.h>
static Float64 _40_ft = WBFL::Units::ConvertToSysUnits(40.0, WBFL::Units::Measure::Feet);
static Float64 _60_ft = WBFL::Units::ConvertToSysUnits(60.0, WBFL::Units::Measure::Feet);


////////////////////////////////////////////////////////
// Steel
////////////////////////////////////////////////////////

BarInfo steel_barinfo[] = 
{
   BarInfo( "#3", WBFL::Units::ConvertToSysUnits(0.375,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(0.376, WBFL::Units::Measure::LbfPerFoot), _40_ft, _40_ft),
   BarInfo( "#4", WBFL::Units::ConvertToSysUnits(0.500,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(0.668, WBFL::Units::Measure::LbfPerFoot), _40_ft, _40_ft),
   BarInfo( "#5", WBFL::Units::ConvertToSysUnits(0.625,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(1.043, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   BarInfo( "#6", WBFL::Units::ConvertToSysUnits(0.750,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(1.502, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   BarInfo( "#7", WBFL::Units::ConvertToSysUnits(0.875,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(2.044, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   BarInfo( "#8", WBFL::Units::ConvertToSysUnits(1.000,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(2.670, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   BarInfo( "#9", WBFL::Units::ConvertToSysUnits(1.128,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(3.400, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   BarInfo("#10", WBFL::Units::ConvertToSysUnits(1.270,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(4.303, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   BarInfo("#11", WBFL::Units::ConvertToSysUnits(1.410,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(5.313, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   BarInfo("#14", WBFL::Units::ConvertToSysUnits(1.693,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(7.650, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   BarInfo("#18", WBFL::Units::ConvertToSysUnits(2.257,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(13.60, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft)
};

BarInfo galvanized_barinfo[] =
{
   BarInfo( "#3", WBFL::Units::ConvertToSysUnits(0.375,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(0.376, WBFL::Units::Measure::LbfPerFoot), _40_ft, _40_ft),
   BarInfo( "#4", WBFL::Units::ConvertToSysUnits(0.500,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(0.668, WBFL::Units::Measure::LbfPerFoot), _40_ft, _40_ft),
   BarInfo( "#5", WBFL::Units::ConvertToSysUnits(0.625,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(1.043, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   BarInfo( "#6", WBFL::Units::ConvertToSysUnits(0.750,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(1.502, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   BarInfo( "#7", WBFL::Units::ConvertToSysUnits(0.875,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(2.044, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   BarInfo( "#8", WBFL::Units::ConvertToSysUnits(1.000,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(2.670, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   BarInfo( "#9", WBFL::Units::ConvertToSysUnits(1.128,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(3.400, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   BarInfo("#10", WBFL::Units::ConvertToSysUnits(1.270,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(4.303, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   BarInfo("#11", WBFL::Units::ConvertToSysUnits(1.410,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(5.313, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   BarInfo("#14", WBFL::Units::ConvertToSysUnits(1.693,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(7.650, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   BarInfo("#18", WBFL::Units::ConvertToSysUnits(2.257,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(13.60, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft)
};

BendInfo steel_bendinfo[] =
{
   BendInfo("#3", utLongitudinal,  6.0),
   BendInfo("#3", utTransverse,    4.0),
   BendInfo("#3", utSeismic,       4.0),

   BendInfo("#4", utLongitudinal,  6.0),
   BendInfo("#4", utTransverse,    4.0),
   BendInfo("#4", utSeismic,       4.0),

   BendInfo("#5", utLongitudinal,  6.0),
   BendInfo("#5", utTransverse,    4.0),
   BendInfo("#5", utSeismic,       4.0),

   BendInfo("#6", utLongitudinal,  6.0),
   BendInfo("#6", utTransverse,    6.0),
   BendInfo("#6", utSeismic,       6.0),

   BendInfo("#7", utLongitudinal,  6.0),
   BendInfo("#7", utTransverse,    6.0),
   BendInfo("#7", utSeismic,       6.0),

   BendInfo("#8", utLongitudinal,  6.0),
   BendInfo("#8", utTransverse,    6.0),
   BendInfo("#8", utSeismic,       6.0),

   BendInfo( "#9", utLongitudinal,  8.0),
   BendInfo("#10", utLongitudinal,  8.0),
   BendInfo("#11", utLongitudinal,  8.0),
   BendInfo("#14", utLongitudinal, 10.0),
   BendInfo("#18", utLongitudinal, 10.0)
};

HookInfo steel_hookinfo[] =
{
   HookInfo("#3", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#3", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   HookInfo("#3", utTransverse,   ht90,   6.0,    0.0),
   HookInfo("#3", utTransverse,   ht135,  6.0,    0.0),
   HookInfo("#3", utSeismic,      ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   HookInfo("#4", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#4", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   HookInfo("#4", utTransverse,   ht90,   6.0,    0.0),
   HookInfo("#4", utTransverse,   ht135,  6.0,    0.0),
   HookInfo("#4", utSeismic,      ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   HookInfo("#5", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#5", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   HookInfo("#5", utTransverse,   ht90,   6.0,    0.0),
   HookInfo("#5", utTransverse,   ht135,  6.0,    0.0),
   HookInfo("#5", utSeismic,      ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   HookInfo("#6", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#6", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   HookInfo("#6", utTransverse,   ht90,  12.0,    0.0),
   HookInfo("#6", utTransverse,   ht135,  6.0,    0.0),
   HookInfo("#6", utSeismic,      ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   HookInfo("#7", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#7", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   HookInfo("#7", utTransverse,   ht90,  12.0,    0.0),
   HookInfo("#7", utTransverse,   ht135,  6.0,    0.0),
   HookInfo("#7", utSeismic,      ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   HookInfo("#8", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#8", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   HookInfo("#8", utTransverse,   ht90,  12.0,    0.0),
   HookInfo("#8", utTransverse,   ht135,  6.0,    0.0),
   HookInfo("#8", utSeismic,      ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   HookInfo("#9", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#9", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),

   HookInfo("#10", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#10", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),

   HookInfo("#11", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#11", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),

   HookInfo("#14", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#14", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),

   HookInfo("#18", utLongitudinal, ht90,  12.0,    0.0),
   HookInfo("#18", utLongitudinal, ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch))
};

////////////////////////////////////////////////////////
// GFRP 
////////////////////////////////////////////////////////
BarInfo gfrp_barinfo[] =
{
   BarInfo("#3",  WBFL::Units::ConvertToSysUnits(0.375,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft), // weight = 0 because quantities are by length
   BarInfo("#4",  WBFL::Units::ConvertToSysUnits(0.500,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   BarInfo("#5",  WBFL::Units::ConvertToSysUnits(0.625,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   BarInfo("#6",  WBFL::Units::ConvertToSysUnits(0.750,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   BarInfo("#7",  WBFL::Units::ConvertToSysUnits(0.875,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   BarInfo("#8",  WBFL::Units::ConvertToSysUnits(1.000,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   BarInfo("#9",  WBFL::Units::ConvertToSysUnits(1.128,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   BarInfo("#10", WBFL::Units::ConvertToSysUnits(1.270,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
};

BendInfo gfrp_bendinfo[] = 
{
   BendInfo("#3", utLongitudinal, 6.0),
   BendInfo("#4", utLongitudinal, 6.0),
   BendInfo("#5", utLongitudinal, 6.0),
   BendInfo("#6", utLongitudinal, 6.0),
   BendInfo("#7", utLongitudinal, 6.0),
   BendInfo("#8", utLongitudinal, 6.0),
};

// GFRP Hook tail extensions are 12db, measured from the outside of the bar.
// The tail length in this data structure is the bar extension measured from
// the center of the bend diameter. We need to compute a tail extension
// that results in a bend tail that is 12db from the outside of the bar
// 12db - (6db)/2 - 1db = 8db
// All bends for GFRP are 6db. The distance from the start of the tail extension
// to the inside of bar bend is half this distance. The distance to the outside
// of the bend is 1 db.
HookInfo gfrp_hookinfo[] =
{
   HookInfo("#3", utLongitudinal, ht90,   8.0, 0.0),
   HookInfo("#3", utLongitudinal, ht180,  8.0, 0.0),
   HookInfo("#3", utTransverse,   ht90,   8.0, 0.0),
   HookInfo("#3", utTransverse,   ht135,  8.0, 0.0),
   HookInfo("#3", utSeismic,      ht135,  8.0, 0.0),

   HookInfo("#4", utLongitudinal, ht90,   8.0, 0.0),
   HookInfo("#4", utLongitudinal, ht180,  8.0, 0.0),
   HookInfo("#4", utTransverse,   ht90,   8.0, 0.0),
   HookInfo("#4", utTransverse,   ht135,  8.0, 0.0),
   HookInfo("#4", utSeismic,      ht135,  8.0, 0.0),

   HookInfo("#5", utLongitudinal, ht90,   8.0, 0.0),
   HookInfo("#5", utLongitudinal, ht180,  8.0, 0.0),
   HookInfo("#5", utTransverse,   ht90,   8.0, 0.0),
   HookInfo("#5", utTransverse,   ht135,  8.0, 0.0),
   HookInfo("#5", utSeismic,      ht135,  8.0, 0.0),

   HookInfo("#6", utLongitudinal, ht90,   8.0, 0.0),
   HookInfo("#6", utLongitudinal, ht180,  8.0, 0.0),
   HookInfo("#6", utTransverse,   ht90,   8.0, 0.0),
   HookInfo("#6", utTransverse,   ht135,  8.0, 0.0),
   HookInfo("#6", utSeismic,      ht135,  8.0, 0.0),

   HookInfo("#7", utLongitudinal, ht90,   8.0, 0.0),
   HookInfo("#7", utLongitudinal, ht180,  8.0, 0.0),
   HookInfo("#7", utTransverse,   ht90,   8.0, 0.0),
   HookInfo("#7", utTransverse,   ht135,  8.0, 0.0),
   HookInfo("#7", utSeismic,      ht135,  8.0, 0.0),

   HookInfo("#8", utLongitudinal, ht90,   8.0, 0.0),
   HookInfo("#8", utLongitudinal, ht180,  8.0, 0.0),
   HookInfo("#8", utTransverse,   ht90,   8.0, 0.0),
   HookInfo("#8", utTransverse,   ht135,  8.0, 0.0),
   HookInfo("#8", utSeismic,      ht135,  8.0, 0.0),
};

HRESULT CBarCollection::Init(const BarInfo barInfo[],int cBars,const BendInfo bendInfo[],int cBends,const HookInfo hookInfo[],int cHooks)
{
   // for each bar size...
   for (int bar = 0; bar < cBars; bar++)
   {
      CComObject<CBarData>* pBarData;
      HRESULT hr = CComObject<CBarData>::CreateInstance(&pBarData);
      if (FAILED(hr))
         return hr;

      pBarData->SetBarData(barInfo[bar].Name, barInfo[bar].db, barInfo[bar].mass, barInfo[bar].length, barInfo[bar].max_length);

      // add bend info for the current bar size
      for (int bend = 0; bend < cBends; bend++)
      {
         if (barInfo[bar].Name.compare(bendInfo[bend].Name) == 0)
         {
            pBarData->AddBendData(bendInfo[bend].use, bendInfo[bend].ID, (BendMeasureType)bendInfo[bend].bendMeasure);
         }
      }

      // add hook info for the current bar size
      for (int hook = 0; hook < cHooks; hook++)
      {
         if (barInfo[bar].Name.compare(hookInfo[hook].Name) == 0)
         {
            pBarData->AddHookData(hookInfo[hook].use, hookInfo[hook].hook, hookInfo[hook].tail, (BendMeasureType)hookInfo[hook].bendMeasure, hookInfo[hook].min_tail);
         }
      }

      CComQIPtr<IDispatch> pDisp(pBarData);
      CComVariant var(pDisp);
      m_coll.push_back(var);
   }

   return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// CBarCollection
HRESULT CBarCollection::InitSteelBars()
{
   return Init(steel_barinfo, sizeof(steel_barinfo)/sizeof(BarInfo), steel_bendinfo, sizeof(steel_bendinfo)/sizeof(BendInfo),steel_hookinfo,sizeof(steel_hookinfo)/sizeof(HookInfo));
}

HRESULT CBarCollection::InitGalvanizedBars()
{
   return Init(galvanized_barinfo, sizeof(galvanized_barinfo) / sizeof(BarInfo), steel_bendinfo, sizeof(steel_bendinfo) / sizeof(BendInfo), steel_hookinfo, sizeof(steel_hookinfo) / sizeof(HookInfo));
}

HRESULT CBarCollection::InitGFRPBars()
{
   return Init(gfrp_barinfo, sizeof(gfrp_barinfo) / sizeof(BarInfo), gfrp_bendinfo, sizeof(gfrp_bendinfo) / sizeof(BendInfo), gfrp_hookinfo, sizeof(gfrp_hookinfo) / sizeof(HookInfo));
}

STDMETHODIMP CBarCollection::get_Item(VARIANT varIndex, IBarData** pVal)
{
   USES_CONVERSION;

   // TODO : Implement
   if ( varIndex.vt == VT_I2 || varIndex.vt == VT_I4 )
   {
      long index = (varIndex.vt == VT_I2 ? varIndex.iVal : varIndex.lVal);
      if ( m_coll.size() <= index )
         return E_INVALIDARG;

      CComVariant varItem( m_coll[index] );
      ATLASSERT(varItem.vt == VT_DISPATCH );
      return varItem.pdispVal->QueryInterface(pVal);
   }
   else if ( varIndex.vt == VT_BSTR )
   {
      CComBSTR target( varIndex.bstrVal );
      LPCSTR lpszTarget = OLE2A(target);

      for( const auto& varItem : m_coll)
      {
         CComQIPtr<IBarData> pBarData( varItem.pdispVal );
         CComBSTR name;
         pBarData->get_Name( &name );
         LPCSTR lpszName = OLE2A(name);
         if ( strcmp(lpszTarget, lpszName ) == 0 )
         {
            return pBarData->QueryInterface( pVal );
         }
      }

      return E_INVALIDARG;
   }

   return E_INVALIDARG;
}
