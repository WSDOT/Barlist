///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2024  Washington State Department of Transportation
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


// HookData.cpp : Implementation of CHookData
#include "stdafx.h"
#include "Bars.h"
#include "HookData.h"

/////////////////////////////////////////////////////////////////////////////
// CHookData

void CHookData::SetData(HookType hook,UseType use,Float64 T,BendMeasureType bm,Float64 Tmin)
{
   m_HookType = hook;
   m_UseType  = use;
   m_Tail     = T;
   m_BendMeasure = bm;
   m_MinTail  = Tmin;
}

STDMETHODIMP CHookData::get_Type(HookType *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_HookType;
	return S_OK;
}

STDMETHODIMP CHookData::get_Use(UseType *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_UseType;
	return S_OK;
}

STDMETHODIMP CHookData::get_Tail(Float64 *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Tail;
	return S_OK;
}

STDMETHODIMP CHookData::get_BendMeasure(BendMeasureType* pVal)
{
   *pVal = m_BendMeasure;
   return S_OK;
}

STDMETHODIMP CHookData::get_MinTail(Float64 *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_MinTail;
	return S_OK;
}
