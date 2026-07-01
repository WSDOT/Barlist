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


// BarCollection.cpp : Implementation of CBarCollection

#include "BarCollection.h"
#include <tchar.h>

#pragma warning(disable : 4251)
#include <Units\Units.h>

struct CBarCollection::BarInfo
{
    BarInfo(const TCHAR* name, Float64 db, Float64 mass, Float64 length, Float64 max_length) :
        Name(name), db(db), mass(mass), length(length), max_length(max_length) {}

    std::_tstring Name;
    Float64 db;
    Float64 mass;
    Float64 length;
    Float64 max_length;
};

struct CBarCollection::BendInfo
{
    BendInfo(const TCHAR* name, UseType use, Float64 ID, BendMeasureType bm = BendMeasureType::BarDiameter) :
        Name(name), use(use), ID(ID), bendMeasure(bm) {}

    std::_tstring Name;
    UseType use;
    Float64 ID;
    BendMeasureType bendMeasure;
};

struct CBarCollection::HookInfo
{
    HookInfo(const TCHAR* name, UseType use, HookType hook, Float64 tail, Float64 min_tail, BendMeasureType bm = BendMeasureType::BarDiameter) :
        Name(name), use(use), hook(hook), tail(tail), min_tail(min_tail), bendMeasure(bm) {}

    std::_tstring Name;
    UseType use;
    HookType hook;
    Float64 tail;
    BendMeasureType bendMeasure;
    Float64 min_tail;
};

// NOTE: These tables previously called WBFL::Units::ConvertToSysUnits from namespace-scope
// (eagerly-initialized) statics. That runs during Bars.dll's DllMain, before the process is
// fully initialized, and crashes with STATUS_STACK_BUFFER_OVERRUN in Release builds when
// Bars.dll is loaded via direct linking rather than lazily via COM activation. Wrapping each
// table in a function-local static defers initialization to first actual use (a C++11 "magic
// static", thread-safe) instead of DLL-load time. Mirrors the Formatter::Init() pattern used
// elsewhere in this codebase for the same class of problem.
static Float64 Feet40()
{
    static Float64 value = WBFL::Units::ConvertToSysUnits(40.0, WBFL::Units::Measure::Feet);
    return value;
}

static Float64 Feet60()
{
    static Float64 value = WBFL::Units::ConvertToSysUnits(60.0, WBFL::Units::Measure::Feet);
    return value;
}

////////////////////////////////////////////////////////
// Steel
////////////////////////////////////////////////////////

