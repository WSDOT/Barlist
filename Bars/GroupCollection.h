///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2023  Washington State Department of Transportation
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


// GroupCollection.h : Declaration of the CGroupCollection

#ifndef __GROUPCOLLECTION_H_
#define __GROUPCOLLECTION_H_

#include "resource.h"       // main symbols
#include <vector>
#include <map>
#include <string>
#include "BarsCP.h"
#include "Barlist.h"

typedef CComEnumOnSTL<IEnumVARIANT,&IID_IEnumVARIANT,VARIANT,_Copy<VARIANT>,std::vector<CComVariant> > GroupEnum;
typedef ICollectionOnSTLImpl<IGroupCollection,std::vector<CComVariant>,VARIANT,_Copy<VARIANT>, GroupEnum> IGroupColl;

/////////////////////////////////////////////////////////////////////////////
// CGroupCollection
class ATL_NO_VTABLE CGroupCollection : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGroupCollection, &CLSID_GroupCollection>,
	public IDispatchImpl<IGroupColl, &IID_IGroupCollection, &LIBID_BARSLib>,
	public CProxyIGroupCollectionEvents< CGroupCollection >,
	public IConnectionPointContainerImpl<CGroupCollection>,
   public ISupportErrorInfo,
	public IGroupEvents
	//public IDispatchImpl<IGroupCollection, &IID_IGroupCollection, &LIBID_BARSLib>
{
public:
	CGroupCollection()
	{
      m_pBarlist = 0;
	}

   virtual ~CGroupCollection()
   {
   }

   void FinalRelease();

   void SetBarlist( CBarlist* pBarlist) 
   {
      m_pBarlist = pBarlist;
   }

   CBarlist* GetBarlist()
   {
      // If this ASSERT fires, you forget to call SetBarlist for this object
      ATLASSERT( m_pBarlist != 0 );
      return m_pBarlist;
   }

DECLARE_REGISTRY_RESOURCEID(IDR_GROUPCOLLECTION)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGroupCollection)
	COM_INTERFACE_ENTRY(IGroupCollection)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IGroupEvents)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

private:
   CBarlist* m_pBarlist;
   std::map<std::string,DWORD> m_Cookies;
   std::string ConvertName(BSTR group);
   long GetIndex(BSTR bstrGroup);
   HRESULT GetGroup(VARIANT key,IGroup** ppGroup);

public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IGroupCollection
public:
	STDMETHOD(Remove)(/*[in]*/ VARIANT varIndex);
	STDMETHOD(Add)(/*[in]*/ BSTR group);
	STDMETHOD(get_Item)(/*[in]*/ VARIANT varIndex, /*[out, retval]*/ IGroup* *pVal);
//	STDMETHOD(get__NewEnum)(/*[out, retval]*/ LPUNKNOWN *pVal);
//	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
public :
	STDMETHOD(MoveDown)(/*[in]*/ VARIANT grp);
	STDMETHOD(MoveUp)(/*[in]*/ VARIANT grp);
	STDMETHOD(Move)(/*[in]*/ VARIANT grp,/*[in]*/ MoveType mt,/*[in]*/ VARIANT target);

BEGIN_CONNECTION_POINT_MAP(CGroupCollection)
	CONNECTION_POINT_ENTRY(IID_IGroupCollectionEvents)
END_CONNECTION_POINT_MAP()

// IGroupEvents
	STDMETHOD(OnGroupChanged)(IGroup* pGroup)
	{
      Fire_OnGroupChanged( pGroup );
      return S_OK;
	}
	STDMETHOD(OnBarRecordAdded)(IGroup* pGroup,IBarRecord * pNewRecord)
	{
      Fire_OnBarRecordAdded( pGroup, pNewRecord );
		return S_OK;
	}
	STDMETHOD(OnBarRecordChanged)(IGroup* pGroup,IBarRecord * pBarRecord)
	{
      Fire_OnBarRecordChanged(pGroup,pBarRecord);
		return S_OK;
	}
	STDMETHOD(OnBarRecordRemoved)(IGroup* pGroup,BSTR bstrMark)
	{
      Fire_OnBarRecordRemoved(pGroup,bstrMark);
		return S_OK;
	}
   STDMETHOD(OnBarRecordsSorted)(/*[in]*/ IGroup* pGroup)
   {
      Fire_OnBarRecordsSorted(pGroup);
      return S_OK;
   }
	STDMETHOD(OnBarRecordMoved)(IGroup* pGroup,IBarRecord * pBarRecord,long idxFrom,long idxTo)
	{
      Fire_OnBarRecordMoved(pGroup,pBarRecord,idxFrom,idxTo);
		return S_OK;
	}
};

#endif //__GROUPCOLLECTION_H_
