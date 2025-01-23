///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2025  Washington State Department of Transportation
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


// StatusMessageCollection.cpp : Implementation of CStatusMessageCollection
#include "stdafx.h"
#include "Bars.h"
#include "StatusMessageCollection.h"

/////////////////////////////////////////////////////////////////////////////
// CStatusMessageCollection

/*
STDMETHODIMP CStatusMessageCollection::get_Count(long *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CStatusMessageCollection::get__NewEnum(LPUNKNOWN *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
*/

STDMETHODIMP CStatusMessageCollection::get_Item(long index, IStatusMessage* *pVal)
{
	// TODO: Add your implementation code here
   ATLASSERT( 0 <= index && index < m_coll.size() );
   CComVariant& var = m_coll[index];
   IDispatch* pDisp = var.pdispVal;
   CComQIPtr<IStatusMessage> pMsg( pDisp );
   pMsg.CopyTo(pVal);
	return S_OK;
}


STDMETHODIMP CStatusMessageCollection::Add(IStatusMessage* pVal)
{
	// TODO: Add your implementation code here
   CComQIPtr<IDispatch> pDisp(pVal);
   CComVariant var(pDisp);
   m_coll.push_back( var );
	return S_OK;
}
