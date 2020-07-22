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


// StatusMgr.cpp: implementation of the CStatusMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bars.h"
#include "StatusMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatusMgr::CStatusMgr()
{
   ResetStatusMsgs();
}

CStatusMgr::~CStatusMgr()
{
}

void CStatusMgr::SetStatusLevel( StatusType status )
{
   if ( m_Status < status )
      m_Status = status;
}

StatusType CStatusMgr::GetStatusLevel()
{
   return m_Status;
}

void CStatusMgr::AddStatusMsg( BSTR bstrMsg, VARIANT v1, VARIANT v2 )
{
   CComObject<CStatusMessage>* pMessage;
   CComObject<CStatusMessage>::CreateInstance( &pMessage );
   pMessage->m_Text = CComBSTR(bstrMsg);
   pMessage->m_Val1 = CComVariant( v1 );
   pMessage->m_Val2 = CComVariant( v2 );

   m_pStatusMsgs->Add( pMessage );
}

IStatusMessageCollection* CStatusMgr::GetStatusMessages()
{
   IStatusMessageCollection* pColl = m_pStatusMsgs;
   pColl->AddRef();
   return pColl;
}

void CStatusMgr::ResetStatusMsgs()
{
   m_Status = stOK;

   if ( m_pStatusMsgs )
      m_pStatusMsgs.Release();

   CComObject<CStatusMessageCollection>* pColl;
   CComObject<CStatusMessageCollection>::CreateInstance( &pColl );
   
   m_pStatusMsgs = pColl;
}
