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

#pragma once

#include <functional>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstddef>

// Native replacement for the old ATL connection-point proxies.
// Subscribers are snapshot-copied before firing so a slot disconnecting
// itself (or another slot) mid-fire is safe, matching the Lock()/Unlock()
// snapshot-under-lock behavior the old CProxyIXxxEvents<T> templates relied on.
template <class... Args>
class Signal
{
public:
    using Slot = std::function<void(Args...)>;
    using Token = std::size_t;

    Token Connect(Slot slot)
    {
        Token token = m_next++;
        m_slots.emplace_back(token, std::move(slot));
        return token;
    }

    void Disconnect(Token token)
    {
        std::erase_if(m_slots, [token](const auto& entry) { return entry.first == token; });
    }

    void operator()(Args... args) const
    {
        auto snapshot = m_slots;
        for (auto& entry : snapshot)
        {
            entry.second(args...);
        }
    }

private:
    std::vector<std::pair<Token, Slot>> m_slots;
    Token m_next = 0;
};
