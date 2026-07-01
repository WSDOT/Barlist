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


// StatusMessageCollection.cpp : Implementation of CStatusMessageCollection

#include "StatusMessageCollection.h"

void CStatusMessageCollection::Add(CStatusMessage msg)
{
    m_Messages.push_back(std::move(msg));
}

void CStatusMessageCollection::Clear()
{
    m_Messages.clear();
}

std::size_t CStatusMessageCollection::Count() const
{
    return m_Messages.size();
}

const CStatusMessage* CStatusMessageCollection::Item(std::size_t index) const
{
    if (m_Messages.size() <= index)
    {
        return nullptr;
    }
    return &m_Messages[index];
}

CStatusMessageCollection::const_iterator CStatusMessageCollection::begin() const
{
    return m_Messages.begin();
}

CStatusMessageCollection::const_iterator CStatusMessageCollection::end() const
{
    return m_Messages.end();
}
