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


// BarRecord.h : Declaration of the CBarRecord

#ifndef __BARRECORD_H_
#define __BARRECORD_H_

#include "resource.h"       // main symbols
#include "BarsCP.h"
#include "StatusMgr.h"
#include "Barlist.h"

/////////////////////////////////////////////////////////////////////////////
// CBarRecord
class ATL_NO_VTABLE CBarRecord : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBarRecord, &CLSID_BarRecord>,
	public IDispatchImpl<IBarRecord, &IID_IBarRecord, &LIBID_BARSLib>,
	public CProxyIBarRecordEvents< CBarRecord >,
	public IConnectionPointContainerImpl<CBarRecord>,
   public ISupportErrorInfo,
	public IDispatchImpl<IBendEvents, &IID_IBendEvents, &LIBID_BARSLib>
{
public:
   CBarRecord() :
#if defined _DEBUG
      m_Mark("XXXX")
#else
      m_Mark("")
#endif
	{
      m_NumEach       = 1;
      m_NumReqd       = 1;
      m_bEpoxy        = VARIANT_FALSE;
      m_bLumpSum      = VARIANT_FALSE;
      m_bSubstructure = VARIANT_FALSE;
      m_Use           = utLongitudinal;

      m_BendType      = 0;

      m_pBarlist      = 0;
   }

   virtual ~CBarRecord()
   {
   }

   void FinalRelease();

   void SetBarlist( CBarlist* pBarlist )
   {
      m_pBarlist = pBarlist;
   }

DECLARE_REGISTRY_RESOURCEID(IDR_BARRECORD)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBarRecord)
	COM_INTERFACE_ENTRY(IBarRecord)
 	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IBendEvents)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

private:
   CBarlist* m_pBarlist;
   CComBSTR m_Mark;
   CComBSTR m_Location;
   long m_NumEach;
   long m_NumReqd;
   VARIANT_BOOL m_bEpoxy;
   VARIANT_BOOL m_bLumpSum;
   VARIANT_BOOL m_bSubstructure;
   UseType m_Use;
   long m_BendType;

   DWORD m_PrimaryBendCookie;
   DWORD m_VariesBendCookie;

   CComPtr<IBend> m_pPrimaryBend;
   CComPtr<IBend> m_pVariesBend;

   CComPtr<IBarData> m_pBarData;

   CStatusMgr m_StatusMgr;

   void Disconnect(IBend* pBend,DWORD dwCookie);
   void UpdateStatus();


public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IBarRecord
public:
	STDMETHOD(get_BarData)(/*[out, retval]*/ IBarData* *pVal);
	STDMETHOD(put_BarData)(/*[in]*/ IBarData* newVal);
	STDMETHOD(get_Mass)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_Status)(/*[out, retval]*/ StatusType *pVal);
	STDMETHOD(get_Size)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_BendType)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_VariesBend)(/*[out, retval]*/ IBend* *pVal);
	STDMETHOD(put_VariesBend)(/*[in]*/ IBend* newVal);
	STDMETHOD(get_PrimaryBend)(/*[out, retval]*/ IBend* *pVal);
	STDMETHOD(put_PrimaryBend)(/*[in]*/ IBend* newVal);
	STDMETHOD(get_NumEach)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_NumEach)(/*[in]*/ long newVal);
	STDMETHOD(get_Varies)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(get_Epoxy)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Epoxy)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Substructure)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Substructure)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_LumpSum)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_LumpSum)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Use)(/*[out, retval]*/ UseType *pVal);
	STDMETHOD(put_Use)(/*[in]*/ UseType newVal);
	STDMETHOD(get_NumReqd)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_NumReqd)(/*[in]*/ long newVal);
	STDMETHOD(get_Location)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Location)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Mark)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Mark)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_StatusMessages)(/*[out, retval]*/ IStatusMessageCollection* *pVal);

BEGIN_CONNECTION_POINT_MAP(CBarRecord)
	CONNECTION_POINT_ENTRY(IID_IBarRecordEvents)
END_CONNECTION_POINT_MAP()

// IBendEvents
	STDMETHOD(OnBendChanged)()
	{
      Fire_OnBarRecordChanged(this);
      return S_OK;
	}
};

#endif //__BARRECORD_H_
