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


#ifndef _BARSCP_H_
#define _BARSCP_H_

template <class T>
class CProxyIBendEvents : public IConnectionPointImpl<T, &IID_IBendEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnBendChanged()
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBendEvents* pIBendEvents = reinterpret_cast<IBendEvents*>(sp.p);
			if (pIBendEvents != NULL)
				ret = pIBendEvents->OnBendChanged();
		}	return ret;
	
	}
};

template <class T>
class CProxyIBarRecordEvents : public IConnectionPointImpl<T, &IID_IBarRecordEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnBarRecordChanged(IBarRecord * pBarRecord)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarRecordEvents* pIBarRecordEvents = reinterpret_cast<IBarRecordEvents*>(sp.p);
			if (pIBarRecordEvents != NULL)
				ret = pIBarRecordEvents->OnBarRecordChanged(pBarRecord);
		}	return ret;
	
	}
};

template <class T>
class CProxyIBarRecordCollectionEvents : public IConnectionPointImpl<T, &IID_IBarRecordCollectionEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnBarRecordAdded(IBarRecord * pNewRecord)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarRecordCollectionEvents* pIBarRecordCollectionEvents = reinterpret_cast<IBarRecordCollectionEvents*>(sp.p);
			if (pIBarRecordCollectionEvents != NULL)
				ret = pIBarRecordCollectionEvents->OnBarRecordAdded(pNewRecord);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordRemoved(BSTR Mark)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarRecordCollectionEvents* pIBarRecordCollectionEvents = reinterpret_cast<IBarRecordCollectionEvents*>(sp.p);
			if (pIBarRecordCollectionEvents != NULL)
				ret = pIBarRecordCollectionEvents->OnBarRecordRemoved(Mark);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordChanged(IBarRecord * pBarRecord)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarRecordCollectionEvents* pIBarRecordCollectionEvents = reinterpret_cast<IBarRecordCollectionEvents*>(sp.p);
			if (pIBarRecordCollectionEvents != NULL)
				ret = pIBarRecordCollectionEvents->OnBarRecordChanged(pBarRecord);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordsSorted()
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarRecordCollectionEvents* pIBarRecordCollectionEvents = reinterpret_cast<IBarRecordCollectionEvents*>(sp.p);
			if (pIBarRecordCollectionEvents != NULL)
				ret = pIBarRecordCollectionEvents->OnBarRecordsSorted();
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordMoved(IBarRecord * pBarRecord,long idxFrom,long idxTo)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarRecordCollectionEvents* pIBarRecordCollectionEvents = reinterpret_cast<IBarRecordCollectionEvents*>(sp.p);
			if (pIBarRecordCollectionEvents != NULL)
				ret = pIBarRecordCollectionEvents->OnBarRecordMoved(pBarRecord,idxFrom,idxTo);
		}	return ret;
	
	}
};

template <class T>
class CProxyIGroupEvents : public IConnectionPointImpl<T, &IID_IGroupEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnGroupChanged(IGroup * pGroup)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupEvents* pIGroupEvents = reinterpret_cast<IGroupEvents*>(sp.p);
			if (pIGroupEvents != NULL)
				ret = pIGroupEvents->OnGroupChanged(pGroup);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordAdded(IGroup * pGroup, IBarRecord * pNewRecord)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupEvents* pIGroupEvents = reinterpret_cast<IGroupEvents*>(sp.p);
			if (pIGroupEvents != NULL)
				ret = pIGroupEvents->OnBarRecordAdded(pGroup, pNewRecord);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordChanged(IGroup * pGroup, IBarRecord * pBarRecord)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupEvents* pIGroupEvents = reinterpret_cast<IGroupEvents*>(sp.p);
			if (pIGroupEvents != NULL)
				ret = pIGroupEvents->OnBarRecordChanged(pGroup, pBarRecord);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordRemoved(IGroup * pGroup, BSTR bstrMark)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupEvents* pIGroupEvents = reinterpret_cast<IGroupEvents*>(sp.p);
			if (pIGroupEvents != NULL)
				ret = pIGroupEvents->OnBarRecordRemoved(pGroup, bstrMark);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordsSorted(IGroup * pGroup)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupEvents* pIGroupEvents = reinterpret_cast<IGroupEvents*>(sp.p);
			if (pIGroupEvents != NULL)
				ret = pIGroupEvents->OnBarRecordsSorted(pGroup);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordMoved(IGroup * pGroup, IBarRecord * pBarRecord,long idxFrom,long idxTo)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupEvents* pIGroupEvents = reinterpret_cast<IGroupEvents*>(sp.p);
			if (pIGroupEvents != NULL)
				ret = pIGroupEvents->OnBarRecordMoved(pGroup, pBarRecord,idxFrom,idxTo);
		}	return ret;
	
	}
};




