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


// BarCollection.h : Declaration of the CBarCollection

#ifndef __BARCOLLECTION_H_
#define __BARCOLLECTION_H_

#include "resource.h"       // main symbols
#include <vector>

typedef CComEnumOnSTL<IEnumVARIANT,&IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>, std::vector<CComVariant> > VecEnum;
typedef ICollectionOnSTLImpl<IBarCollection,std::vector<CComVariant>, CComVariant, _Copy<CComVariant>, VecEnum> IBarColl;

struct BarInfo;
struct BendInfo;
struct HookInfo;


/////////////////////////////////////////////////////////////////////////////
// CBarCollection
class ATL_NO_VTABLE CBarCollection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBarCollection, &CLSID_BarCollection>,
   public IDispatchImpl<IBarColl, &IID_IBarCollection, &LIBID_BARSLib>
//	public IDispatchImpl<IBarCollection, &IID_IBarCollection, &LIBID_BARSLib>
{
public:
	CBarCollection()
	{
	}

   HRESULT InitSteelBars();
   HRESULT InitGalvanizedBars();
   HRESULT InitGFRPBars();

protected:
   HRESULT Init(const BarInfo barInfo[], int cBars, const BendInfo bendInfo[], int cBends, const HookInfo hookInfo[], int cHooks);

DECLARE_REGISTRY_RESOURCEID(IDR_BARCOLLECTION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBarCollection)
	COM_INTERFACE_ENTRY(IBarCollection)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IBarCollection
public:
// ICollectionOnSTLImpl<> implements get_Item(long Index, ItemType* pvar) but
// I want to be able to index with a zero based index or bar number ("#3"),
// so I replace it with this.
	STDMETHOD(get_Item)(/*[in]*/ VARIANT varIndex, /*[out, retval]*/ IBarData** pVal);

// the following two are implemented by ICollectionOnSTLImpl<>
//	STDMETHOD(get__NewEnum)(/*[out, retval]*/ LPUNKNOWN *pVal);
//	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
};

#endif //__BARCOLLECTION_H_
