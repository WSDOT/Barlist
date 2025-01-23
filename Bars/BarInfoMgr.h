///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2025  Washington State Department of Transportation
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


// BarInfoMgr.h : Declaration of the CBarInfoMgr

#ifndef __BARINFOMGR_H_
#define __BARINFOMGR_H_

#include "resource.h"       // main symbols

interface IBarCollection;

/////////////////////////////////////////////////////////////////////////////
// CBarInfoMgr
class ATL_NO_VTABLE CBarInfoMgr : 
	public CComObjectRootEx<CComSingleThreadModel>,
   //public CComRefCountTracer<CBarInfoMgr, CComObjectRootEx<CComSingleThreadModel>>,
   public CComCoClass<CBarInfoMgr, &CLSID_BarInfoMgr>,
	public ISupportErrorInfo,
	public IDispatchImpl<IBarInfoMgr, &IID_IBarInfoMgr, &LIBID_BARSLib>
{
public:
	CBarInfoMgr()
	{
	}

   HRESULT FinalConstruct();
   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_BARINFOMGR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBarInfoMgr)
	COM_INTERFACE_ENTRY(IBarInfoMgr)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IBarInfoMgr
public:
	STDMETHOD(get_Bars)(/*[in]*/MaterialType material,/*[out, retval]*/ IBarCollection* *pVal);

private:
   CComPtr<IBarCollection> m_pSteelBars;
   CComPtr<IBarCollection> m_pGalvanizedBars;
   CComPtr<IBarCollection> m_pGFRPBars;
};

#endif //__BARINFOMGR_H_
