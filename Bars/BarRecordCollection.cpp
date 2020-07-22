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


// BarRecordCollection.cpp : Implementation of CBarRecordCollection
#include "stdafx.h"
#include "Bars.h"
#include "BarRecordCollection.h"
#include "BarRecord.h"

#include <Algorithm>

/////////////////////////////////////////////////////////////////////////////
// CBarRecordCollection

STDMETHODIMP CBarRecordCollection::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IBarRecordCollection,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


void CBarRecordCollection::FinalRelease()
{
   USES_CONVERSION;
   long dwOldRef = m_dwRef;

   // Release all the connection points
   for (auto& item : m_coll)
   {
      CComQIPtr<IBarRecord> pBarRecord(item.pdispVal);

      CComQIPtr<IConnectionPointContainer> pCPC(pBarRecord);
      CComPtr<IConnectionPoint> pCP;
      HRESULT hr = pCPC->FindConnectionPoint(IID_IBarRecordEvents, &pCP);
      ATLASSERT(SUCCEEDED(hr));

      CComBSTR bstrMark;
      pBarRecord->get_Mark(&bstrMark);
      auto found = m_Cookies.find(std::string(OLE2A(bstrMark)));
      ATLASSERT(found != m_Cookies.end());
      DWORD cookie = found->second;

      InternalAddRef();
      hr = pCP->Unadvise(cookie);  // decrements RefCount
      ATLASSERT(SUCCEEDED(hr));

      pBarRecord.Release();
      pCPC.Release();
      pCP.Release();

   }

   ATLASSERT( dwOldRef == m_dwRef );
}

/*
STDMETHODIMP CBarRecordCollection::get_Count(long *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CBarRecordCollection::get__NewEnum(LPUNKNOWN *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
*/
STDMETHODIMP CBarRecordCollection::get_Item(VARIANT varIndex, IBarRecord **pVal)
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
         CComQIPtr<IBarRecord> pRecord( varItem.pdispVal );
         CComBSTR name;
         pRecord->get_Mark( &name );
         LPCSTR lpszName = OLE2A(name);
         if ( strcmp(lpszTarget, lpszName ) == 0 )
         {
            return pRecord->QueryInterface( pVal );
         }
      }

      return E_INVALIDARG;
   }

   return E_INVALIDARG;
}

STDMETHODIMP CBarRecordCollection::Add(IBarRecord *pBarRecord)
{
	// TODO: Add your implementation code here
   CComVariant var;
   var.vt = VT_DISPATCH;
   pBarRecord->QueryInterface( &var.pdispVal );

   // Get the mark number
   CComBSTR bstrMark;
   pBarRecord->get_Mark( &bstrMark );
   std::string strMark = ConvertMark(bstrMark);

// Reducing duplicate mark records to a warning.
// Don't check for duplicates here. Check them in the bar record
//   if ( m_pBarlist->DoesBarRecordExist( strMark ) )
//   {
//      // This mark already exists.  There can't be duplicate marks
//      return Error(IDS_E_DUPMARK,IID_IBarRecordCollection);
//   }

   // We have to do a little back door implementation here.
   // If the bar record object is from our implementation, then
   // we should be able to cast to the C++ class.
   CBarRecord* pBR = dynamic_cast<CBarRecord*>( pBarRecord );
   if ( pBR != 0 )
   {
      pBR->SetBarlist( m_pBarlist );
   }

   m_coll.push_back(var);

   // Hookup to the connection point
   DWORD dwCookie;
   CComVariant real_var = m_coll.back();
   CComQIPtr<IBarRecord> rec(real_var.pdispVal);
   rec.Advise( GetUnknown(), IID_IBarRecordEvents, &dwCookie );
   m_Cookies.insert( std::make_pair(strMark,dwCookie) );

   // Avoid circular reference. Decrement RefCount in a thread safe way
   InternalRelease();

   Fire_OnBarRecordAdded( pBarRecord );
   return S_OK;
}

