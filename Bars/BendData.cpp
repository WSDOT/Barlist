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


// BendData.cpp : Implementation of CBendData
#include "stdafx.h"
#include "Bars.h"
#include "BendData.h"

/////////////////////////////////////////////////////////////////////////////
// CBendData


STDMETHODIMP CBendData::get_Use(UseType *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Use;
	return S_OK;
}

STDMETHODIMP CBendData::get_InsideDiameter(Float64 *pVal)
{
   *pVal = m_ID;
	return S_OK;
}

STDMETHODIMP CBendData::get_BendMeasure(BendMeasureType* pVal)
{
   *pVal = m_BendMeasure;
   return S_OK;
}
