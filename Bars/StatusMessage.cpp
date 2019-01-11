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


// StatusMessage.cpp : Implementation of CStatusMessage
#include "stdafx.h"
#include "Bars.h"
#include "StatusMessage.h"

/////////////////////////////////////////////////////////////////////////////
// CStatusMessage


STDMETHODIMP CStatusMessage::get_Text(BSTR *pVal)
{
	// TODO: Add your implementation code here
   *pVal = m_Text.Copy();
	return S_OK;
}

STDMETHODIMP CStatusMessage::get_Val1(VARIANT *pVal)
{
	// TODO: Add your implementation code here
   CComVariant var;
   var.Attach( pVal );
   var = m_Val1;
   var.Detach( pVal );
	return S_OK;
}

STDMETHODIMP CStatusMessage::get_Val2(VARIANT *pVal)
{
	// TODO: Add your implementation code here
   CComVariant var;
   var.Attach( pVal );
   var = m_Val2;
   var.Detach( pVal );
	return S_OK;
}