long CBarRecordCollection::GetIndex(BSTR bstrMark)
{
   USES_CONVERSION;

   long index = -1;
   CComBSTR bstrTarget( bstrMark );
   LPCSTR lpszTarget = OLE2A( bstrTarget);

   std::vector<CComVariant>::iterator iter;
   for ( iter = m_coll.begin(); iter != m_coll.end(); iter++ )
   {
      index++;
      CComVariant varItem( *iter );
      CComQIPtr<IBarRecord> pRecord( varItem.pdispVal );
      CComBSTR bstrName;
      pRecord->get_Mark( &bstrName );
      LPCSTR lpszName = OLE2A(bstrName);
      if ( strcmp(lpszTarget, lpszName ) == 0 )
      {
         return index;
      }
   }

   return -1;
}

STDMETHODIMP CBarRecordCollection::Remove(VARIANT varIndex)
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

   // Attach to the bar record object, QIing for its IBarRecord interface
   CComQIPtr<IBarRecord> pBarRecord( m_coll[index].pdispVal );
   CComBSTR bstrMark;
   pBarRecord->get_Mark( &bstrMark );

   // Want to prevent ref-count from going too low.
   // This is because of circular ref problem in Add
   InternalAddRef();

   // Disconnection from connection point
   std::string strMark = ConvertMark( bstrMark );
   std::multimap<std::string,DWORD>::iterator found;
   found = m_Cookies.find( strMark );
   CComQIPtr<IConnectionPointContainer> pCPC( pBarRecord );
   CComPtr<IConnectionPoint> pCP;
   pCPC->FindConnectionPoint( IID_IBarRecordEvents, &pCP );
   pCP->Unadvise( (*found).second );
   m_Cookies.erase( found ); // remove this mark from the cookie map

   pBarRecord.Release();
   pCPC.Release();
   pCP.Release();

   m_coll.erase( m_coll.begin() + index );

   Fire_OnBarRecordRemoved(bstrMark);
   return S_OK;
}

STDMETHODIMP CBarRecordCollection::Move( VARIANT rec, MoveType mt, VARIANT target)
{
	// TODO: Add your implementation code here
   HRESULT hr;
   CComPtr<IBarRecord> pBarRec;
   CComPtr<IBarRecord> pTarget;

   // Get the source and target records
   hr = GetBarRecord( rec, &pBarRec );
   if ( FAILED(hr) )
      return hr;

   hr = GetBarRecord( target, &pTarget );
   if ( FAILED(hr) )
      return hr;

   // Get the index of the source
   long recIdx = -1;
   if ( rec.vt == VT_I2 || rec.vt == VT_I4 )
   {
      recIdx = (rec.vt == VT_I2 ? rec.iVal : rec.lVal);
   }
   else
   {
      CComBSTR bstrBarRec;
      pBarRec->get_Mark( &bstrBarRec );
      recIdx = GetIndex( bstrBarRec );
   }
   

   // Remove the record from the collection
   std::vector<CComVariant>::iterator iterRec  = m_coll.begin() + recIdx;
   m_coll.erase( iterRec );

   // Get the index of the target.
   // NOTE: This must come after the call to erase above. The erase operation
   //       will adjust all of the indices in the collection
   long targetIdx = -1;
   if ( target.vt == VT_I2 || target.vt == VT_I4 )
   {
      targetIdx = (target.vt == VT_I2 ? target.iVal : target.lVal);
   }
   else
   {
      CComBSTR bstrTarget;
      pTarget->get_Mark( &bstrTarget );
      targetIdx = GetIndex( bstrTarget );
   }

   // Insert the group into the collection at Target
   // vector::insert inserts before the element pointed to by the insert iterator
   // If inserting after, iterTarget must be advanced one.
   std::vector<CComVariant>::iterator iterTarget = m_coll.begin() + targetIdx;
   if ( mt == mtAfter && targetIdx < recIdx)
   {
      targetIdx++;
      iterTarget++;
   }

   CComVariant var( pBarRec );
   m_coll.insert( iterTarget, var );

   // Fire event
   Fire_OnBarRecordMoved( pBarRec, recIdx, targetIdx );

	return S_OK;
}

