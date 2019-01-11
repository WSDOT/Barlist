///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 2009-2019, Washington State Department of Transportation
//                     Bridge and Structures Office
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


// BarInfoMgr.cpp : Implementation of CBarInfoMgr
#include "stdafx.h"
#include "Bars.h"
#include "BarInfoMgr.h"
#include "BarCollection.h"

/////////////////////////////////////////////////////////////////////////////
// CBarInfoMgr
HRESULT CBarInfoMgr::FinalConstruct()
{
   // Instantiate the collection of bars
   CComObject<CBarCollection>* pBars;
   HRESULT hr = CComObject<CBarCollection>::CreateInstance(&pBars);
   if ( FAILED(hr) )
      return hr;

   hr = pBars->QueryInterface(&m_pBars);
   return hr;
}

void CBarInfoMgr::FinalRelease()
{
   m_pBars = 0;
}

STDMETHODIMP CBarInfoMgr::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IBarInfoMgr
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CBarInfoMgr::get_Specification(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = CComBSTR("AASHTO LRFD Bridge Design Specification, 2nd Edition 1998").Copy();

	return S_OK;
}

STDMETHODIMP CBarInfoMgr::get_Bars(IBarCollection **pVal)
{
	// TODO: Add your implementation code here
   return m_pBars->QueryInterface(IID_IBarCollection,(void**)pVal);
}
