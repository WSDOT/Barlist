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


// Barlist.cpp : Implementation of CBarlist
#include "stdafx.h"
#include "Bars.h"
#include "Barlist.h"
#include "GroupCollection.h"

/////////////////////////////////////////////////////////////////////////////
// CBarlist
HRESULT CBarlist::FinalConstruct()
{
   CComObject<CGroupCollection>* pGroups;
   HRESULT hr = CComObject<CGroupCollection>::CreateInstance(&pGroups);
   if ( FAILED(hr) )
      return hr;

   pGroups->SetBarlist( this );

   m_Groups = pGroups;
   m_Groups.Advise( GetUnknown(), IID_IGroupCollectionEvents, &m_GroupCookie );

   // Because of the connection point, there is a circular reference between
   // this barlist and the group collection object.
   // Reducing the reference count on the barlist object will create a weak
   // reference from the collection to the barlist.
   // Decrement RefCount in a thread safe way
   InternalRelease();

   return S_OK;
}

void CBarlist::FinalRelease()
{
   {
      long dwOldRef = m_dwRef;
      InternalAddRef();
      CComQIPtr<IConnectionPointContainer> pCPC(m_Groups);
      CComPtr<IConnectionPoint> pCP;
      pCPC->FindConnectionPoint(IID_IGroupCollectionEvents, &pCP);
      pCP->Unadvise(m_GroupCookie); // decrements RefCount
      ATLASSERT(dwOldRef == m_dwRef);
   }

   m_Groups.Release();

   m_Marks.clear();
   m_GroupNames.clear();
}

void CBarlist::Update()
{
   m_Superstructure.fill(0);
   m_SuperstructureEpoxy.fill(0);
   m_Substructure.fill(0);
   m_SubstructureEpoxy.fill(0);

   m_Status = stOK;

   long cGroups;
   m_Groups->get_Count( &cGroups );
   for ( long i = 0; i < cGroups; i++ )
   {
      CComPtr<IGroup> pGroup;
      m_Groups->get_Item( CComVariant(i), &pGroup );

      StatusType status;
      pGroup->get_Status( &status );
      if ( m_Status < status )
         m_Status = status;

      auto n = m_Superstructure.size();
      for (auto i = 0; i < n; i++)
      {
         MaterialType material = (MaterialType)i;
         Float64 quantity;
         pGroup->get_Quantity(material, VARIANT_TRUE /*epoxy*/, VARIANT_TRUE /*substructure*/, &quantity);
         m_SubstructureEpoxy[material] += quantity;

         pGroup->get_Quantity(material, VARIANT_FALSE /*epoxy*/, VARIANT_TRUE /*substructure*/, &quantity);
         m_Substructure[material] += quantity;

         pGroup->get_Quantity(material, VARIANT_TRUE /*epoxy*/, VARIANT_FALSE /*substructure*/, &quantity);
         m_SuperstructureEpoxy[material] += quantity;

         pGroup->get_Quantity(material, VARIANT_FALSE /*epoxy*/, VARIANT_FALSE /*substructure*/, &quantity);
         m_Superstructure[material] += quantity;
      }
   }
}

STDMETHODIMP CBarlist::get_Groups(IGroupCollection **pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Groups;
   (*pVal)->AddRef(); // I think this one is leaked
	return S_OK;
}

STDMETHODIMP CBarlist::get_Quantity(MaterialType material, VARIANT_BOOL bEpoxy, VARIANT_BOOL bSubstructure, Float64* pVal)
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

STDMETHODIMP CBarlist::get_Status(StatusType *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Status;
	return S_OK;
}

STDMETHODIMP CBarlist::get_Project(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Project.Copy();
	return S_OK;
}

STDMETHODIMP CBarlist::put_Project(BSTR newVal)
{
	// TODO: Add your implementation code here
   m_Project = newVal;
	return S_OK;
}

STDMETHODIMP CBarlist::get_JobNumber(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_JobNumber.Copy();
	return S_OK;
}

STDMETHODIMP CBarlist::put_JobNumber(BSTR newVal)
{
	// TODO: Add your implementation code here
   m_JobNumber = newVal;
	return S_OK;
}

STDMETHODIMP CBarlist::get_Engineer(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Engineer.Copy();
	return S_OK; 
}

STDMETHODIMP CBarlist::put_Engineer(BSTR newVal)
{
	// TODO: Add your implementation code here
   m_Engineer = newVal;
	return S_OK;
}

STDMETHODIMP CBarlist::get_Company(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Company.Copy();
	return S_OK;
}

STDMETHODIMP CBarlist::put_Company(BSTR newVal)
{
	// TODO: Add your implementation code here
   m_Company = newVal;
	return S_OK;
}

STDMETHODIMP CBarlist::get_Comments(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Comments.Copy();
	return S_OK;
}

STDMETHODIMP CBarlist::put_Comments(BSTR newVal)
{
	// TODO: Add your implementation code here
   m_Comments = newVal;
	return S_OK;
}