STDMETHODIMP CBarRecordCollection::MoveUp(VARIANT rec)
{
   HRESULT hr;
   long idx = -1;

   if ( rec.vt == VT_I2 || rec.vt == VT_I4 )
   {
      idx = (rec.vt == VT_I2 ? rec.iVal : rec.lVal);
   }
   else
   {
      CComPtr<IBarRecord> pBarRec;
      hr = GetBarRecord( rec, &pBarRec );
      if ( FAILED(hr) )
         return hr;

      CComBSTR bstrBarRec;
      pBarRec->get_Mark( &bstrBarRec );
      idx = GetIndex( bstrBarRec );
   }

   // If we are at the top of the list, do nothing
   if ( idx == 0 )
      return S_OK;

   // We are moving up so the target index is one less
   idx--;

   CComVariant target(idx);
   Move( rec, mtBefore, target );

	return S_OK;
}

STDMETHODIMP CBarRecordCollection::MoveDown(VARIANT rec)
{
   HRESULT hr;
   long idx = -1;

   if ( rec.vt == VT_I2 || rec.vt == VT_I4 )
   {
      idx = (rec.vt == VT_I2 ? rec.iVal : rec.lVal);
   }
   else
   {
      CComPtr<IBarRecord> pBarRec;
      hr = GetBarRecord( rec, &pBarRec );
      if ( FAILED(hr) )
         return hr;

      CComBSTR bstrBarRec;
      pBarRec->get_Mark( &bstrBarRec );
      idx = GetIndex( bstrBarRec );
   }

   // If we are at the bottom of the list, do nothing
   if ( idx == m_coll.size()-1 )
      return S_OK;

   // We are moving up so the target index is one more
   idx++;

   CComVariant target(idx);
   Move( rec, mtAfter, target );

	return S_OK;
}

std::string CBarRecordCollection::ConvertMark(BSTR bstrMark)
{
   USES_CONVERSION;
   return OLE2A(bstrMark);
}
HRESULT CBarRecordCollection::GetBarRecord(VARIANT key,IBarRecord** ppBarRec)
{
   HRESULT hr;
   CComPtr<IBarRecord> pBarRec;
   switch( key.vt )
   {
   case VT_I2:
   case VT_I4:
   case VT_BSTR:
      hr = get_Item( key, &pBarRec );
      break;

   case VT_UNKNOWN:
      hr = key.punkVal->QueryInterface( &pBarRec );
      break;

   case VT_DISPATCH:
      hr = key.pdispVal->QueryInterface( &pBarRec );
      break;
   }

   if ( SUCCEEDED(hr) )
   {
      *ppBarRec = pBarRec;
      (*ppBarRec)->AddRef();
   }
   else
      *ppBarRec = 0;

   return hr;
}

class AlphaSortPred
{
public:
   AlphaSortPred() {}
   bool operator()(CComVariant& varRec1,CComVariant& varRec2)
   {
      USES_CONVERSION;

      CComQIPtr<IBarRecord> pRec1(varRec1.pdispVal);
      CComQIPtr<IBarRecord> pRec2(varRec2.pdispVal);

      CComBSTR bstrMark1;
      pRec1->get_Mark( &bstrMark1 );
      std::string strMark1( OLE2A(bstrMark1) );

      CComBSTR bstrMark2;
      pRec2->get_Mark( &bstrMark2 );
      std::string strMark2( OLE2A(bstrMark2) );

      return strMark1 < strMark2;
   }
};

class NumericSortPred
{
public:
   NumericSortPred() {}
   bool operator()(CComVariant& varRec1,CComVariant& varRec2)
   {
      USES_CONVERSION;

      CComQIPtr<IBarRecord> pRec1(varRec1.pdispVal);
      CComQIPtr<IBarRecord> pRec2(varRec2.pdispVal);

      CComBSTR bstrMark1;
      pRec1->get_Mark( &bstrMark1 );
      std::string strMark1( OLE2A(bstrMark1) );

      CComBSTR bstrMark2;
      pRec2->get_Mark( &bstrMark2 );
      std::string strMark2( OLE2A(bstrMark2) );

      Float64 val1;
      char* pStop1;
      val1 = strtod( strMark1.c_str(), &pStop1 );

      Float64 val2;
      char* pStop2;
      val2 = strtod( strMark2.c_str(), &pStop2 );

      bool bLessThan = true;
      if ( val1 != 0 && val2 != 0 )
      {
         // both are numeric
         bLessThan = val1 < val2;
      }
      else
      {
         // one is alpha-numeric
         bLessThan = strMark1 < strMark2;
      }

      return bLessThan;
   }
};

