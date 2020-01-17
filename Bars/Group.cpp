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


// Group.cpp : Implementation of CGroup
#include "stdafx.h"
#include "Bars.h"
#include "Group.h"
#include "BarRecordCollection.h"

/////////////////////////////////////////////////////////////////////////////
// CGroup
void CGroup::SetBarlist(CBarlist* pBarlist)
{
   m_pBarlist = pBarlist;
   CBarRecordCollection* pColl = dynamic_cast<CBarRecordCollection*>(m_Bars.p);
   ATLASSERT( pColl != 0 );
   pColl->SetBarlist( m_pBarlist );
}

STDMETHODIMP CGroup::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IGroup,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CGroup::FinalConstruct()
{
   CComObject<CBarRecordCollection>* pBars;
   HRESULT hr = CComObject<CBarRecordCollection>::CreateInstance(&pBars);
   if ( FAILED(hr) )
      return hr;

   pBars->AddRef();
   m_Bars.Attach(pBars);
   m_Bars.Advise( GetUnknown(), IID_IBarRecordCollectionEvents, &m_BarsCookie );

   return S_OK;
}

void CGroup::FinalRelease()
{
   // Increment the reference count so Unadvacing from the connection point doesn't
   // call this destructor again.
   long dwOldRef = m_dwRef;
   InternalAddRef();

   CComQIPtr<IConnectionPointContainer> pCPC( m_Bars );
   CComPtr<IConnectionPoint> pCP;
   pCPC->FindConnectionPoint( IID_IBarRecordCollectionEvents,  &pCP );
   pCP->Unadvise( m_BarsCookie );

   ATLASSERT( dwOldRef == m_dwRef );

   m_Bars.Release();
}

void CGroup::UpdateStatus()
{
   m_SuperstructureMass = 0;
   m_SuperstructureMassEpoxy = 0;
   m_SubstructureMass = 0;
   m_SubstructureMassEpoxy = 0;

   m_Status = stOK;

   long cBars;
   m_Bars->get_Count( &cBars );
   for ( long i = 0; i < cBars; i++ )
   {
      CComPtr<IBarRecord> pBar;
      m_Bars->get_Item( CComVariant(i), &pBar );

      StatusType status;
      pBar->get_Status( &status );
      if ( m_Status < status )
         m_Status = status;

      VARIANT_BOOL bEpoxy;
      VARIANT_BOOL bSubstructure;
      pBar->get_Epoxy( &bEpoxy );
      pBar->get_Substructure( &bSubstructure );

      double mass;
      pBar->get_Mass( &mass );

      if ( bSubstructure == VARIANT_TRUE )
      {
         if ( bEpoxy == VARIANT_TRUE )
         {
            m_SubstructureMassEpoxy += mass;
         }
         else
         {
            m_SubstructureMass += mass;
         }
      }
      else
      {
         if ( bEpoxy == VARIANT_TRUE )
         {
            m_SuperstructureMassEpoxy += mass;
         }
         else
         {
            m_SuperstructureMass += mass;
         }
      }
   }
}

STDMETHODIMP CGroup::get_Name(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Name.Copy();
	return S_OK;
}

STDMETHODIMP CGroup::put_Name(BSTR newVal)
{
   // Check to see if the inbound group is already in use
   if ( m_pBarlist )
   {
      USES_CONVERSION;
      std::string strName( OLE2A(newVal) );
      if ( m_pBarlist->DoesGroupExist( strName ) )
      {
         // This group already exists.  There can't be duplicate groups
         return Error(IDS_E_DUPGROUP,IID_IGroup);
      }

      std::string strOldName( OLE2A(m_Name) );
      m_pBarlist->ReplaceGroup( strOldName, strName );
   }

   m_Name = newVal;
	Fire_OnGroupChanged(this);
   return S_OK;
}

STDMETHODIMP CGroup::get_BarRecords(IBarRecordCollection **pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Bars;
   (*pVal)->AddRef();

	return S_OK;
}

STDMETHODIMP CGroup::get_SuperstructureMass(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_SuperstructureMass;
	return S_OK;
}

STDMETHODIMP CGroup::get_SuperstructureMassEpoxy(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_SuperstructureMassEpoxy;
	return S_OK;
}

STDMETHODIMP CGroup::get_SubstructureMass(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_SubstructureMass;
	return S_OK;
}

STDMETHODIMP CGroup::get_SubstructureMassEpoxy(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_SubstructureMassEpoxy;
	return S_OK;
}

STDMETHODIMP CGroup::get_Status(StatusType *pVal)
{
   UpdateStatus();
   *pVal = m_Status;
   return S_OK;
}
