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


// Barlist.h : Declaration of the CBarlist

#ifndef __BARLIST_H_
#define __BARLIST_H_

#include "resource.h"       // main symbols
#include "BarsCP.h"
#include <set>
#include <string>

/////////////////////////////////////////////////////////////////////////////
// CBarlist
class ATL_NO_VTABLE CBarlist : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBarlist, &CLSID_Barlist>,
	public IDispatchImpl<IBarlist, &IID_IBarlist, &LIBID_BARSLib>,
	public CProxyIBarlistEvents< CBarlist >,
	public IConnectionPointContainerImpl<CBarlist>,
	public IDispatchImpl<IGroupCollectionEvents, &IID_IGroupCollectionEvents, &LIBID_BARSLib>
{
public:
   CBarlist() :
   m_Project("Barlist"),
   m_JobNumber(""),
   m_Engineer(""),
   m_Company(""),
   m_Comments("")
	{
      m_RetainingWall    = 0;
      m_BridgeGrateInlet = 0;
      m_TrafficBarrier   = 0;

      m_SubstructureMass = 0;
      m_SubstructureMassEpoxy = 0;
      m_SuperstructureMass = 0;
      m_SuperstructureMassEpoxy = 0;

      m_Status = stOK;
	}

   HRESULT FinalConstruct();
   void FinalRelease();

   bool BarRecordMultiUse(const std::string& strMark) const
   {
      std::multiset<std::string>::const_iterator first = m_Marks.lower_bound(strMark);
      std::multiset<std::string>::const_iterator last  = m_Marks.upper_bound(strMark);

      // Mark wasn't found so it can't be used more than once
      if ( first == m_Marks.end() )
         return false;

      if ( first == last && *first != strMark )
         return false;

      // If iterators are at the same location then there is only one occurance
      // of the mark number
      if ( ++first == last )
         return false;

      // At least two occurances of the mark number were found
      return true;
   }

   void ReplaceMarkNumber(const std::string& strOldMark,const std::string& strNewMark)
   {
      std::multiset<std::string>::iterator found = m_Marks.find(strOldMark);
      if ( found != m_Marks.end() )
      {
         m_Marks.erase( found );
         m_Marks.insert( strNewMark );
      }
   }

   void RemoveMarkNumber(const std::string& strMark)
   {
      std::multiset<std::string>::iterator found = m_Marks.find(strMark);
      if ( found != m_Marks.end() )
      {
         m_Marks.erase( found );
      }
   }

   bool DoesGroupExist(const std::string& strGroup) const
   {
      std::set<std::string>::const_iterator found = m_GroupNames.find(strGroup);
      return found != m_GroupNames.end();
   }

   void ReplaceGroup(const std::string& strOldGroup,const std::string& strNewGroup)
   {
      m_GroupNames.erase( strOldGroup );
      m_GroupNames.insert( strNewGroup );
   }

DECLARE_REGISTRY_RESOURCEID(IDR_BARLIST)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBarlist)
	COM_INTERFACE_ENTRY(IBarlist)
 	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IGroupCollectionEvents)
END_COM_MAP()

private:
   void Update();

   CComPtr<IGroupCollection> m_Groups;
   DWORD m_GroupCookie;

   double m_RetainingWall;
   double m_BridgeGrateInlet;
   double m_TrafficBarrier;
   double m_SubstructureMass;
   double m_SubstructureMassEpoxy;
   double m_SuperstructureMass;
   double m_SuperstructureMassEpoxy;

   StatusType m_Status;

   // Project Properties
   CComBSTR m_Project;
   CComBSTR m_JobNumber;
   CComBSTR m_Engineer;
   CComBSTR m_Company;
   CComBSTR m_Comments;

   // Collection of mark numbers so that we can easily detect duplicate
   // mark numbers
   std::multiset<std::string> m_Marks;
   std::set<std::string> m_GroupNames;

// IBarlist
public:
	STDMETHOD(get_SubstructureMassEpoxy)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_SubstructureMass)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_SuperstructureMassEpoxy)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_SuperstructureMass)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_RetainingWallQuantity)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_RetainingWallQuantity)(/*[in]*/ double newVal);
	STDMETHOD(get_BridgeGrateInletQuantity)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_BridgeGrateInletQuantity)(/*[in]*/ double newVal);
	STDMETHOD(get_TrafficBarrierQuantity)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_TrafficBarrierQuantity)(/*[in]*/ double newVal);
	STDMETHOD(get_Groups)(/*[out, retval]*/ IGroupCollection* *pVal);
