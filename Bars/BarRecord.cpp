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


// BarRecord.cpp : Implementation of CBarRecord
#include "stdafx.h"
#include "Bars.h"
#include "BarRecord.h"

/////////////////////////////////////////////////////////////////////////////
// CBarRecord

STDMETHODIMP CBarRecord::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IBarRecord,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


void CBarRecord::FinalRelease()
{
   USES_CONVERSION;
   // Disconnection from any connection points we might have

   // NOTE: There is a circular reference between this bar record and its
   //       bends.  This reference is caused by the connection points.  This
   //       bend record holds a reference to the bend because it is a subscriber.
   //       At the same time, the bend holds a reference to this bar record because
   //       it is a publisher.  In order to avoid a double-delete situation, we increment
   //       the reference count for each connection point.
   long dwOldRef = m_dwRef;

   if (m_pPrimaryBend)
   {
      InternalAddRef(); // Increment the RefCount
   }

   if (m_pVariesBend)
   {
      InternalAddRef(); // Increment the RefCount
   }

   Disconnect( m_pPrimaryBend, m_PrimaryBendCookie );
   Disconnect( m_pVariesBend, m_VariesBendCookie );

   ATLASSERT( m_dwRef == dwOldRef );
}

void CBarRecord::Disconnect(IBend* pBend,DWORD dwCookie)
{
   // If we have a previous bend, disconnection from it.
   if ( pBend )
   {
      CComQIPtr<IConnectionPointContainer> pCPC(pBend);
      CComPtr<IConnectionPoint> pCP;
      pCPC->FindConnectionPoint( IID_IBendEvents, &pCP );
      pCP->Unadvise( dwCookie );
   }
}

STDMETHODIMP CBarRecord::get_Material(/*[out, retval]*/ MaterialType *pVal)
{
   *pVal = m_Material;
   return S_OK;
}

