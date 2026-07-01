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


// StatusMessage.h : Declaration of CStatusMessage (native, replaces the
// IStatusMessage ATL/COM coclass)

#pragma once

#include "BarsExport.h"
#include <string>
#include <variant>
#include <WBFLTypes.h>

// Replaces the VARIANT Val1/Val2 properties -- these were only ever
// populated with an unset value, a long, a double, or a string at any
// call site in the codebase.
using StatusValue = std::variant<std::monostate, long, double, std::_tstring>;

class BARS_API CStatusMessage
{
public:
    CStatusMessage() = default;
    CStatusMessage(std::_tstring text, StatusValue val1 = {}, StatusValue val2 = {});

    const std::_tstring& GetText() const;
    const StatusValue& GetVal1() const;
    const StatusValue& GetVal2() const;

private:
    std::_tstring m_Text;
    StatusValue m_Val1;
    StatusValue m_Val2;
};
