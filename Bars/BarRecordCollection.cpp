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


// BarRecordCollection.cpp : Implementation of CBarRecordCollection

#include "BarRecordCollection.h"
#include "BarRecord.h"
#include "Barlist.h"
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <tchar.h>

CBarRecordCollection::~CBarRecordCollection()
{
    // Defensive: disconnect from any record that outlives this collection
    // (e.g. still held externally by an undo buffer or clipboard) so it
    // doesn't retain a lambda capturing this destroyed collection.
    for (auto& [record, token] : m_ChangeTokens)
    {
        record->OnBarRecordChanged.Disconnect(token);
    }
}

void CBarRecordCollection::SetBarlist(CBarlist* pBarlist)
{
    m_pBarlist = pBarlist;
}

CBarlist* CBarRecordCollection::GetBarlist() const
{
    return m_pBarlist;
}

std::size_t CBarRecordCollection::Count() const
{
    return m_Records.size();
}

std::shared_ptr<CBarRecord> CBarRecordCollection::Item(std::size_t index) const
{
    if (m_Records.size() <= index)
    {
        return nullptr;
    }
    return m_Records[index];
}

std::shared_ptr<CBarRecord> CBarRecordCollection::Find(const std::_tstring& mark) const
{
    for (const auto& record : m_Records)
    {
        if (record->GetMark() == mark)
        {
            return record;
        }
    }
    return nullptr;
}

long CBarRecordCollection::IndexOf(const CBarRecord* record) const
{
    for (std::size_t i = 0; i < m_Records.size(); ++i)
    {
        if (m_Records[i].get() == record)
        {
            return static_cast<long>(i);
        }
    }
    return -1;
}

void CBarRecordCollection::Add(std::shared_ptr<CBarRecord> record)
{
    Insert(-1, std::move(record)); // add means insert at end
}

void CBarRecordCollection::Insert(long index, std::shared_ptr<CBarRecord> record)
{
    record->SetBarlist(m_pBarlist);

    CBarRecord& recordRef = *record;
    CBarRecord* recordPtr = record.get();
    m_ChangeTokens[recordPtr] = record->OnBarRecordChanged.Connect([this](CBarRecord& r) { OnBarRecordChanged(r); });

    if (index < 0 || static_cast<std::size_t>(index) >= m_Records.size())
    {
        m_Records.push_back(std::move(record));
    }
    else
    {
        m_Records.insert(m_Records.begin() + index, std::move(record));
    }

    OnBarRecordAdded(recordRef);
}

void CBarRecordCollection::Replace(long index, std::shared_ptr<CBarRecord> record)
{
    if (index < 0 || static_cast<std::size_t>(index) >= m_Records.size())
    {
        throw std::out_of_range("CBarRecordCollection::Replace: index out of range");
    }

    std::_tstring oldMark = m_Records[index]->GetMark();
    std::_tstring newMark = record->GetMark();

    record->SetBarlist(m_pBarlist);
    if (m_pBarlist && oldMark != newMark)
    {
        m_pBarlist->ReplaceMarkNumber(oldMark, newMark);
    }

    CBarRecord* oldPtr = m_Records[index].get();
    auto found = m_ChangeTokens.find(oldPtr);
    if (found != m_ChangeTokens.end())
    {
        m_Records[index]->OnBarRecordChanged.Disconnect(found->second);
        m_ChangeTokens.erase(found);
    }

    m_ChangeTokens[record.get()] = record->OnBarRecordChanged.Connect([this](CBarRecord& r) { OnBarRecordChanged(r); });

    m_Records[index] = record;
    OnBarRecordChanged(*record);
}

void CBarRecordCollection::Remove(long index)
{
    if (index < 0 || static_cast<std::size_t>(index) >= m_Records.size())
    {
        throw std::out_of_range("CBarRecordCollection::Remove: index out of range");
    }

    CBarRecord* recordPtr = m_Records[index].get();
    auto found = m_ChangeTokens.find(recordPtr);
    if (found != m_ChangeTokens.end())
    {
        recordPtr->OnBarRecordChanged.Disconnect(found->second);
        m_ChangeTokens.erase(found);
    }

    std::_tstring mark = m_Records[index]->GetMark();
    m_Records.erase(m_Records.begin() + index);

    OnBarRecordRemoved(mark);
}

void CBarRecordCollection::Move(long recIndex, MoveType mt, long targetIndex)
{
    std::shared_ptr<CBarRecord> record = m_Records.at(recIndex);

    m_Records.erase(m_Records.begin() + recIndex);

    // NOTE: this must come after the erase above -- erase shifts every
    // subsequent index down by one.
    if (mt == MoveType::mtAfter && targetIndex < recIndex)
    {
        targetIndex++;
    }

    m_Records.insert(m_Records.begin() + targetIndex, record);

    OnBarRecordMoved(*record, recIndex, targetIndex);
}

void CBarRecordCollection::MoveUp(long index)
{
    if (index == 0)
    {
        return; // already at the top
    }

    Move(index, MoveType::mtBefore, index - 1);
}

void CBarRecordCollection::MoveDown(long index)
{
    if (static_cast<std::size_t>(index) == m_Records.size() - 1)
    {
        return; // already at the bottom
    }

    Move(index, MoveType::mtAfter, index + 1);
}

void CBarRecordCollection::Sort()
{
    std::sort(m_Records.begin(), m_Records.end(),
        [](const std::shared_ptr<CBarRecord>& a, const std::shared_ptr<CBarRecord>& b)
        {
            const std::_tstring& mark1 = a->GetMark();
            const std::_tstring& mark2 = b->GetMark();

            TCHAR* pStop1;
            Float64 val1 = _tcstod(mark1.c_str(), &pStop1);
            TCHAR* pStop2;
            Float64 val2 = _tcstod(mark2.c_str(), &pStop2);

            if (val1 != 0 && val2 != 0)
            {
                // both are numeric
                return val1 < val2;
            }

            // one is alpha-numeric
            return mark1 < mark2;
        });

    OnBarRecordsSorted();
}

CBarRecordCollection::const_iterator CBarRecordCollection::begin() const
{
    return m_Records.begin();
}

CBarRecordCollection::const_iterator CBarRecordCollection::end() const
{
    return m_Records.end();
}
