///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright � 1999-2025  Washington State Department of Transportation
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


// Type55.h : Declaration of the CType55

#ifndef __TYPE55_H_
#define __TYPE55_H_

#include "resource.h"       // main symbols
#include "BendImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CBendData
class ATL_NO_VTABLE CType55 : 
//	public CComObjectRootEx<CComSingleThreadModel>,
//	public CComCoClass<CType55, &CLSID_Type55>,
   public CBendImpl<CType55,&CLSID_Type55>
{
public:
	CType55() : CBendImpl<CType55, &CLSID_Type55>(55,true)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TYPE55)

DECLARE_PROTECT_FINAL_CONSTRUCT()

//BEGIN_COM_MAP(CType55)
//	COM_INTERFACE_ENTRY(IBend)
//	COM_INTERFACE_ENTRY(IDispatch)
//END_COM_MAP()

protected:
   virtual void BuildBend() override;

// IBend
public:
	// STDMETHOD(get_BarRecord)(/*[out, retval]*/ IBarRecord* *pVal);
	// STDMETHOD(put_BarRecord)(/*[in]*/ IBarRecord* newVal);
   // STDMETHOD(get_U)(/*[out,retval]*/ Float64* pVal);
   // STDMETHOD(put_U)(/*[in]*/ Float64 newVal);
   // STDMETHOD(get_W)(/*[out,retval]*/ Float64* pVal);
   // STDMETHOD(put_W)(/*[in]*/ Float64 newVal);
   // STDMETHOD(get_X)(/*[out,retval]*/ Float64* pVal);
   // STDMETHOD(put_X)(/*[in]*/ Float64 newVal);
   // STDMETHOD(get_Y)(/*[out,retval]*/ Float64* pVal);
   // STDMETHOD(put_Y)(/*[in]*/ Float64 newVal);
   // STDMETHOD(get_Z)(/*[out,retval]*/ Float64* pVal);
   // STDMETHOD(put_Z)(/*[in]*/ Float64 newVal);
   // STDMETHOD(get_T1)(/*[out,retval]*/ Float64* pVal);
   // STDMETHOD(put_T1)(/*[in]*/ Float64 newVal);
   // STDMETHOD(get_T2)(/*[out,retval]*/ Float64* pVal);
   // STDMETHOD(put_T2)(/*[in]*/ Float64 newVal);
   // STDMETHOD(get_Status)(/*[out,retval]*/ StatusType* pVal);
   // STDMETHOD(get_StatusMsg)(/*[out,retval]*/ BSTR* pVal);
   // STDMETHOD(get_Length)(/*[out,retval]*/ Float64* pVal);
	// STDMETHOD(get_BendType)(/*[out, retval]*/ long *pVal);
};

#endif //__TYPE55_H_
