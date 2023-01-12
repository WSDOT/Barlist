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


// GroupCollection.cpp : Implementation of CGroupCollection
#include "stdafx.h"
#include "Bars.h"
#include "GroupCollection.h"
#include "Group.h"

/////////////////////////////////////////////////////////////////////////////
// CGroupCollection

void CGroupCollection::FinalRelease()
{
   long dwOldRef = m_dwRef;

   // Release all the connection points
   for(const auto& pair : m_Cookies)
   {
      long index = GetIndex(CComBSTR(pair.first.c_str()));
      DWORD cookie = pair.second; 
      CComQIPtr<IGroup> pGroup( m_coll[index].pdispVal );

      CComQIPtr<IConnectionPointContainer> pCPC( pGroup );
      CComPtr<IConnectionPoint> pCP;
      pCPC->FindConnectionPoint( IID_IGroupEvents, &pCP );

      InternalAddRef();
      pCP->Unadvise( cookie );  // decrements RefCount

      pGroup.Release();
      pCPC.Release();
      pCP.Release();
   }

   ATLASSERT( dwOldRef == m_dwRef );
   m_coll.clear();
}

STDMETHODIMP CGroupCollection::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IGroupCollection,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

/*
STDMETHODIMP CGroupCollection::get_Count(long *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CGroupCollection::get__NewEnum(LPUNKNOWN *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
*/

STDMETHODIMP CGroupCollection::get_Item(VARIANT varIndex, IGroup **pVal)
{
	// TODO: Add your implementation code here
   USES_CONVERSION;

   // TODO : Implement
   if ( varIndex.vt == VT_I2 || varIndex.vt == VT_I4 )
   {
      long index = (varIndex.vt == VT_I2 ? varIndex.iVal : varIndex.lVal);
      if ( index >= m_coll.size() )
         return E_INVALIDARG;

      CComVariant varItem( m_coll[index] );
      ATLASSERT(varItem.vt == VT_DISPATCH );
      return varItem.pdispVal->QueryInterface(pVal);
   }
   else if ( varIndex.vt == VT_BSTR )
   {
      CComBSTR target( varIndex.bstrVal );
      LPCSTR lpszTarget = OLE2A(target);

      std::vector<CComVariant>::iterator iter;
      for ( iter = m_coll.begin(); iter != m_coll.end(); iter++ )
      {
         CComVariant varItem( *iter );
         CComQIPtr<IGroup> pGroup( varItem.pdispVal );
         CComBSTR name;
         pGroup->get_Name( &name );
         LPCSTR lpszName = OLE2A(name);
         if ( strcmp(lpszTarget, lpszName ) == 0 )
         {
            return pGroup->QueryInterface( pVal );
         }
      }

      return E_INVALIDARG;
   }

   return E_INVALIDARG;
}

STDMETHODIMP CGroupCollection::Add(BSTR bstrGroup)
{
	// TODO: Add your implementation code here
   USES_CONVERSION;

   // You forgot to set the barlist.
   ATLASSERT(m_pBarlist != 0 );

   if ( m_pBarlist->DoesGroupExist( OLE2A(bstrGroup) ) )
   {
      // This group already exists.  There can't be duplicate groups
      return Error(IDS_E_DUPGROUP,IID_IGroupCollection);
   }

   CComObject<CGroup>* pGroup;
   HRESULT hr = CComObject<CGroup>::CreateInstance(&pGroup);
   if ( FAILED(hr) )
      return hr;

   CComPtr<IGroup> group = pGroup;

   group->put_Name(bstrGroup);

   // Must set barlist after put_Name.  If the groups isn't part of the
   // barlist yet.
   pGroup->SetBarlist( m_pBarlist );

   CComVariant var;
   var.vt = VT_DISPATCH;
   pGroup->QueryInterface( &var.pdispVal );

   m_coll.push_back(var);

   // Hookup to the connection point
   std::string strName = ConvertName(bstrGroup);
   DWORD dwCookie;
   group.Advise( GetUnknown(), IID_IGroupEvents, &dwCookie );
   m_Cookies.insert( std::make_pair(strName,dwCookie) );

   // Avoid circular reference. Decrement RefCount in a thread safe way
   InternalRelease();

	Fire_OnGroupAdded(group);
   return S_OK;
}

long CGroupCollection::GetIndex(BSTR bstrGroup)
{
   USES_CONVERSION;

   long index = -1;

   CComBSTR bstrTarget( bstrGroup );
   LPCSTR lpszTarget = OLE2A(bstrTarget);

   std::vector<CComVariant>::iterator iter;
   for ( iter = m_coll.begin(); iter != m_coll.end(); iter++ )
   {
      index++;
      CComVariant varItem( *iter );
      CComQIPtr<IGroup> pGroup( varItem.pdispVal );
      CComBSTR bstrName;
      pGroup->get_Name( &bstrName );
      LPCSTR lpszName = OLE2A(bstrName);
      if ( strcmp(lpszTarget, lpszName ) == 0 )
      {
         return index;
      }
   }

   return -1;
}

