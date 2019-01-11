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


// BarCollection.cpp : Implementation of CBarCollection
#include "stdafx.h"
#include "Bars.h"
#include "BarCollection.h"
#include "BarData.h"

// Lookup Tables for Bar Data
struct BarInfo
{
   BarInfo(const char* name,double db,double mass,double length,double max_length) :
   Name(name),db(db),mass(mass),length(length),max_length(max_length) {}

   std::string Name;
   double db;
   double mass;
   double length;
   double max_length;
};

struct BendInfo
{
   BendInfo(const char* name,UseType use,double ID) :
   Name(name),use(use),ID(ID) {}

   std::string Name;
   UseType use;
   double ID;
};

struct HookInfo
{
   HookInfo(const char* name,UseType use,HookType hook,double tail,double min_tail):
   Name(name),use(use),hook(hook),tail(tail),min_tail(min_tail) {}

   std::string Name;
   UseType use;
   HookType hook;
   double tail;
   double min_tail;
};

BarInfo barinfo[] = 
{
   BarInfo( "#3",  9.5e-03,  0.560, 12.192, 12.192),
   BarInfo( "#4", 12.7e-03,  0.994, 12.192, 12.192),
   BarInfo( "#5", 15.9e-03,  1.552, 18.288, 18.288),
   BarInfo( "#6", 19.1e-03,  2.235, 18.288, 18.288),
   BarInfo( "#7", 22.2e-03,  3.042, 18.288, 18.288),
   BarInfo( "#8", 25.4e-03,  3.973, 18.288, 18.288),
   BarInfo( "#9", 28.7e-03,  5.060, 18.288, 18.288),
   BarInfo("#10", 32.3e-03,  6.404, 18.288, 18.288),
   BarInfo("#11", 35.8e-03,  7.907, 18.288, 18.288),
   BarInfo("#14", 43.0e-03, 11.384, 18.288, 18.288),
   BarInfo("#18", 57.3e-03, 20.239, 18.288, 18.288)
};

BendInfo bendinfo[] =
{
   BendInfo( "#3", utLongitudinal,  6.0),
   BendInfo( "#3", utTransverse,    4.0),
   BendInfo( "#3", utSeismic,       4.0),

   BendInfo( "#4", utLongitudinal,  6.0),
   BendInfo( "#4", utTransverse,    4.0),
   BendInfo( "#4", utSeismic,       4.0),

   BendInfo( "#5", utLongitudinal,  6.0),
   BendInfo( "#5", utTransverse,    4.0),
   BendInfo( "#5", utSeismic,       4.0),

   BendInfo( "#6", utLongitudinal,  6.0),
   BendInfo( "#6", utTransverse,    6.0),
   BendInfo( "#6", utSeismic,       6.0),

   BendInfo( "#7", utLongitudinal,  6.0),
   BendInfo( "#7", utTransverse,    6.0),
   BendInfo( "#7", utSeismic,       6.0),

   BendInfo( "#8", utLongitudinal,  6.0),
   BendInfo( "#8", utTransverse,    6.0),
   BendInfo( "#8", utSeismic,       6.0),

   BendInfo( "#9", utLongitudinal,  8.0),
   BendInfo("#10", utLongitudinal,  8.0),
   BendInfo("#11", utLongitudinal,  8.0),
   BendInfo("#14", utLongitudinal, 10.0),
   BendInfo("#18", utLongitudinal, 10.0)
};

