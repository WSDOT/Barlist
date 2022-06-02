///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2022  Washington State Department of Transportation
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
#include <Bars\Bars.h>

const std::vector<long>& CBendFactory::GetBends()
{
   static std::vector<long> vBends{
      50,51,52,53,54,55,56,57,58,59,60,61,62,63,/*64,65,*/66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,/*85,86,87,88,*/89,90,91,92,93,94,95,96,97,98,99 };
   return vBends;
}

HRESULT CBendFactory::CreateBend(long bendType, IBend** ppBend)
{
   CLSID clsid;
   switch (bendType)
   {
   case 50: clsid = CLSID_Type50; break;
   case 51: clsid = CLSID_Type51; break;
   case 52: clsid = CLSID_Type52; break;
   case 53: clsid = CLSID_Type53; break;
   case 54: clsid = CLSID_Type54; break;
   case 55: clsid = CLSID_Type55; break;
   case 56: clsid = CLSID_Type56; break;
   case 57: clsid = CLSID_Type57; break;
   case 58: clsid = CLSID_Type58; break;
   case 59: clsid = CLSID_Type59; break;

   case 60: clsid = CLSID_Type60; break;
   case 61: clsid = CLSID_Type61; break;
   case 62: clsid = CLSID_Type62; break;
   case 63: clsid = CLSID_Type63; break;
      //case 64: clsid = CLSID_Type64; break;
      //case 65: clsid = CLSID_Type65; break;
   case 66: clsid = CLSID_Type66; break;
   case 67: clsid = CLSID_Type67; break;
   case 68: clsid = CLSID_Type68; break;
   case 69: clsid = CLSID_Type69; break;

   case 70: clsid = CLSID_Type70; break;
   case 71: clsid = CLSID_Type71; break;
   case 72: clsid = CLSID_Type72; break;
   case 73: clsid = CLSID_Type73; break;
   case 74: clsid = CLSID_Type74; break;
   case 75: clsid = CLSID_Type75; break;
   case 76: clsid = CLSID_Type76; break;
   case 77: clsid = CLSID_Type77; break;
   case 78: clsid = CLSID_Type78; break;
   case 79: clsid = CLSID_Type79; break;

   case 80: clsid = CLSID_Type80; break;
   case 81: clsid = CLSID_Type81; break;
   case 82: clsid = CLSID_Type82; break;
   case 83: clsid = CLSID_Type83; break;
   case 84: clsid = CLSID_Type84; break;
      //case 85: clsid = CLSID_Type85; break;
      //case 86: clsid = CLSID_Type86; break;
      //case 87: clsid = CLSID_Type87; break;
      //case 88: clsid = CLSID_Type88; break;
   case 89: clsid = CLSID_Type89; break;

   case 90: clsid = CLSID_Type90; break;
   case 91: clsid = CLSID_Type91; break;
   case 92: clsid = CLSID_Type92; break;
   case 93: clsid = CLSID_Type93; break;
   case 94: clsid = CLSID_Type94; break;
   case 95: clsid = CLSID_Type95; break;
   case 96: clsid = CLSID_Type96; break;
   case 97: clsid = CLSID_Type97; break;
   case 98: clsid = CLSID_Type98; break;
   case 99: clsid = CLSID_Type99; break;

   default:
      ATLASSERT(false); // should never get here
      clsid = CLSID_Type50;
      break;
   }

   CComPtr<IBend> bend;
   HRESULT hr = bend.CoCreateInstance(clsid);
   if (FAILED(hr))
   {
      AfxMessageBox(_T("Error creating bar bend object"), MB_OK);
      return hr;
   }

   return bend.CopyTo(ppBend);
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
