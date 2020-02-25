///////////////////////////////////////////////////////////////////////
// Barlist - Barlist Extensible Application Framework
// Copyright © 1999-2019  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

#pragma once

// CATID for plug-ins that extend Barlist itself
// {37E8EB8B-571F-46E1-8596-8BAD977F2638}
DEFINE_GUID(CATID_BarlistPlugin, 
0x37e8eb8b, 0x571f, 0x46e1, 0x85, 0x96, 0x8b, 0xad, 0x97, 0x7f, 0x26, 0x38);

// CATID for plug-ins that provide Barlist applications
// {AFBC445E-98FB-43C1-AD02-6AF599903BD9}
DEFINE_GUID(CATID_BarlistAppPlugin, 
0xafbc445e, 0x98fb, 0x43c1, 0xad, 0x2, 0x6a, 0xf5, 0x99, 0x90, 0x3b, 0xd9);

// {7047875C-F3A0-453F-9F04-DDA9238AB5CF}
DEFINE_GUID(CATID_BarlistComponentInfo, 
0x7047875c, 0xf3a0, 0x453f, 0x9f, 0x4, 0xdd, 0xa9, 0x23, 0x8a, 0xb5, 0xcf);