static const CBarCollection::BarInfo* GetSteelBarInfo(int& count)
{
    static const Float64 _40_ft = Feet40();
    static const Float64 _60_ft = Feet60();
    static const CBarCollection::BarInfo data[] =
{
   CBarCollection::BarInfo( _T("#3"), WBFL::Units::ConvertToSysUnits(0.375,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(0.376, WBFL::Units::Measure::LbfPerFoot), _40_ft, _40_ft),
   CBarCollection::BarInfo( _T("#4"), WBFL::Units::ConvertToSysUnits(0.500,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(0.668, WBFL::Units::Measure::LbfPerFoot), _40_ft, _40_ft),
   CBarCollection::BarInfo( _T("#5"), WBFL::Units::ConvertToSysUnits(0.625,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(1.043, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   CBarCollection::BarInfo( _T("#6"), WBFL::Units::ConvertToSysUnits(0.750,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(1.502, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   CBarCollection::BarInfo( _T("#7"), WBFL::Units::ConvertToSysUnits(0.875,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(2.044, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   CBarCollection::BarInfo( _T("#8"), WBFL::Units::ConvertToSysUnits(1.000,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(2.670, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   CBarCollection::BarInfo( _T("#9"), WBFL::Units::ConvertToSysUnits(1.128,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(3.400, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   CBarCollection::BarInfo(_T("#10"), WBFL::Units::ConvertToSysUnits(1.270,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(4.303, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   CBarCollection::BarInfo(_T("#11"), WBFL::Units::ConvertToSysUnits(1.410,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(5.313, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   CBarCollection::BarInfo(_T("#14"), WBFL::Units::ConvertToSysUnits(1.693,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(7.650, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft),
   CBarCollection::BarInfo(_T("#18"), WBFL::Units::ConvertToSysUnits(2.257,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(13.60, WBFL::Units::Measure::LbfPerFoot), _60_ft, _60_ft)
};
    count = sizeof(data) / sizeof(data[0]);
    return data;
}

static const CBarCollection::BarInfo* GetGalvanizedBarInfo(int& count)
{
    static const Float64 _40_ft = Feet40();
    static const Float64 _60_ft = Feet60();
    static const CBarCollection::BarInfo data[] =
{
   CBarCollection::BarInfo( _T("#3"), WBFL::Units::ConvertToSysUnits(0.375,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(0.376, WBFL::Units::Measure::LbfPerFoot), _40_ft, _40_ft),
   CBarCollection::BarInfo( _T("#4"), WBFL::Units::ConvertToSysUnits(0.500,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(0.668, WBFL::Units::Measure::LbfPerFoot), _40_ft, _40_ft),
   CBarCollection::BarInfo( _T("#5"), WBFL::Units::ConvertToSysUnits(0.625,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(1.043, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   CBarCollection::BarInfo( _T("#6"), WBFL::Units::ConvertToSysUnits(0.750,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(1.502, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   CBarCollection::BarInfo( _T("#7"), WBFL::Units::ConvertToSysUnits(0.875,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(2.044, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   CBarCollection::BarInfo( _T("#8"), WBFL::Units::ConvertToSysUnits(1.000,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(2.670, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   CBarCollection::BarInfo( _T("#9"), WBFL::Units::ConvertToSysUnits(1.128,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(3.400, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#10"), WBFL::Units::ConvertToSysUnits(1.270,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(4.303, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#11"), WBFL::Units::ConvertToSysUnits(1.410,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(5.313, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#14"), WBFL::Units::ConvertToSysUnits(1.693,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(7.650, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#18"), WBFL::Units::ConvertToSysUnits(2.257,WBFL::Units::Measure::Inch),  WBFL::Units::ConvertToSysUnits(13.60, WBFL::Units::Measure::LbfPerFoot), _40_ft, _60_ft)
};
    count = sizeof(data) / sizeof(data[0]);
    return data;
}

static const CBarCollection::BendInfo* GetSteelBendInfo(int& count)
{
    static const CBarCollection::BendInfo data[] =
{
   CBarCollection::BendInfo(_T("#3"), UseType::utLongitudinal,  6.0),
   CBarCollection::BendInfo(_T("#3"), UseType::utTransverse,    4.0),
   CBarCollection::BendInfo(_T("#3"), UseType::utSeismic,       4.0),

   CBarCollection::BendInfo(_T("#4"), UseType::utLongitudinal,  6.0),
   CBarCollection::BendInfo(_T("#4"), UseType::utTransverse,    4.0),
   CBarCollection::BendInfo(_T("#4"), UseType::utSeismic,       4.0),

   CBarCollection::BendInfo(_T("#5"), UseType::utLongitudinal,  6.0),
   CBarCollection::BendInfo(_T("#5"), UseType::utTransverse,    4.0),
   CBarCollection::BendInfo(_T("#5"), UseType::utSeismic,       4.0),

   CBarCollection::BendInfo(_T("#6"), UseType::utLongitudinal,  6.0),
   CBarCollection::BendInfo(_T("#6"), UseType::utTransverse,    6.0),
   CBarCollection::BendInfo(_T("#6"), UseType::utSeismic,       6.0),

   CBarCollection::BendInfo(_T("#7"), UseType::utLongitudinal,  6.0),
   CBarCollection::BendInfo(_T("#7"), UseType::utTransverse,    6.0),
   CBarCollection::BendInfo(_T("#7"), UseType::utSeismic,       6.0),

   CBarCollection::BendInfo(_T("#8"), UseType::utLongitudinal,  6.0),
   CBarCollection::BendInfo(_T("#8"), UseType::utTransverse,    6.0),
   CBarCollection::BendInfo(_T("#8"), UseType::utSeismic,       6.0),

   CBarCollection::BendInfo( _T("#9"), UseType::utLongitudinal,  8.0),
   CBarCollection::BendInfo(_T("#10"), UseType::utLongitudinal,  8.0),
   CBarCollection::BendInfo(_T("#11"), UseType::utLongitudinal,  8.0),
   CBarCollection::BendInfo(_T("#14"), UseType::utLongitudinal, 10.0),
   CBarCollection::BendInfo(_T("#18"), UseType::utLongitudinal, 10.0)
};
    count = sizeof(data) / sizeof(data[0]);
    return data;
}

static const CBarCollection::HookInfo* GetSteelHookInfo(int& count)
{
    static const CBarCollection::HookInfo data[] =
{
   CBarCollection::HookInfo(_T("#3"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#3"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   CBarCollection::HookInfo(_T("#3"), UseType::utTransverse,   HookType::ht90,   6.0,    0.0),
   CBarCollection::HookInfo(_T("#3"), UseType::utTransverse,   HookType::ht135,  6.0,    0.0),
   CBarCollection::HookInfo(_T("#3"), UseType::utSeismic,      HookType::ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#4"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#4"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   CBarCollection::HookInfo(_T("#4"), UseType::utTransverse,   HookType::ht90,   6.0,    0.0),
   CBarCollection::HookInfo(_T("#4"), UseType::utTransverse,   HookType::ht135,  6.0,    0.0),
   CBarCollection::HookInfo(_T("#4"), UseType::utSeismic,      HookType::ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#5"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#5"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   CBarCollection::HookInfo(_T("#5"), UseType::utTransverse,   HookType::ht90,   6.0,    0.0),
   CBarCollection::HookInfo(_T("#5"), UseType::utTransverse,   HookType::ht135,  6.0,    0.0),
   CBarCollection::HookInfo(_T("#5"), UseType::utSeismic,      HookType::ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#6"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#6"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   CBarCollection::HookInfo(_T("#6"), UseType::utTransverse,   HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#6"), UseType::utTransverse,   HookType::ht135,  6.0,    0.0),
   CBarCollection::HookInfo(_T("#6"), UseType::utSeismic,      HookType::ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#7"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#7"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   CBarCollection::HookInfo(_T("#7"), UseType::utTransverse,   HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#7"), UseType::utTransverse,   HookType::ht135,  6.0,    0.0),
   CBarCollection::HookInfo(_T("#7"), UseType::utSeismic,      HookType::ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#8"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#8"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),
   CBarCollection::HookInfo(_T("#8"), UseType::utTransverse,   HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#8"), UseType::utTransverse,   HookType::ht135,  6.0,    0.0),
   CBarCollection::HookInfo(_T("#8"), UseType::utSeismic,      HookType::ht135,  6.0,   WBFL::Units::ConvertToSysUnits(3.0,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#9"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#9"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#10"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#10"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#11"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#11"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#14"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#14"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch)),

   CBarCollection::HookInfo(_T("#18"), UseType::utLongitudinal, HookType::ht90,  12.0,    0.0),
   CBarCollection::HookInfo(_T("#18"), UseType::utLongitudinal, HookType::ht180,  4.0,   WBFL::Units::ConvertToSysUnits(2.5,WBFL::Units::Measure::Inch))
};
    count = sizeof(data) / sizeof(data[0]);
    return data;
}

////////////////////////////////////////////////////////
// GFRP
////////////////////////////////////////////////////////
static const CBarCollection::BarInfo* GetGfrpBarInfo(int& count)
{
    static const Float64 _40_ft = Feet40();
    static const Float64 _60_ft = Feet60();
    static const CBarCollection::BarInfo data[] =
{
   CBarCollection::BarInfo(_T("#3"),  WBFL::Units::ConvertToSysUnits(0.375,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft), // weight = 0 because quantities are by length
   CBarCollection::BarInfo(_T("#4"),  WBFL::Units::ConvertToSysUnits(0.500,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#5"),  WBFL::Units::ConvertToSysUnits(0.625,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#6"),  WBFL::Units::ConvertToSysUnits(0.750,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#7"),  WBFL::Units::ConvertToSysUnits(0.875,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#8"),  WBFL::Units::ConvertToSysUnits(1.000,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#9"),  WBFL::Units::ConvertToSysUnits(1.128,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
   CBarCollection::BarInfo(_T("#10"), WBFL::Units::ConvertToSysUnits(1.270,WBFL::Units::Measure::Inch), 0.0, _40_ft, _60_ft),
};
    count = sizeof(data) / sizeof(data[0]);
    return data;
}

static const CBarCollection::BendInfo* GetGfrpBendInfo(int& count)
{
    static const CBarCollection::BendInfo data[] =
{
   CBarCollection::BendInfo(_T("#3"), UseType::utLongitudinal, 6.0),
   CBarCollection::BendInfo(_T("#4"), UseType::utLongitudinal, 6.0),
   CBarCollection::BendInfo(_T("#5"), UseType::utLongitudinal, 6.0),
   CBarCollection::BendInfo(_T("#6"), UseType::utLongitudinal, 6.0),
   CBarCollection::BendInfo(_T("#7"), UseType::utLongitudinal, 6.0),
   CBarCollection::BendInfo(_T("#8"), UseType::utLongitudinal, 6.0),
};
    count = sizeof(data) / sizeof(data[0]);
    return data;
}

// GFRP Hook tail extensions are 12db, measured from the outside of the bar.
// The tail length in this data structure is the bar extension measured from
// the center of the bend diameter. We need to compute a tail extension
// that results in a bend tail that is 12db from the outside of the bar
// 12db - (6db)/2 - 1db = 8db
// All bends for GFRP are 6db. The distance from the start of the tail extension
// to the inside of bar bend is half this distance. The distance to the outside
// of the bend is 1 db.
static const CBarCollection::HookInfo* GetGfrpHookInfo(int& count)
{
    static const CBarCollection::HookInfo data[] =
{
   CBarCollection::HookInfo(_T("#3"), UseType::utLongitudinal, HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#3"), UseType::utLongitudinal, HookType::ht180,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#3"), UseType::utTransverse,   HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#3"), UseType::utTransverse,   HookType::ht135,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#3"), UseType::utSeismic,      HookType::ht135,  8.0, 0.0),

   CBarCollection::HookInfo(_T("#4"), UseType::utLongitudinal, HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#4"), UseType::utLongitudinal, HookType::ht180,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#4"), UseType::utTransverse,   HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#4"), UseType::utTransverse,   HookType::ht135,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#4"), UseType::utSeismic,      HookType::ht135,  8.0, 0.0),

   CBarCollection::HookInfo(_T("#5"), UseType::utLongitudinal, HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#5"), UseType::utLongitudinal, HookType::ht180,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#5"), UseType::utTransverse,   HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#5"), UseType::utTransverse,   HookType::ht135,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#5"), UseType::utSeismic,      HookType::ht135,  8.0, 0.0),

   CBarCollection::HookInfo(_T("#6"), UseType::utLongitudinal, HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#6"), UseType::utLongitudinal, HookType::ht180,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#6"), UseType::utTransverse,   HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#6"), UseType::utTransverse,   HookType::ht135,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#6"), UseType::utSeismic,      HookType::ht135,  8.0, 0.0),

   CBarCollection::HookInfo(_T("#7"), UseType::utLongitudinal, HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#7"), UseType::utLongitudinal, HookType::ht180,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#7"), UseType::utTransverse,   HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#7"), UseType::utTransverse,   HookType::ht135,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#7"), UseType::utSeismic,      HookType::ht135,  8.0, 0.0),

   CBarCollection::HookInfo(_T("#8"), UseType::utLongitudinal, HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#8"), UseType::utLongitudinal, HookType::ht180,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#8"), UseType::utTransverse,   HookType::ht90,   8.0, 0.0),
   CBarCollection::HookInfo(_T("#8"), UseType::utTransverse,   HookType::ht135,  8.0, 0.0),
   CBarCollection::HookInfo(_T("#8"), UseType::utSeismic,      HookType::ht135,  8.0, 0.0),
};
    count = sizeof(data) / sizeof(data[0]);
    return data;
}

void CBarCollection::Init(const BarInfo barInfo[], int cBars, const BendInfo bendInfo[], int cBends, const HookInfo hookInfo[], int cHooks)
{
    // for each bar size...
    for (int bar = 0; bar < cBars; bar++)
    {
        CBarData barData;
        barData.SetBarData(barInfo[bar].Name, barInfo[bar].db, barInfo[bar].mass, barInfo[bar].length, barInfo[bar].max_length);

        // add bend info for the current bar size
        for (int bend = 0; bend < cBends; bend++)
        {
            if (barInfo[bar].Name.compare(bendInfo[bend].Name) == 0)
            {
                barData.AddBendData(bendInfo[bend].use, bendInfo[bend].ID, bendInfo[bend].bendMeasure);
            }
        }

        // add hook info for the current bar size
        for (int hook = 0; hook < cHooks; hook++)
        {
            if (barInfo[bar].Name.compare(hookInfo[hook].Name) == 0)
            {
                barData.AddHookData(hookInfo[hook].use, hookInfo[hook].hook, hookInfo[hook].tail, hookInfo[hook].bendMeasure, hookInfo[hook].min_tail);
            }
        }

        m_Bars.push_back(std::move(barData));
    }
}

void CBarCollection::InitSteelBars()
{
    int cBars, cBends, cHooks;
    const BarInfo* barInfo = GetSteelBarInfo(cBars);
    const BendInfo* bendInfo = GetSteelBendInfo(cBends);
    const HookInfo* hookInfo = GetSteelHookInfo(cHooks);
    Init(barInfo, cBars, bendInfo, cBends, hookInfo, cHooks);
}

void CBarCollection::InitGalvanizedBars()
{
    int cBars, cBends, cHooks;
    const BarInfo* barInfo = GetGalvanizedBarInfo(cBars);
    const BendInfo* bendInfo = GetSteelBendInfo(cBends);
    const HookInfo* hookInfo = GetSteelHookInfo(cHooks);
    Init(barInfo, cBars, bendInfo, cBends, hookInfo, cHooks);
}

void CBarCollection::InitGFRPBars()
{
    int cBars, cBends, cHooks;
    const BarInfo* barInfo = GetGfrpBarInfo(cBars);
    const BendInfo* bendInfo = GetGfrpBendInfo(cBends);
    const HookInfo* hookInfo = GetGfrpHookInfo(cHooks);
    Init(barInfo, cBars, bendInfo, cBends, hookInfo, cHooks);
}

std::size_t CBarCollection::Count() const
{
    return m_Bars.size();
}

const CBarData* CBarCollection::Item(std::size_t index) const
{
    if (m_Bars.size() <= index)
    {
        return nullptr;
    }
    return &m_Bars[index];
}

const CBarData* CBarCollection::Find(const std::_tstring& name) const
{
    for (const auto& bar : m_Bars)
    {
        if (bar.GetName() == name)
        {
            return &bar;
        }
    }
    return nullptr;
}

CBarCollection::const_iterator CBarCollection::begin() const
{
    return m_Bars.begin();
}

CBarCollection::const_iterator CBarCollection::end() const
{
    return m_Bars.end();
}