STDMETHODIMP CBarRecordCollection::Sort()
{
	// TODO: Add your implementation code here
   std::sort( m_coll.begin(), m_coll.end(), NumericSortPred() );
   Fire_OnBarRecordsSorted();
	return S_OK;
}

STDMETHODIMP CBarRecordCollection::Replace(VARIANT varIndex, IBarRecord *pBarRecord)
{
   // varIndex can be:
   // 1.  The index of the bar record
   // 2.  The mark number
   // 3.  The IDispatch pointer for a bar record
   // 4.  The IUnknown pointer for a bar record

   long index = -1;

   if ( varIndex.vt == VT_BSTR )
   {
      index = GetIndex( varIndex.bstrVal );
   }
   else if ( varIndex.vt == VT_I2 || varIndex.vt == VT_I4 )
   {
      index = (varIndex.vt == VT_I2 ? varIndex.iVal : varIndex.lVal);
   }
   else if ( varIndex.vt == VT_DISPATCH || varIndex.vt == VT_UNKNOWN )
   {
      CComQIPtr<IBarRecord> pOldRecord( varIndex.vt == VT_DISPATCH ? varIndex.pdispVal : varIndex.punkVal );
      CComBSTR bstrMark;
      pOldRecord->get_Mark( &bstrMark );
      index = GetIndex( bstrMark );
   }
   else
   {
      return E_INVALIDARG;
   }

   if ( index == -1 )
      return E_INVALIDARG;
   
   // Get old mark number
   CComQIPtr<IBarRecord> pOldBarRecord( m_coll[index].pdispVal );
   CComBSTR bstrOldMark;
   pOldBarRecord->get_Mark( &bstrOldMark );
   std::string strOldMark = ConvertMark( bstrOldMark );

   // Get new mark number
   CComBSTR bstrNewMark;
   pBarRecord->get_Mark( &bstrNewMark );
   std::string strNewMark = ConvertMark( bstrNewMark );

//   // Check if the mark number of the new record is unique
//   if ( strNewMark != strOldMark && m_pBarlist->DoesBarRecordExist( strNewMark ) )
//   {
//      // This mark already exists.  There can't be duplicate marks
//      return Error(IDS_E_DUPMARK,IID_IBarRecordCollection);
//   }

   //
   // OK, we can make the replacement
   //

   // We have to do a little back door implementation here.
   // If the bar record object is from our implementation, then
   // we should be able to cast to the C++ class.
   CBarRecord* pBR = dynamic_cast<CBarRecord*>( pBarRecord );
   if ( pBR != 0 )
   {
      pBR->SetBarlist( m_pBarlist );
      m_pBarlist->ReplaceMarkNumber( strOldMark, strNewMark );
   }


   // Disconnection old record from connection point
   std::multimap<std::string,DWORD>::iterator found;
   found = m_Cookies.find( strOldMark );
   ATLASSERT( found != m_Cookies.end() );
   CComQIPtr<IConnectionPointContainer> pCPC( pOldBarRecord );
   CComPtr<IConnectionPoint> pCP;
   pCPC->FindConnectionPoint( IID_IBarRecordEvents, &pCP );
   pCP->Unadvise( (*found).second );
   m_Cookies.erase( found ); // remove this mark from the cookie map

   pOldBarRecord.Release();
   pCPC.Release();
   pCP.Release();

   // Hookup to the new record to the connection point
   DWORD dwCookie;
   CComPtr<IBarRecord> rec( pBarRecord );
   rec.Advise( GetUnknown(), IID_IBarRecordEvents, &dwCookie );
   m_Cookies.insert( std::make_pair(strNewMark,dwCookie) );

   // Replace the bar record
   m_coll[index] = CComVariant(pBarRecord);
   Fire_OnBarRecordChanged(pBarRecord);

   return S_OK;
}
