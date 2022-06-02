///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2022  Washington State Department of Transportation
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

   m_Bars = pBars;
   m_Bars.Advise( GetUnknown(), IID_IBarRecordCollectionEvents, &m_BarsCookie );
   InternalRelease();

   return S_OK;
}

void CGroup::FinalRelease()
{
   {
      // Increment the reference count so Unadvacing from the connection point doesn't
      // call this destructor again.
      long dwOldRef = m_dwRef;
      InternalAddRef();

      CComQIPtr<IConnectionPointContainer> pCPC(m_Bars);
      CComPtr<IConnectionPoint> pCP;
      pCPC->FindConnectionPoint(IID_IBarRecordCollectionEvents, &pCP);
      pCP->Unadvise(m_BarsCookie);

      ATLASSERT(dwOldRef == m_dwRef);
   }

   m_Bars.Release();
}

void CGroup::UpdateStatus()
{
   m_Superstructure.fill(0);
   m_SuperstructureEpoxy.fill(0);
   m_Substructure.fill(0);
   m_SubstructureEpoxy.fill(0);

   m_Status = stOK;

   long cBars;
   m_Bars->get_Count( &cBars );
   for ( long i = 0; i < cBars; i++ )
   {
      CComPtr<IBarRecord> pBar;
      m_Bars->get_Item( CComVariant(i), &pBar );

      StatusType status;
      pBar->get_Status( &status );
      if (m_Status < status)
      {
         m_Status = status;
      }

      MaterialType material;
      pBar->get_Material(&material);

      VARIANT_BOOL bEpoxy;
      VARIANT_BOOL bSubstructure;
      VARIANT_BOOL bVaries;
      pBar->get_Epoxy( &bEpoxy );
      pBar->get_Substructure( &bSubstructure );
      pBar->get_Varies(&bVaries);

      Float64 quantity = 0;
      if (material == D7957)
      {
         // the quantity for D7957/GFRP bars is length
         CComPtr<IBend> primary_bend;
         pBar->get_PrimaryBend(&primary_bend);

         Float64 length;
         primary_bend->get_Length(&length);

         if (bVaries == VARIANT_TRUE)
         {
            CComPtr<IBend> varies_bend;
            pBar->get_VariesBend(&varies_bend);
            Float64 varies_length;
            varies_bend->get_Length(&varies_length);

            // average length of primary and varies bends
            length = (length + varies_length) / 2;
         }

         long nReqd;
         pBar->get_NumReqd(&nReqd);
         quantity = nReqd * length;
      }
      else
      {
         pBar->get_Mass(&quantity);
      }

      if ( bSubstructure == VARIANT_TRUE )
      {
         if ( bEpoxy == VARIANT_TRUE )
         {
            m_SubstructureEpoxy[material] += quantity;
         }
         else
         {
            m_Substructure[material] += quantity;
         }
      }
      else
      {
         if ( bEpoxy == VARIANT_TRUE )
         {
            m_SuperstructureEpoxy[material] += quantity;
         }
         else
         {
            m_Superstructure[material] += quantity;
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

STDMETHODIMP CGroup::get_Quantity(MaterialType material, VARIANT_BOOL bEpoxy, VARIANT_BOOL bSubstructure, Float64* pVal)
{
   if (bSubstructure == VARIANT_TRUE)
   {
      if (bEpoxy == VARIANT_TRUE)
      {
         *pVal = m_SubstructureEpoxy[material];
      }
      else
      {
         *pVal = m_Substructure[material];
      }
   }
   else
   {
      if (bEpoxy == VARIANT_TRUE)
      {
         *pVal = m_SuperstructureEpoxy[material];
      }
      else
      {
         *pVal = m_Superstructure[material];
      }
   }

   return S_OK;
}

STDMETHODIMP CGroup::get_Status(StatusType *pVal)
{
   UpdateStatus();
   *pVal = m_Status;
   return S_OK;
}
