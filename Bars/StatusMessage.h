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


// StatusMessage.h : Declaration of the CStatusMessage

#ifndef __STATUSMESSAGE_H_
#define __STATUSMESSAGE_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CStatusMessage
class ATL_NO_VTABLE CStatusMessage : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CStatusMessage, &CLSID_StatusMessage>,
	public IDispatchImpl<IStatusMessage, &IID_IStatusMessage, &LIBID_BARSLib>
{
public:
	CStatusMessage()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_STATUSMESSAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStatusMessage)
	COM_INTERFACE_ENTRY(IStatusMessage)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

public:
   CComBSTR m_Text;
   CComVariant m_Val1;
   CComVariant m_Val2;

// IStatusMessage
public:
	STDMETHOD(get_Val2)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_Val1)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(get_Text)(/*[out, retval]*/ BSTR *pVal);
};

#endif //__STATUSMESSAGE_H_
