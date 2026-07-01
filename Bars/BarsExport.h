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

// Bars.dll defines BARS_EXPORTS on its own compile; every consumer (Barlist,
// BXF, ExcelExporter, BarlistUnitTests) picks up the dllimport side instead.
// All four projects must stay on the same PlatformToolset/RuntimeLibrary so
// the exported C++ classes remain ABI-compatible across the DLL boundary --
// there is no COM manifest step anymore to catch a mismatch.
#ifdef BARS_EXPORTS
#define BARS_API __declspec(dllexport)
#else
#define BARS_API __declspec(dllimport)
#endif
