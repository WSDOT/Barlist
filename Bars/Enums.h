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

// Native enum class replacements for the enums declared in SDK\Include\Barlst.idl.
// Values are pinned to match the original IDL enums exactly, since they are
// persisted (bar list files) and must remain stable across the migration.

enum class StatusType
{
    stOK = 0,
    stWarning = 1,
    stError = 2
};

enum class MaterialType
{
    // Plain low alloy carbon
    A706_Grade60, A706_Grade80,

    // Chromium / MMFX
    A1035_Grade100, A1035_Grade120,

    // Galvanized
    A767_A1094_Grade60, A767_A1094_Grade80, A767_A1094_Grade100,

    // Stainless steel
    A955_Grade60, A955_Grade75, A955_Grade80,

    // GFRP
    D7957
};

inline constexpr int MATERIAL_COUNT = static_cast<int>(MaterialType::D7957) + 1;

enum class HookType { ht90, ht135, ht180 };
enum class UseType { utLongitudinal, utTransverse, utSeismic };
enum class DimensionType { dimU = 0, dimW = 1, dimX = 2, dimY = 3, dimZ = 4, dimT1 = 5, dimT2 = 6 };
enum class MoveType { mtBefore = 1, mtAfter = 2 };
enum class BendMeasureType { BarDiameter, InsideDiameter };
