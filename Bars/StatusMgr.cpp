///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright � 1999-2026  Washington State Department of Transportation
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

#include "StatusMgr.h"

CStatusMgr::CStatusMgr()
{
    ResetStatusMsgs();
}

void CStatusMgr::SetStatusLevel(StatusType status)
{
    if (m_Status < status)
    {
        m_Status = status;
    }
}

StatusType CStatusMgr::GetStatusLevel() const
{
    return m_Status;
}

void CStatusMgr::AddStatusMsg(std::_tstring msg, StatusValue v1, StatusValue v2)
{
    m_StatusMsgs.Add(CStatusMessage(std::move(msg), std::move(v1), std::move(v2)));
}

const CStatusMessageCollection& CStatusMgr::GetStatusMessages() const
{
    return m_StatusMsgs;
}

void CStatusMgr::ResetStatusMsgs()
{
    m_Status = StatusType::stOK;
    m_StatusMsgs.Clear();
}