template <class T>
class CProxyIGroupCollectionEvents : public IConnectionPointImpl<T, &IID_IGroupCollectionEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnGroupAdded(IGroup * pGroup)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupCollectionEvents* pIGroupCollectionEvents = reinterpret_cast<IGroupCollectionEvents*>(sp.p);
			if (pIGroupCollectionEvents != NULL)
				ret = pIGroupCollectionEvents->OnGroupAdded(pGroup);
		}	return ret;
	
	}
	HRESULT Fire_OnGroupRemoved(BSTR Name)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupCollectionEvents* pIGroupCollectionEvents = reinterpret_cast<IGroupCollectionEvents*>(sp.p);
			if (pIGroupCollectionEvents != NULL)
				ret = pIGroupCollectionEvents->OnGroupRemoved(Name);
		}	return ret;
	
	}
	HRESULT Fire_OnGroupChanged(IGroup * pGroup)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupCollectionEvents* pIGroupCollectionEvents = reinterpret_cast<IGroupCollectionEvents*>(sp.p);
			if (pIGroupCollectionEvents != NULL)
				ret = pIGroupCollectionEvents->OnGroupChanged(pGroup);
		}	return ret;
	
	}
	HRESULT Fire_OnGroupMoved(IGroup * pGroup,long idxFrom,long idxTo)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupCollectionEvents* pIGroupCollectionEvents = reinterpret_cast<IGroupCollectionEvents*>(sp.p);
			if (pIGroupCollectionEvents != NULL)
				ret = pIGroupCollectionEvents->OnGroupMoved(pGroup,idxFrom,idxTo);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordAdded(IGroup * pGroup, IBarRecord * pNewRecord)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupCollectionEvents* pIGroupCollectionEvents = reinterpret_cast<IGroupCollectionEvents*>(sp.p);
			if (pIGroupCollectionEvents != NULL)
				ret = pIGroupCollectionEvents->OnBarRecordAdded(pGroup, pNewRecord);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordChanged(IGroup * pGroup, IBarRecord * pBarRecord)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupCollectionEvents* pIGroupCollectionEvents = reinterpret_cast<IGroupCollectionEvents*>(sp.p);
			if (pIGroupCollectionEvents != NULL)
				ret = pIGroupCollectionEvents->OnBarRecordChanged(pGroup, pBarRecord);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordRemoved(IGroup * pGroup, BSTR bstrMark)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupCollectionEvents* pIGroupCollectionEvents = reinterpret_cast<IGroupCollectionEvents*>(sp.p);
			if (pIGroupCollectionEvents != NULL)
				ret = pIGroupCollectionEvents->OnBarRecordRemoved(pGroup, bstrMark);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordsSorted(IGroup * pGroup)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupCollectionEvents* pIGroupCollectionEvents = reinterpret_cast<IGroupCollectionEvents*>(sp.p);
			if (pIGroupCollectionEvents != NULL)
				ret = pIGroupCollectionEvents->OnBarRecordsSorted(pGroup);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordMoved(IGroup * pGroup, IBarRecord * pBarRecord, long idxFrom, long idxTo)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IGroupCollectionEvents* pIGroupCollectionEvents = reinterpret_cast<IGroupCollectionEvents*>(sp.p);
			if (pIGroupCollectionEvents != NULL)
				ret = pIGroupCollectionEvents->OnBarRecordMoved(pGroup, pBarRecord, idxFrom, idxTo );
		}	return ret;
	
	}
};

template <class T>
class CProxyIBarlistEvents : public IConnectionPointImpl<T, &IID_IBarlistEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnNotIncludedQuantitiesChanged()
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnNotIncludedQuantitiesChanged();
		}	return ret;
	
	}
	HRESULT Fire_OnGroupAdded(IGroup * pGroup)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnGroupAdded(pGroup);
		}	return ret;
	
	}
	HRESULT Fire_OnGroupRemoved(BSTR Name)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnGroupRemoved(Name);
		}	return ret;
	
	}
	HRESULT Fire_OnGroupChanged(IGroup * pGroup)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnGroupChanged(pGroup);
		}	return ret;
	
	}
	HRESULT Fire_OnGroupMoved(IGroup * pGroup,long idxFrom,long idxTo)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnGroupMoved(pGroup,idxFrom,idxTo);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordAdded(IGroup * pGroup, IBarRecord * pNewRecord)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnBarRecordAdded(pGroup, pNewRecord);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordChanged(IGroup * pGroup, IBarRecord * pBarRecord)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnBarRecordChanged(pGroup, pBarRecord);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordRemoved(IGroup * pGroup, BSTR bstrMark)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnBarRecordRemoved(pGroup, bstrMark);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordsSorted(IGroup * pGroup)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnBarRecordsSorted(pGroup);
		}	return ret;
	
	}
	HRESULT Fire_OnBarRecordMoved(IGroup * pGroup, IBarRecord * pBarRecord,long idxFrom,long idxTo)
	{
		HRESULT ret = S_OK;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IBarlistEvents* pIBarlistEvents = reinterpret_cast<IBarlistEvents*>(sp.p);
			if (pIBarlistEvents != NULL)
				ret = pIBarlistEvents->OnBarRecordMoved(pGroup, pBarRecord, idxFrom, idxTo);
		}	return ret;
	
	}
};
#endif