STDMETHODIMP CGroupCollection::Remove(VARIANT varIndex)
{
	// TODO: Add your implementation code here
   long index = -1;
   if ( varIndex.vt == VT_BSTR )
   {
      index = GetIndex( varIndex.bstrVal );
   }
   else if ( varIndex.vt == VT_I2 || varIndex.vt == VT_I4 )
   {
      index = (varIndex.vt == VT_I2 ? varIndex.iVal : varIndex.lVal);
   }
   else
   {
      return E_INVALIDARG;
   }

   if ( index < 0 || m_coll.size() <= index )
   {
      return E_INVALIDARG;
   }

   // Attach to the group object, QIing for its IGroup interface
   CComQIPtr<IGroup> pGroup( m_coll[index].pdispVal );

   // Remove each bar in the group.
   // Before all the connection points are dismantled, we need to remove
   // each bar record in this group.  We need to do it before Unadvise because
   // the barlist object needs to hear the events so it can update the management of
   // mark numbers
   CComPtr<IBarRecordCollection> pBars;
   pGroup->get_BarRecords( &pBars );
   long nBars;
   pBars->get_Count(&nBars);
   for ( long i = nBars-1; i >= 0; i-- )
   {
      pBars->Remove( CComVariant(i) );
   }

   CComBSTR bstrName;
   pGroup->get_Name( &bstrName );
   std::string strName = ConvertName( bstrName );

   // Replace the ref count we took away because of the connection point in Add
   InternalAddRef();

   // Disconnection from the connection point
   std::map<std::string,DWORD>::iterator found;
   found = m_Cookies.find( strName );
   CComQIPtr<IConnectionPointContainer> pCPC( pGroup );
   CComPtr<IConnectionPoint> pCP;
   pCPC->FindConnectionPoint( IID_IGroupEvents, &pCP );
   pCP->Unadvise( (*found).second );
   m_Cookies.erase( found ); // Remove the group from the cookie map

   // Release the group
   pGroup.Release();

   // Remove the group from the collection (group is still alive)
   m_coll.erase( m_coll.begin() + index );

   // Fire the event
   Fire_OnGroupRemoved( bstrName );

   return S_OK;
}

std::string CGroupCollection::ConvertName(BSTR group)
{
   USES_CONVERSION;
   return OLE2A( group );
}

STDMETHODIMP CGroupCollection::Move(VARIANT grp, MoveType mt, VARIANT target)
{
	// TODO: Add your implementation code here
   HRESULT hr;
   CComPtr<IGroup> pGroup;
   CComPtr<IGroup> pTarget;

   // Get the source and target groups
   hr = GetGroup( grp, &pGroup );
   if ( FAILED(hr) )
      return hr;

   hr = GetGroup( target, &pTarget );
   if ( FAILED(hr) )
      return hr;

   // Get the name and index of the source
   CComBSTR bstrGroup;
   pGroup->get_Name( &bstrGroup );
   long grpIdx = GetIndex( bstrGroup );

   // Remove the group from the collection
   std::vector<CComVariant>::iterator iterGroup  = m_coll.begin() + grpIdx;
   m_coll.erase( iterGroup );

   // Get the name and index of the target.
   // NOTE: This must come after the call to erase above. The erase operation
   //       will adjust all of the indices in the collection
   CComBSTR bstrTarget;
   pTarget->get_Name( &bstrTarget );
   long targetIdx = GetIndex( bstrTarget );

   // Insert the group into the collection at Target
   // vector::insert inserts before the element pointed to by the insert iterator
   // If inserting after, iterTarget must be advanced one.
   std::vector<CComVariant>::iterator iterTarget = m_coll.begin() + targetIdx;
   if ( mt == mtAfter )
   {
      targetIdx++;
      iterTarget++;
   }

   CComVariant var( pGroup );
   m_coll.insert( iterTarget, var );

   // Fire event
   Fire_OnGroupMoved( pGroup, grpIdx, targetIdx );

	return S_OK;
}

STDMETHODIMP CGroupCollection::MoveUp(VARIANT grp)
{
   HRESULT hr;
   CComPtr<IGroup> pGroup;
   hr = GetGroup( grp, &pGroup );
   if ( FAILED(hr) )
      return hr;

   CComBSTR bstrGroup;
   pGroup->get_Name( &bstrGroup );
   long idx = GetIndex( bstrGroup );

   // If we are at the top of the list, do nothing
   if ( idx == 0 )
      return S_OK;

   // We are moving up so the target index is one less
   idx--;

   CComVariant target = m_coll[idx];
   Move( grp, mtBefore, target );

	return S_OK;
}

STDMETHODIMP CGroupCollection::MoveDown(VARIANT grp)
{
   HRESULT hr;
   CComPtr<IGroup> pGroup;
   hr = GetGroup( grp, &pGroup );
   if ( FAILED(hr) )
      return hr;

   CComBSTR bstrGroup;
   pGroup->get_Name( &bstrGroup );
   long idx = GetIndex( bstrGroup );

   // If we are at the bottom of the list, do nothing
   if ( idx == m_coll.size()-1 )
      return S_OK;

   // We are moving up so the target index is one more
   idx++;

   CComVariant target = m_coll[idx];
   Move( grp, mtAfter, target );

	return S_OK;
}

HRESULT CGroupCollection::GetGroup(VARIANT key,IGroup** ppGroup)
{
   HRESULT hr;
   CComPtr<IGroup> pGroup;
   switch( key.vt )
   {
   case VT_I2:
   case VT_I4:
   case VT_BSTR:
      hr = get_Item( key, &pGroup );
      break;

   case VT_UNKNOWN:
      hr = key.punkVal->QueryInterface( &pGroup );
      break;

   case VT_DISPATCH:
      hr = key.pdispVal->QueryInterface( &pGroup );
      break;
   }

   if ( SUCCEEDED(hr) )
   {
      *ppGroup = pGroup;
      (*ppGroup)->AddRef();
   }
   else
      *ppGroup = 0;

   return hr;
}
