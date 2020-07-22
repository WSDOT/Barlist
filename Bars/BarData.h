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


// BarData.h : Declaration of the CBarData

#ifndef __BARDATA_H_
#define __BARDATA_H_

#include "resource.h"       // main symbols
#include <string>

typedef enum BendMeasure
{
   DiameterRatio,
   Diameter
} BendMeasure;

/////////////////////////////////////////////////////////////////////////////
// CBarData
class ATL_NO_VTABLE CBarData : 
	public CComObjectRootEx<CComSingleThreadModel>,
   //public CComRefCountTracer<CBarData, CComObjectRootEx<CComSingleThreadModel>>,
   public CComCoClass<CBarData, &CLSID_BarData>,
	public IDispatchImpl<IBarData, &IID_IBarData, &LIBID_BARSLib>
{
public:
	CBarData()
	{
	}

   void FinalRelease();

   void SetBarData(const std::string& name,Float64 db,Float64 mass,Float64 length,Float64 max_length);
   void AddBendData(UseType use,Float64 ID, BendMeasureType bendMeasure);
   void AddHookData(UseType use,HookType hook,Float64 T,BendMeasureType bendMeasure, Float64 Tmin);

DECLARE_REGISTRY_RESOURCEID(IDR_BARDATA)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBarData)
	COM_INTERFACE_ENTRY(IBarData)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IBarData
public:
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_HookData)(/*[in]*/ UseType use,/*[in]*/ HookType hook,/*[out, retval]*/ IHookData* *pVal);
	STDMETHOD(get_BendData)(/*[in]*/ UseType use, /*[out, retval]*/ IBendData* *pVal);
   STDMETHOD(get_MaxLength)(/*[out, retval]*/ Float64 *pVal);
   STDMETHOD(get_NormalLength)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(get_Mass)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(get_Diameter)(/*[out, retval]*/ Float64 *pVal);

private:
   CComBSTR m_Name;
   Float64 m_db;
   Float64 m_Mass;
   Float64 m_Length;
   Float64 m_MaxLength;

   CComPtr<IBendData> m_pBends[3];
   CComPtr<IHookData> m_pHooks[3][3];
};

#endif //__BARDATA_H_