HookInfo hookinfo[] =
{
   HookInfo( "#3", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo( "#3", utLongitudinal, ht180,  4.0,   63.5e-03),
   HookInfo( "#3", utTransverse,   ht90,   6.0,    0.0e-03),
   HookInfo( "#3", utTransverse,   ht135,  6.0,    0.0e-03),
//   HookInfo( "#3", utSeismic,      ht90,  10.0,  152.4e-03),
//   HookInfo( "#3", utSeismic,      ht135, 10.0,  152.4e-03),
//   HookInfo( "#3", utSeismic,      ht180, 10.0,  152.4e-03),
   HookInfo( "#3", utSeismic,      ht135,  6.0,   76.2e-02),

   HookInfo( "#4", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo( "#4", utLongitudinal, ht180,  4.0,   63.5e-03),
   HookInfo( "#4", utTransverse,   ht90,   6.0,    0.0e-03),
   HookInfo( "#4", utTransverse,   ht135,  6.0,    0.0e-03),
//   HookInfo( "#4", utSeismic,      ht90,  10.0,  152.4e-03),
//   HookInfo( "#4", utSeismic,      ht135, 10.0,  152.4e-03),
//   HookInfo( "#4", utSeismic,      ht180, 10.0,  152.4e-03),
   HookInfo( "#4", utSeismic,      ht135,  6.0,   76.2e-02),

   HookInfo( "#5", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo( "#5", utLongitudinal, ht180,  4.0,   63.5e-03),
   HookInfo( "#5", utTransverse,   ht90,   6.0,    0.0e-03),
   HookInfo( "#5", utTransverse,   ht135,  6.0,    0.0e-03),
//   HookInfo( "#5", utSeismic,      ht90,  10.0,  152.4e-03),
//   HookInfo( "#5", utSeismic,      ht135, 10.0,  152.4e-03),
//   HookInfo( "#5", utSeismic,      ht180, 10.0,  152.4e-03),
   HookInfo( "#5", utSeismic,      ht135,  6.0,   76.2e-02),

   HookInfo( "#6", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo( "#6", utLongitudinal, ht180,  4.0,   63.5e-03),
   HookInfo( "#6", utTransverse,   ht90,  12.0,    0.0e-03),
   HookInfo( "#6", utTransverse,   ht135,  6.0,    0.0e-03),
//   HookInfo( "#6", utSeismic,      ht90,  10.0,  152.4e-03),
//   HookInfo( "#6", utSeismic,      ht135, 10.0,  152.4e-03),
//   HookInfo( "#6", utSeismic,      ht180, 10.0,  152.4e-03),
   HookInfo( "#6", utSeismic,      ht135,  6.0,   76.2e-02),

   HookInfo( "#7", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo( "#7", utLongitudinal, ht180,  4.0,   63.5e-03),
   HookInfo( "#7", utTransverse,   ht90,  12.0,    0.0e-03),
   HookInfo( "#7", utTransverse,   ht135,  6.0,    0.0e-03),
//   HookInfo( "#7", utSeismic,      ht90,  10.0,  152.4e-03),
//   HookInfo( "#7", utSeismic,      ht135, 10.0,  152.4e-03),
//   HookInfo( "#7", utSeismic,      ht180, 10.0,  152.4e-03),
   HookInfo( "#7", utSeismic,      ht135,  6.0,   76.2e-02),

   HookInfo( "#8", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo( "#8", utLongitudinal, ht180,  4.0,   63.5e-03),
   HookInfo( "#8", utTransverse,   ht90,  12.0,    0.0e-03),
   HookInfo( "#8", utTransverse,   ht135,  6.0,    0.0e-03),
//   HookInfo( "#8", utSeismic,      ht90,  10.0,  152.4e-03),
//   HookInfo( "#8", utSeismic,      ht135, 10.0,  152.4e-03),
//   HookInfo( "#8", utSeismic,      ht180, 10.0,  152.4e-03),
   HookInfo( "#8", utSeismic,      ht135,  6.0,   76.2e-02),

   HookInfo( "#9", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo( "#9", utLongitudinal, ht180,  4.0,   63.5e-03),

   HookInfo("#10", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo("#10", utLongitudinal, ht180,  4.0,   63.5e-03),

   HookInfo("#11", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo("#11", utLongitudinal, ht180,  4.0,   63.5e-03),

   HookInfo("#14", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo("#14", utLongitudinal, ht180,  4.0,   63.5e-03),

   HookInfo("#18", utLongitudinal, ht90,  12.0,    0.0e-03),
   HookInfo("#18", utLongitudinal, ht180,  4.0,   63.5e-03)
};

//////////////////////////////////////////////////////////////////////////
// CBarCollection
HRESULT CBarCollection::FinalConstruct()
{
   // This is where we load up the collection!!!

   int cBars = sizeof(barinfo)/sizeof(BarInfo);
   for ( int bar = 0; bar < cBars; bar++ )
   {
      CComObject<CBarData>* pBarData;
      HRESULT hr = CComObject<CBarData>::CreateInstance(&pBarData);
      if ( FAILED(hr) )
         return hr;

      pBarData->SetBarData(barinfo[bar].Name,
                           barinfo[bar].db,
                           barinfo[bar].mass,
                           barinfo[bar].length,
                           barinfo[bar].max_length);

      int cBends = sizeof(bendinfo)/sizeof(BendInfo);
      for ( int bend = 0; bend < cBends; bend++ )
      {
         if ( barinfo[bar].Name.compare(bendinfo[bend].Name) == 0 )
            pBarData->AddBendData( bendinfo[bend].use, bendinfo[bend].ID );
      }

      int cHooks = sizeof(hookinfo)/sizeof(HookInfo);
      for ( int hook = 0; hook < cHooks; hook++ )
      {
         if ( barinfo[bar].Name.compare(hookinfo[hook].Name) == 0 )
            pBarData->AddHookData( hookinfo[hook].use, hookinfo[hook].hook, hookinfo[hook].tail, hookinfo[hook].min_tail );
      }

      CComQIPtr<IDispatch> pDisp(pBarData);
      CComVariant var(pDisp);
      m_coll.push_back(var);
   }

   return S_OK;
}

STDMETHODIMP CBarCollection::get_Item(VARIANT varIndex, IBarData** pVal)
{
   USES_CONVERSION;

   // TODO : Implement
   if ( varIndex.vt == VT_I2 || varIndex.vt == VT_I4 )
   {
      long index = (varIndex.vt == VT_I2 ? varIndex.iVal : varIndex.lVal);
      if ( index >= m_coll.size() )
         return E_INVALIDARG;

      CComVariant varItem( m_coll[index] );
      ATLASSERT(varItem.vt == VT_DISPATCH );
      return varItem.pdispVal->QueryInterface(pVal);
   }
   else if ( varIndex.vt == VT_BSTR )
   {
      CComBSTR target( varIndex.bstrVal );
      LPCSTR lpszTarget = OLE2A(target);

      std::vector<CComVariant>::iterator iter;
      for ( iter = m_coll.begin(); iter != m_coll.end(); iter++ )
      {
         CComVariant varItem( *iter );
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