STDMETHODIMP CBarRecord::put_Material(MaterialType newVal)
{
   m_Material = newVal;
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_Mark(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Mark.Copy();
	return S_OK;
}

STDMETHODIMP CBarRecord::put_Mark(BSTR newVal)
{
	// TODO: Add your implementation code here

   if ( m_pBarlist )
   {
      // Check to see if the inbound mark number is already in use
      USES_CONVERSION;
      std::string strMark( OLE2A(newVal) );
      std::string strOldMark( OLE2A(m_Mark) );
      m_pBarlist->ReplaceMarkNumber( strOldMark, strMark );
   }

   m_Mark = newVal;
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_Location(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Location.Copy();
	return S_OK;
}

STDMETHODIMP CBarRecord::put_Location(BSTR newVal)
{
	// TODO: Add your implementation code here
   m_Location = newVal;
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_NumReqd(long *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_NumReqd;
	return S_OK;
}

STDMETHODIMP CBarRecord::put_NumReqd(long newVal)
{
	// TODO: Add your implementation code here
   m_NumReqd = newVal;
   m_StatusMgr.ResetStatusMsgs();
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_Use(UseType *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Use;
	return S_OK;
}

STDMETHODIMP CBarRecord::put_Use(UseType newVal)
{
	// TODO: Add your implementation code here
   m_Use = newVal;
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_Substructure(VARIANT_BOOL *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_bSubstructure;
	return S_OK;
}

STDMETHODIMP CBarRecord::put_Substructure(VARIANT_BOOL newVal)
{
	// TODO: Add your implementation code here
   m_bSubstructure = newVal;
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_Epoxy(VARIANT_BOOL *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_bEpoxy;
	return S_OK;
}

STDMETHODIMP CBarRecord::put_Epoxy(VARIANT_BOOL newVal)
{
	// TODO: Add your implementation code here
   m_bEpoxy = newVal;
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_Varies(VARIANT_BOOL *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_pVariesBend.p == 0 ? VARIANT_FALSE : VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CBarRecord::get_NumEach(long *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_NumEach;
	return S_OK;
}

STDMETHODIMP CBarRecord::put_NumEach(long newVal)
{
	// TODO: Add your implementation code here
   m_NumEach = newVal;
   m_StatusMgr.ResetStatusMsgs();
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_PrimaryBend(IBend **pVal)
{
	// TODO: Add your implementation code here
   (*pVal) = m_pPrimaryBend;
   (*pVal)->AddRef();
	return S_OK;
}

STDMETHODIMP CBarRecord::put_PrimaryBend(IBend *newVal)
{
	// TODO: Add your implementation code here
   // Disconnection from connection point
   Disconnect( m_pPrimaryBend, m_PrimaryBendCookie );

   // Attach to the interface
   m_pPrimaryBend.Attach(newVal);
   newVal->AddRef();
   m_pPrimaryBend->put_BarRecord(this);

   // Attach to the connection point
   m_pPrimaryBend.Advise( GetUnknown(), IID_IBendEvents, &m_PrimaryBendCookie );

   // Avoid circular reference. Decrement RefCount in a thread safe way
   InternalRelease();

   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_VariesBend(IBend **pVal)
{
	// TODO: Add your implementation code here
   if ( m_pVariesBend )
   {
      (*pVal) = m_pVariesBend;
      (*pVal)->AddRef();
   }

	return S_OK;
}

STDMETHODIMP CBarRecord::put_VariesBend(IBend *newVal)
{
	// TODO: Add your implementation code here

   // Disconnection from connection point
   Disconnect( m_pVariesBend, m_VariesBendCookie );


   if ( newVal == 0 )
   {
      // Varies bend has been removed.
      // clear out status message because no each/no required problem
      // can't exists if there isn't a varies bend.
      m_StatusMgr.ResetStatusMsgs();
      m_NumEach = 1;
      m_pVariesBend = 0;
      return S_OK;
   }

   // Attach to the interface
   m_pVariesBend.Attach(newVal);
   newVal->AddRef();
   m_pVariesBend->put_BarRecord(this);

   // Attach to the connection point.
   m_pVariesBend.Advise( GetUnknown(), IID_IBendEvents, &m_VariesBendCookie );

   // Avoid circular reference. Decrement RefCount in a thread safe way
   InternalRelease();
   
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

STDMETHODIMP CBarRecord::get_BendType(long *pVal)
{
	// TODO: Add your implementation code here
   return m_pPrimaryBend->get_BendType(pVal);
}

STDMETHODIMP CBarRecord::get_Size(BSTR *pVal)
{
	// TODO: Add your implementation code here
   return m_pBarData->get_Name(pVal);
}


STDMETHODIMP CBarRecord::get_Status(StatusType *pVal)
{
	// TODO: Add your implementation code here
   UpdateStatus();

   StatusType recordStatus = m_StatusMgr.GetStatusLevel();

   StatusType primaryStatus;
   m_pPrimaryBend->get_Status( &primaryStatus );
   
   StatusType variesStatus = stOK;
   if (m_pVariesBend)
   {
      m_pVariesBend->get_Status(&variesStatus);
   }

   StatusType status = max(recordStatus,max(primaryStatus,variesStatus));
   *pVal = status;
   return S_OK;
}

STDMETHODIMP CBarRecord::get_Mass(Float64 *pVal)
{
	// TODO: Add your implementation code here
   Float64 primaryLength;
   Float64 variesLength;

   m_pPrimaryBend->get_Length(&primaryLength);
   if (m_pVariesBend)
   {
      m_pVariesBend->get_Length(&variesLength);
   }
   else
   {
      variesLength = primaryLength;
   }

   Float64 length = (primaryLength + variesLength)/2;
   
   Float64 unitMass;
   m_pBarData->get_Mass(&unitMass);

   Float64 mass = m_NumReqd * length * unitMass;

   // If status is error, don't figure the mass.
   StatusType status;
   get_Status(&status);

   *pVal = status == stError ? 0.00 : mass;

   return S_OK;
}

STDMETHODIMP CBarRecord::get_BarData(IBarData **pVal)
{
	// TODO: Add your implementation code here
   (*pVal) = m_pBarData;
   (*pVal)->AddRef();
	return S_OK;
}

STDMETHODIMP CBarRecord::put_BarData(IBarData *newVal)
{
	// TODO: Add your implementation code here
   m_pBarData.Attach( newVal );
   newVal->AddRef();
   Fire_OnBarRecordChanged(this);
   return S_OK;
}

void CBarRecord::UpdateStatus()
{
   USES_CONVERSION;

   m_StatusMgr.ResetStatusMsgs();

   std::string strMark = OLE2A( m_Mark );
   // Check if the mark number is in use elsewhere
   if ( m_pBarlist->BarRecordMultiUse( strMark ) )
   {
      m_StatusMgr.SetStatusLevel( stWarning );
      CComBSTR msg;
      msg.LoadString( WARN_DUPMARKNUMBER );
      m_StatusMgr.AddStatusMsg( msg, CComVariant(m_Mark), CComVariant() );
   }

   // Check if the number required is evenly divided by the number each
   // if this is a varies bend record.
   VARIANT_BOOL bVaries;
   get_Varies( &bVaries );
   if ( bVaries == VARIANT_TRUE )
   {
      long remainder = m_NumReqd % m_NumEach;
      if ( remainder != 0 )
      {
         m_StatusMgr.SetStatusLevel( stError );
         CComBSTR msg;
         msg.LoadString( ERR_NOEACHNOTMULTIPLEOFNOREQD );
         m_StatusMgr.AddStatusMsg( msg, CComVariant(), CComVariant() );
      }
   }
}

STDMETHODIMP CBarRecord::get_StatusMessages(IStatusMessageCollection **pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_StatusMgr.GetStatusMessages();
	return S_OK;
}
