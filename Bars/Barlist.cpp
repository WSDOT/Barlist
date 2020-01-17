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

   pGroups->AddRef();
   m_Groups.Attach(pGroups);
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
   long dwOldRef = m_dwRef;
   InternalAddRef();
   CComQIPtr<IConnectionPointContainer> pCPC( m_Groups );
   CComPtr<IConnectionPoint> pCP;
   pCPC->FindConnectionPoint( IID_IGroupCollectionEvents,  &pCP );
   pCP->Unadvise( m_GroupCookie ); // decrements RefCount

   ATLASSERT( dwOldRef == m_dwRef );

   m_Groups.Release();

   m_Marks.clear();
   m_GroupNames.clear();
}

void CBarlist::Update()
{
   m_SuperstructureMass = 0;
   m_SuperstructureMassEpoxy = 0;
   m_SubstructureMass = 0;
   m_SubstructureMassEpoxy = 0;

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

      double mass;
      pGroup->get_SubstructureMassEpoxy( &mass );
      m_SubstructureMassEpoxy += mass;

      pGroup->get_SubstructureMass( &mass );
      m_SubstructureMass += mass;

      pGroup->get_SuperstructureMassEpoxy( &mass );
      m_SuperstructureMassEpoxy += mass;

      pGroup->get_SuperstructureMass( &mass );
      m_SuperstructureMass += mass;
   }
}

STDMETHODIMP CBarlist::get_Groups(IGroupCollection **pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Groups;
   (*pVal)->AddRef(); // I think this one is leaked
	return S_OK;
}

STDMETHODIMP CBarlist::get_TrafficBarrierQuantity(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_TrafficBarrier;
	return S_OK;
}

STDMETHODIMP CBarlist::put_TrafficBarrierQuantity(double newVal)
{
	// TODO: Add your implementation code here
   m_TrafficBarrier = newVal;
	Fire_OnNotIncludedQuantitiesChanged();
   return S_OK;
}

STDMETHODIMP CBarlist::get_BridgeGrateInletQuantity(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_BridgeGrateInlet;
	return S_OK;
}

STDMETHODIMP CBarlist::put_BridgeGrateInletQuantity(double newVal)
{
	// TODO: Add your implementation code here
   m_BridgeGrateInlet = newVal;
	Fire_OnNotIncludedQuantitiesChanged();
   return S_OK;
}

STDMETHODIMP CBarlist::get_RetainingWallQuantity(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_RetainingWall;
	return S_OK;
}

STDMETHODIMP CBarlist::put_RetainingWallQuantity(double newVal)
{
	// TODO: Add your implementation code here
   m_RetainingWall = newVal;
	Fire_OnNotIncludedQuantitiesChanged();
   return S_OK;
}

STDMETHODIMP CBarlist::get_SuperstructureMass(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_SuperstructureMass;
	return S_OK;
}

STDMETHODIMP CBarlist::get_SuperstructureMassEpoxy(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_SuperstructureMassEpoxy;
	return S_OK;
}

STDMETHODIMP CBarlist::get_SubstructureMass(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_SubstructureMass;
	return S_OK;
}

STDMETHODIMP CBarlist::get_SubstructureMassEpoxy(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_SubstructureMassEpoxy;
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
