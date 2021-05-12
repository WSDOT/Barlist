///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2021  Washington State Department of Transportation
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


// HookData.h : Declaration of the CHookData

#ifndef __HOOKDATA_H_
#define __HOOKDATA_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHook
class ATL_NO_VTABLE CHookData : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHookData, &CLSID_HookData>,
	public IDispatchImpl<IHookData, &IID_IHookData, &LIBID_BARSLib>
{
public:
	CHookData()
	{
	}

   void SetData(HookType hook,UseType use,Float64 T,BendMeasureType bm,Float64 Tmin);

DECLARE_REGISTRY_RESOURCEID(IDR_HOOKDATA)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHookData)
	COM_INTERFACE_ENTRY(IHookData)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IHookData
public:
	STDMETHOD(get_MinTail)(/*[out, retval]*/ Float64 *pVal);
   STDMETHOD(get_BendMeasure)(/*[out, retval]*/BendMeasureType* pVal);
   STDMETHOD(get_Tail)(/*[out, retval]*/ Float64 *pVal);
	STDMETHOD(get_Use)(/*[out, retval]*/ UseType *pVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ HookType *pVal);

private:
   HookType m_HookType;
   UseType  m_UseType;
   Float64   m_Tail;
   BendMeasureType m_BendMeasure;
   Float64   m_MinTail;
};

#endif //__HOOKDATA_H_
