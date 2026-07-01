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


// StatusMessageCollection.h : Declaration of CStatusMessageCollection
// (native, replaces the IStatusMessageCollection ATL/COM coclass, which
// used ICollectionOnSTLImpl<...>/CComEnumOnSTL over std::vector<CComVariant>)

#pragma once

#include "BarsExport.h"
#include "StatusMessage.h"
#include <vector>
#include <cstddef>

class BARS_API CStatusMessageCollection
{
public:
    void Add(CStatusMessage msg);
    void Clear();

    std::size_t Count() const;
    const CStatusMessage* Item(std::size_t index) const; // nullptr if index out of range

    using const_iterator = std::vector<CStatusMessage>::const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

private:
    std::vector<CStatusMessage> m_Messages;
};
