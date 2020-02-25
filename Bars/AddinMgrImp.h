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


// AddinMgrImp.h : Declaration of the CAddinMgr

#ifndef __ADDINMGR_H_
#define __ADDINMGR_H_

#include "resource.h"       // main symbols
#include <vector>

typedef CComEnumOnSTL<IEnumVARIANT,&IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>, std::vector<CComVariant> > VecEnum;
typedef ICollectionOnSTLImpl<IAddinMgr,std::vector<CComVariant>,VARIANT,_Copy<VARIANT>,VecEnum> IAddinManager;

/////////////////////////////////////////////////////////////////////////////
// CAddinMgr
class ATL_NO_VTABLE CAddinMgr : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAddinMgr, &CLSID_AddinMgr>,
	public IDispatchImpl<IAddinManager, &IID_IAddinMgr, &LIBID_BARSLib>
//	public IDispatchImpl<IAddinMgr, &IID_IAddinMgr, &LIBID_BARSLib>
{
public:
	CAddinMgr()
	{
	}

   HRESULT FinalConstruct();

DECLARE_REGISTRY_RESOURCEID(IDR_ADDINMGR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAddinMgr)
	COM_INTERFACE_ENTRY(IAddinMgr)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IAddinMgr
public:
   STDMETHOD(get_Item)(/*[in]*/ short index, /*[out, retval]*/ IBarlistAddin* *pVal);
};

#endif //__ADDINMGR_H_
