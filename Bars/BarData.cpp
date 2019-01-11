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


// BarData.cpp : Implementation of CBarData
#include "stdafx.h"
#include "Bars.h"
#include "BarData.h"
#include "BendData.h"
#include "HookData.h"

/////////////////////////////////////////////////////////////////////////////
// CBarData
void CBarData::FinalRelease()
{
   for ( short i = 0; i < 3; i++ )
   {
      m_pBends[i] = 0;
      for ( short j = 0; j < 3; j++ )
      {
         m_pHooks[i][j] = 0;
      }
   }
}

void CBarData::SetBarData(const std::string& name,double db,double mass,double length,double maxLength)
{
   m_Name      = name.c_str();
   m_db        = db;
   m_Mass      = mass;
   m_Length    = length;
   m_MaxLength = maxLength;
}

void CBarData::AddBendData(UseType use,double ID)
{
	// TODO: Add your implementation code here
   CComObject<CBendData>* pBendData;
   CComObject<CBendData>::CreateInstance( &pBendData );
   pBendData->AddRef();
   pBendData->SetData(ID,use);
   m_pBends[use].Attach(pBendData);
}

void CBarData::AddHookData(UseType use,HookType hook,double T,double Tmin)
{
	// TODO: Add your implementation code here
   CComObject<CHookData>* pHookData;
   CComObject<CHookData>::CreateInstance( &pHookData );
   pHookData->AddRef();
   pHookData->SetData(hook,use,T,Tmin);
   m_pHooks[use][hook].Attach(pHookData);
}

STDMETHODIMP CBarData::get_Diameter(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_db;
	return S_OK;
}

STDMETHODIMP CBarData::get_Mass(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Mass;
	return S_OK;
}

STDMETHODIMP CBarData::get_NormalLength(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Length;
	return S_OK;
}

STDMETHODIMP CBarData::get_MaxLength(double *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_MaxLength;
	return S_OK;
}

STDMETHODIMP CBarData::get_BendData(UseType use, IBendData **pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_pBends[use];
   if ( *pVal )
      (*pVal)->AddRef();

	return S_OK;
}

STDMETHODIMP CBarData::get_HookData(UseType use, HookType hook, IHookData **pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_pHooks[use][hook];
   if ( *pVal )
      (*pVal)->AddRef();

	return S_OK;
}

STDMETHODIMP CBarData::get_Name(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Name.Copy();
	return S_OK;
}
