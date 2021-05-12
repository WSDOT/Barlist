///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2021  Washington State Department of Transportation
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


// Group.h : Declaration of the CGroup

#ifndef __GROUP_H_
#define __GROUP_H_

#include "resource.h"       // main symbols
#include "BarsCP.h"
#include "Barlist.h"

#include <array>

/////////////////////////////////////////////////////////////////////////////
// CGroup
class ATL_NO_VTABLE CGroup : 
	public CComObjectRootEx<CComSingleThreadModel>,
   //public CComRefCountTracer<CGroup, CComObjectRootEx<CComSingleThreadModel>>,
	public CComCoClass<CGroup, &CLSID_Group>,
	public IDispatchImpl<IGroup, &IID_IGroup, &LIBID_BARSLib>,
	public CProxyIGroupEvents< CGroup >,
	public IConnectionPointContainerImpl<CGroup>,
   public ISupportErrorInfo,
	public IBarRecordCollectionEvents
{
public:
   CGroup() :
      m_Name("")
	{
      m_Superstructure.fill(0);
      m_SuperstructureEpoxy.fill(0);
      m_Substructure.fill(0);
      m_SubstructureEpoxy.fill(0);

      m_Status = stOK;

      m_pBarlist = nullptr;
   }

   void SetBarlist(CBarlist* pBarlist);

   CBarlist* GetBarlist()
   {
      // If this ASSERT fires, you forget to call SetBarlist for this object
      ATLASSERT( m_pBarlist != nullptr );
      return m_pBarlist;
   }

   HRESULT FinalConstruct();
   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_GROUP)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CGroup)
	COM_INTERFACE_ENTRY(IGroup)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IBarRecordCollectionEvents)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

private:
   CComBSTR m_Name;
   CComPtr<IBarRecordCollection> m_Bars;
   DWORD m_BarsCookie;
   void UpdateStatus();

   // array index is MaterialType
   // quantities are mass/weight in all cases
   // except D7957 (GFRP) in which case the quantity is length
   std::array<Float64, 16> m_Superstructure;
   std::array<Float64, 16> m_SuperstructureEpoxy;
   std::array<Float64, 16> m_Substructure;
   std::array<Float64, 16> m_SubstructureEpoxy;

   StatusType m_Status;
   CBarlist* m_pBarlist;

public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


// IGroup
public:
   STDMETHOD(get_Quantity)(/*[in]*/MaterialType material, /*[in]*/VARIANT_BOOL bEpoxy, /*[in]*/VARIANT_BOOL bSubstructure, /*[out, retval]*/Float64* pVal);
	STDMETHOD(get_BarRecords)(/*[out, retval]*/ IBarRecordCollection* *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
   STDMETHOD(get_Status)(/*[out,retval]*/ StatusType *pVal);

public :

BEGIN_CONNECTION_POINT_MAP(CGroup)
	CONNECTION_POINT_ENTRY(IID_IGroupEvents)
END_CONNECTION_POINT_MAP()
// IBarRecordCollectionEvents
	STDMETHOD(OnBarRecordAdded)(IBarRecord * pNewRecord)
	{
      Fire_OnBarRecordAdded( this, pNewRecord );
      UpdateStatus();
      return S_OK;
	}
	STDMETHOD(OnBarRecordRemoved)(BSTR Mark)
	{
      Fire_OnBarRecordRemoved( this, Mark );
      UpdateStatus();
      return S_OK;
	}
	STDMETHOD(OnBarRecordChanged)(IBarRecord* pBarRecord)
	{
      Fire_OnBarRecordChanged( this, pBarRecord );
      UpdateStatus();
      return S_OK;
	}

	STDMETHOD(OnBarRecordsSorted)()
   {
      Fire_OnBarRecordsSorted( this );
   	return S_OK;
   }
   STDMETHOD(OnBarRecordMoved)(IBarRecord* pBarRecord,long idxFrom,long idxTo)
   {
      Fire_OnBarRecordMoved( this, pBarRecord, idxFrom, idxTo );
      return S_OK;
   }
};

#endif //__GROUP_H_
