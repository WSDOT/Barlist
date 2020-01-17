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


// Type67.h : Declaration of the CType67

#ifndef __TYPE67_H_
#define __TYPE67_H_

#include "resource.h"       // main symbols
#include "BendImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CBendData
class ATL_NO_VTABLE CType67 : 
//	public CComObjectRootEx<CComSingleThreadModel>,
//	public CComCoClass<CType67, &CLSID_Type67>,
   public CBendImpl<CType67,&CLSID_Type67>
{
public:
	CType67() : CBendImpl<CType67, &CLSID_Type67>(67,true,true,true,true,true)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TYPE67)

DECLARE_PROTECT_FINAL_CONSTRUCT()

//BEGIN_COM_MAP(CType67)
//	COM_INTERFACE_ENTRY(IBend)
//	COM_INTERFACE_ENTRY(IDispatch)
//END_COM_MAP()

protected:
   void BuildBend();
   void PreValidateBend();
   void PostValidateBend();

// IBend
public:
	// STDMETHOD(get_BarRecord)(/*[out, retval]*/ IBarRecord* *pVal);
	// STDMETHOD(put_BarRecord)(/*[in]*/ IBarRecord* newVal);
   // STDMETHOD(get_U)(/*[out,retval]*/ double* pVal);
   // STDMETHOD(put_U)(/*[in]*/ double newVal);
   // STDMETHOD(get_W)(/*[out,retval]*/ double* pVal);
   // STDMETHOD(put_W)(/*[in]*/ double newVal);
   // STDMETHOD(get_X)(/*[out,retval]*/ double* pVal);
   // STDMETHOD(put_X)(/*[in]*/ double newVal);
   // STDMETHOD(get_Y)(/*[out,retval]*/ double* pVal);
   // STDMETHOD(put_Y)(/*[in]*/ double newVal);
   // STDMETHOD(get_Z)(/*[out,retval]*/ double* pVal);
   // STDMETHOD(put_Z)(/*[in]*/ double newVal);
   // STDMETHOD(get_T1)(/*[out,retval]*/ double* pVal);
   // STDMETHOD(put_T1)(/*[in]*/ double newVal);
   // STDMETHOD(get_T2)(/*[out,retval]*/ double* pVal);
   // STDMETHOD(put_T2)(/*[in]*/ double newVal);
   // STDMETHOD(get_Status)(/*[out,retval]*/ StatusType* pVal);
   // STDMETHOD(get_StatusMsg)(/*[out,retval]*/ BSTR* pVal);
   // STDMETHOD(get_Length)(/*[out,retval]*/ double* pVal);
	// STDMETHOD(get_BendType)(/*[out, retval]*/ long *pVal);
};

#endif //__TYPE67_H_
