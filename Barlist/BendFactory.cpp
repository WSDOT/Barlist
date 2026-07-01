///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2026  Washington State Department of Transportation
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

#include "stdafx.h"
#include "BendFactory.h"

#include <Bars\BendImpl.h>
#include <Bars\Type50.h>
#include <Bars\Type51.h>
#include <Bars\Type52.h>
#include <Bars\Type53.h>
#include <Bars\Type54.h>
#include <Bars\Type55.h>
#include <Bars\Type56.h>
#include <Bars\Type57.h>
#include <Bars\Type58.h>
#include <Bars\Type59.h>
#include <Bars\Type60.h>
#include <Bars\Type61.h>
#include <Bars\Type62.h>
#include <Bars\Type63.h>
#include <Bars\Type66.h>
#include <Bars\Type67.h>
#include <Bars\Type68.h>
#include <Bars\Type69.h>
#include <Bars\Type70.h>
#include <Bars\Type71.h>
#include <Bars\Type72.h>
#include <Bars\Type73.h>
#include <Bars\Type74.h>
#include <Bars\Type75.h>
#include <Bars\Type76.h>
#include <Bars\Type77.h>
#include <Bars\Type78.h>
#include <Bars\Type79.h>
#include <Bars\Type80.h>
#include <Bars\Type81.h>
#include <Bars\Type82.h>
#include <Bars\Type83.h>
#include <Bars\Type84.h>
#include <Bars\Type89.h>
#include <Bars\Type90.h>
#include <Bars\Type91.h>
#include <Bars\Type92.h>
#include <Bars\Type93.h>
#include <Bars\Type94.h>
#include <Bars\Type95.h>
#include <Bars\Type96.h>
#include <Bars\Type97.h>
#include <Bars\Type98.h>
#include <Bars\Type99.h>

const std::vector<long>& CBendFactory::GetBends()
{
   static std::vector<long> vBends{
      50,51,52,53,54,55,56,57,58,59,60,61,62,63,/*64,65,*/66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,/*85,86,87,88,*/89,90,91,92,93,94,95,96,97,98,99 };
   return vBends;
}

std::shared_ptr<CBend> CBendFactory::CreateBend(long bendType)
{
   switch (bendType)
   {
   case 50: return std::make_shared<CType50>();
   case 51: return std::make_shared<CType51>();
   case 52: return std::make_shared<CType52>();
   case 53: return std::make_shared<CType53>();
   case 54: return std::make_shared<CType54>();
   case 55: return std::make_shared<CType55>();
   case 56: return std::make_shared<CType56>();
   case 57: return std::make_shared<CType57>();
   case 58: return std::make_shared<CType58>();
   case 59: return std::make_shared<CType59>();

   case 60: return std::make_shared<CType60>();
   case 61: return std::make_shared<CType61>();
   case 62: return std::make_shared<CType62>();
   case 63: return std::make_shared<CType63>();
      //case 64: return std::make_shared<CType64>();
      //case 65: return std::make_shared<CType65>();
   case 66: return std::make_shared<CType66>();
   case 67: return std::make_shared<CType67>();
   case 68: return std::make_shared<CType68>();
   case 69: return std::make_shared<CType69>();

   case 70: return std::make_shared<CType70>();
   case 71: return std::make_shared<CType71>();
   case 72: return std::make_shared<CType72>();
   case 73: return std::make_shared<CType73>();
   case 74: return std::make_shared<CType74>();
   case 75: return std::make_shared<CType75>();
   case 76: return std::make_shared<CType76>();
   case 77: return std::make_shared<CType77>();
   case 78: return std::make_shared<CType78>();
   case 79: return std::make_shared<CType79>();

   case 80: return std::make_shared<CType80>();
   case 81: return std::make_shared<CType81>();
   case 82: return std::make_shared<CType82>();
   case 83: return std::make_shared<CType83>();
   case 84: return std::make_shared<CType84>();
      //case 85: return std::make_shared<CType85>();
      //case 86: return std::make_shared<CType86>();
      //case 87: return std::make_shared<CType87>();
      //case 88: return std::make_shared<CType88>();
   case 89: return std::make_shared<CType89>();

   case 90: return std::make_shared<CType90>();
   case 91: return std::make_shared<CType91>();
   case 92: return std::make_shared<CType92>();
   case 93: return std::make_shared<CType93>();
   case 94: return std::make_shared<CType94>();
   case 95: return std::make_shared<CType95>();
   case 96: return std::make_shared<CType96>();
   case 97: return std::make_shared<CType97>();
   case 98: return std::make_shared<CType98>();
   case 99: return std::make_shared<CType99>();

   default:
      ATLASSERT(false); // should never get here
      return std::make_shared<CType50>();
   }
}

CString CBendFactory::GetBendName(long bend)
{
   CString strName;
   if (bend < 0)
   {
      strName = _T("Type Unknown");
   }
   else
   {
      strName.Format(_T("Type %d"), bend);
   }
   return strName;
}
