///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2024  Washington State Department of Transportation
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

#include <Bars\Barlst.h>

// Returns a text string for the material specification, such as A615 Grade 40
LPCTSTR GetMaterialSpecification(MaterialType material);

// Returns a text string for the material common name, such as Galvinized
LPCTSTR GetMaterialCommonName(MaterialType material);

// Returns the material designation code
CString GetMaterialDesignation(MaterialType material);

// Returns the material grade code
CString GetMaterialGrade(MaterialType material);

inline bool CanBeEpoxyCoated(MaterialType material) { return (int)material < (int)A1035_Grade100; }
inline bool IsA706(MaterialType material) { return (int)A706_Grade60 <= (int)material && (int)material <= (int)A706_Grade80; }
inline bool IsMMFX(MaterialType material) { return (int)A1035_Grade100 <= (int)material && (int)material <= (int)A1035_Grade120; }
inline bool IsGalvanized(MaterialType material) { return (int)A767_A1094_Grade60 <= (int)material && (int)material <= (int)A767_A1094_Grade100; }
inline bool IsStainless(MaterialType material) { return (int)A955_Grade60 <= (int)material && (int)material <= (int)A955_Grade80; }
inline bool IsGFRP(MaterialType materialType) { return (int)D7957 == (int)materialType; }