public :
	STDMETHOD(get_Comments)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Comments)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Company)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Company)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Engineer)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Engineer)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_JobNumber)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_JobNumber)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Project)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Project)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Status)(/*[out, retval]*/ StatusType *pVal);

BEGIN_CONNECTION_POINT_MAP(CBarlist)
	CONNECTION_POINT_ENTRY(IID_IBarlistEvents)
END_CONNECTION_POINT_MAP()

// IGroupCollectionEvents
	STDMETHOD(OnGroupAdded)(IGroup* pGroup)
	{
      USES_CONVERSION;
      CComBSTR bstrName;
      pGroup->get_Name( &bstrName );
      std::string strName( OLE2A(bstrName) );

      // If this assert fires,  a duplicate mark number has been added
      ATLASSERT( !DoesGroupExist(strName) );

      m_GroupNames.insert( strName );

      Fire_OnGroupAdded(pGroup);
      return S_OK;
	}

	STDMETHOD(OnGroupRemoved)(BSTR Name)
	{
      USES_CONVERSION;
      std::string strName = OLE2A(Name);

      // This group better exist.  If it doesn't then
      // the collection of groups is messed up
      ATLASSERT( DoesGroupExist(strName) );

      m_GroupNames.erase( strName );

      Update();
      Fire_OnGroupRemoved(Name);
      return S_OK;
	}

   STDMETHOD(OnGroupChanged)(IGroup* pGroup)
   {
#if defined _DEBUG
      // This group better exist.
      USES_CONVERSION;
      CComBSTR bstrName;
      pGroup->get_Name( &bstrName );
      std::string strName = OLE2A(bstrName);
      ATLASSERT( DoesGroupExist( strName ) );
#endif

      Fire_OnGroupChanged(pGroup);
      return S_OK;
   }

   STDMETHOD(OnGroupMoved)(IGroup *pGroup, long idxFrom, long idxTo)
   {
      Fire_OnGroupMoved( pGroup, idxFrom, idxTo );
	   return S_OK;
   }

	STDMETHOD(OnBarRecordAdded)(IGroup* pGroup,IBarRecord* pNewRecord)
   {
      USES_CONVERSION;
      CComBSTR bstrMark;
      pNewRecord->get_Mark( &bstrMark );
      std::string strMark = OLE2A(bstrMark);

      // If this assert fires,  a duplicate mark number has been added
//      ATLASSERT( !DoesBarRecordExist(strMark) );

      m_Marks.insert( strMark );

      Update();
      Fire_OnBarRecordAdded(pGroup,pNewRecord);
      return S_OK;
   }

	STDMETHOD(OnBarRecordChanged)(IGroup* pGroup,IBarRecord* pBarRecord)
   {
#if defined _DEBUG
      // This mark number better exist.
      // If not, there is an error in IBarRecord::put_Mark();
      USES_CONVERSION;
      CComBSTR bstrMark;
      pBarRecord->get_Mark( &bstrMark );
      std::string strMark = OLE2A(bstrMark);
//      ATLASSERT( DoesBarRecordExist( strMark ) );
#endif

      Update();
      Fire_OnBarRecordChanged(pGroup,pBarRecord);
      return S_OK;
   }

	STDMETHOD(OnBarRecordRemoved)(IGroup* pGroup,BSTR bstrMark)
   {
      USES_CONVERSION;
      std::string strMark = OLE2A(bstrMark);

      // This mark number better exist.  If it doesn't then
      // the collection of mark numbers is messed up
//      ATLASSERT( DoesBarRecordExist(strMark) );

      std::multiset<std::string>::iterator found = m_Marks.find(strMark);
      ATLASSERT( found != m_Marks.end() );
      m_Marks.erase( found );

      Update();
      Fire_OnBarRecordRemoved(pGroup,bstrMark);
      return S_OK;
   }

   STDMETHOD(OnBarRecordsSorted)(/*[in]*/ IGroup* pGroup)
   {
      Fire_OnBarRecordsSorted(pGroup);
      return S_OK;
   }
	STDMETHOD(OnBarRecordMoved)(IGroup* pGroup,IBarRecord* pBarRecord,long idxFrom,long idxTo)
   {
      Fire_OnBarRecordMoved(pGroup,pBarRecord,idxFrom,idxTo);
      return S_OK;
   }
};

#endif //__BARLIST_H_
