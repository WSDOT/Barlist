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


// StatusMessageCollection.h : Declaration of the CStatusMessageCollection

#ifndef __STATUSMESSAGECOLLECTION_H_
#define __STATUSMESSAGECOLLECTION_H_

#include "resource.h"       // main symbols
#include <vector>

typedef CComEnumOnSTL<IEnumVARIANT,&IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>, std::vector<CComVariant> > StatusMsgEnum;
typedef ICollectionOnSTLImpl<IStatusMessageCollection,std::vector<CComVariant>,VARIANT,_Copy<VARIANT>,StatusMsgEnum> IStatusMessageColl;

/////////////////////////////////////////////////////////////////////////////
// CStatusMessageCollection
class ATL_NO_VTABLE CStatusMessageCollection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CStatusMessageCollection, &CLSID_StatusMessageCollection>,
	public IDispatchImpl<IStatusMessageColl, &IID_IStatusMessageCollection, &LIBID_BARSLib>
//	public IDispatchImpl<IStatusMessageCollection, &IID_IStatusMessageCollection, &LIBID_BARSLib>
{
public:
	CStatusMessageCollection()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_STATUSMESSAGECOLLECTION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStatusMessageCollection)
	COM_INTERFACE_ENTRY(IStatusMessageCollection)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IStatusMessageCollection
public:
	STDMETHOD(Add)(/*[in]*/ IStatusMessage* pVal);
	STDMETHOD(get_Item)(/*[in]*/ long index, /*[out, retval]*/ IStatusMessage* *pVal);
//	STDMETHOD(get__NewEnum)(/*[out, retval]*/ LPUNKNOWN *pVal);
//	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

#endif //__STATUSMESSAGECOLLECTION_H_
