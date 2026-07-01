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


// StatusMessage.cpp : Implementation of CStatusMessage

#include "StatusMessage.h"

CStatusMessage::CStatusMessage(std::_tstring text, StatusValue val1, StatusValue val2)
    : m_Text(std::move(text)), m_Val1(std::move(val1)), m_Val2(std::move(val2))
{
}

const std::_tstring& CStatusMessage::GetText() const
{
    return m_Text;
}

const StatusValue& CStatusMessage::GetVal1() const
{
    return m_Val1;
}

const StatusValue& CStatusMessage::GetVal2() const
{
    return m_Val2;
}
