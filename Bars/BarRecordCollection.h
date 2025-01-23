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


// BarRecordCollection.h : Declaration of the CBarRecordCollection

#ifndef __BARRECORDCOLLECTION_H_
#define __BARRECORDCOLLECTION_H_

#include "resource.h"       // main symbols
#include <vector>
#include <map>
#include <string>
#include "BarsCP.h"
#include "Barlist.h"

typedef CComEnumOnSTL<IEnumVARIANT,&IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>, std::vector<CComVariant> > BrEnum;
typedef ICollectionOnSTLImpl<IBarRecordCollection,std::vector<CComVariant>,VARIANT,_Copy<VARIANT>,BrEnum> IBarRecordColl;

/////////////////////////////////////////////////////////////////////////////
// CBarRecordCollection
class ATL_NO_VTABLE CBarRecordCollection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBarRecordCollection, &CLSID_BarRecordCollection>,
	public IDispatchImpl<IBarRecordColl, &IID_IBarRecordCollection, &LIBID_BARSLib>,
	public CProxyIBarRecordCollectionEvents< CBarRecordCollection >,
	public IConnectionPointContainerImpl<CBarRecordCollection>,
   public ISupportErrorInfo,
	public IBarRecordEvents
//	public IDispatchImpl<IBarRecordCollection, &IID_IBarRecordCollection, &LIBID_BARSLib>
{
public:
	CBarRecordCollection()
	{
      m_pBarlist = 0;
	}

   void SetBarlist(CBarlist* pBarlist)
   {
      m_pBarlist = pBarlist;
   }

   CBarlist* GetBarlist()
   {
      // If this ASSERT fires, you forget to call SetBarlist for this object
      ATLASSERT( m_pBarlist != 0 );
      return m_pBarlist;
   }

   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_BARRECORDCOLLECTION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBarRecordCollection)
	COM_INTERFACE_ENTRY(IBarRecordCollection)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IBarRecordEvents)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

private:
   HRESULT GetIndex(VARIANT varIndex,long* pIndex);
   long GetIndex(BSTR bstrMark);
   std::string ConvertMark(BSTR bstrMark);
   std::multimap<std::string,DWORD> m_Cookies;
   CBarlist* m_pBarlist;
   HRESULT GetBarRecord(VARIANT key,IBarRecord** ppBarRec);

public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IBarRecordCollection
public:
   STDMETHOD(Insert)(/*[in]*/VARIANT varIdx, /*[in]*/IBarRecord* pBarRecord);
	STDMETHOD(Move)(/*[in]*/ VARIANT rec, /*[in]*/ MoveType mt, /*[in]*/ VARIANT target);
	STDMETHOD(MoveUp)(/*[in]*/ VARIANT rec);
	STDMETHOD(MoveDown)(/*[in]*/ VARIANT rec);
	STDMETHOD(Replace)(/*[in]*/ VARIANT varIndex,/*[in]*/ IBarRecord* pBarRecord);
	STDMETHOD(Sort)();
	STDMETHOD(Remove)(/*[in]*/ VARIANT varIndex);
	STDMETHOD(Add)(/*[in]*/ IBarRecord* pBarRecord);
	STDMETHOD(get_Item)(/*[in]*/ VARIANT varIndex, /*[out, retval]*/ IBarRecord* *pVal);
//	STDMETHOD(get__NewEnum)(/*[out, retval]*/ LPUNKNOWN *pVal);
//	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);

BEGIN_CONNECTION_POINT_MAP(CBarRecordCollection)
	CONNECTION_POINT_ENTRY(IID_IBarRecordCollectionEvents)
END_CONNECTION_POINT_MAP()

// IBarRecordEvents
	STDMETHOD(OnBarRecordChanged)(IBarRecord* pBarRecord)
	{
      Fire_OnBarRecordChanged(pBarRecord);
		return S_OK;
	}
};

#endif //__BARRECORDCOLLECTION_H_
