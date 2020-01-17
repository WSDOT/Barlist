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


// BendData.h : Declaration of the CBendData

#ifndef __BENDDATA_H_
#define __BENDDATA_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBendData
class ATL_NO_VTABLE CBendData : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBendData, &CLSID_BendData>,
	public IDispatchImpl<IBendData, &IID_IBendData, &LIBID_BARSLib>
{
public:
	CBendData()
	{
	}

   void SetData(double ID,UseType use)
   {
      m_ID  = ID;
      m_Use = use;
   }

DECLARE_REGISTRY_RESOURCEID(IDR_BENDDATA)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBendData)
	COM_INTERFACE_ENTRY(IBendData)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IBendData
public:
	STDMETHOD(get_InsideDiameter)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_Use)(/*[out, retval]*/ UseType *pVal);

private:
   UseType m_Use;
   double m_ID; // ID/db (inside diameter is this number of bar diameters)
};

#endif //__BENDDATA_H_
