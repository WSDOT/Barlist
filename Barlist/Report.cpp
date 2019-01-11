///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 2009-2019  Washington State Department of Transportation
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
#include "Report.h"
#include "BarlistDoc.h"

#pragma Reminder("WORKING HERE - implement reporting")

CReport::CReport()
{
}


CReport::~CReport()
{
}

CString CReport::GetReport(IBarlist* pBarlist)
{
   CString strGroups = ReportGroups(pBarlist);
   CString strQNI = ReportQNI(pBarlist);
   CString strSummary = ReportSummary(pBarlist);
   return strGroups + _T("\r\n") + strQNI + _T("\r\n") + strSummary;
}

CString CReport::ReportGroups(IBarlist* pBarlist)
{
   USES_CONVERSION;
   CComPtr<IGroupCollection> groups;
   pBarlist->get_Groups(&groups);

   CString strGroups(_T("\r\n"));
   long nGroups;
   groups->get_Count(&nGroups);
   for (long grpIdx = 0; grpIdx < nGroups; grpIdx++)
   {
      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(grpIdx), &group);

      CComBSTR bstrName;
      group->get_Name(&bstrName);

      CString strName(bstrName);
      strName.MakeUpper();
      CString strGroup;
      strGroup.Format(_T("%4s %-29s\r\n"), _T(""), strName);
      strGroups += strGroup;
      strGroups += ReportBarRecords(group);
      strGroups += _T("\r\n");
   }
   return strGroups;
}

CString CReport::ReportBarRecords(IGroup* pGroup)
{
   CString strBarRecords;
   CComPtr<IBarRecordCollection> bars;
   pGroup->get_BarRecords(&bars);
   long nBars;
   bars->get_Count(&nBars);
   for (long barIdx = 0; barIdx < nBars; barIdx++)
   {
      CComPtr<IBarRecord> bar;
      bars->get_Item(CComVariant(barIdx), &bar);
      strBarRecords += ReportBarRecord(bar);
   }
   return strBarRecords;
}

CString CReport::ReportBarRecord(IBarRecord* pBarRecord)
{
   USES_CONVERSION;

   CString strBarRecord;

   CComBSTR bstrMark;
   pBarRecord->get_Mark(&bstrMark);

   CComBSTR bstrLocation;
   pBarRecord->get_Location(&bstrLocation);

   CComBSTR bstrSize;
   pBarRecord->get_Size(&bstrSize);
   CString strSize(bstrSize);
   strSize.Remove(_T('#'));

   long nReqd;
   pBarRecord->get_NumReqd(&nReqd);

   long bendType;
   pBarRecord->get_BendType(&bendType);

   UseType use;
   pBarRecord->get_Use(&use);

   VARIANT_BOOL vbLumpSum;
   pBarRecord->get_LumpSum(&vbLumpSum);

   VARIANT_BOOL vbSubstructure;
   pBarRecord->get_Substructure(&vbSubstructure);

   VARIANT_BOOL vbEpoxy;
   pBarRecord->get_Epoxy(&vbEpoxy);

   VARIANT_BOOL vbVaries;
   pBarRecord->get_Varies(&vbVaries);

   strBarRecord.Format(_T("%4s %-29s %-2s %4d %2d %c %c %c %c %c "), OLE2T(bstrMark), OLE2T(bstrLocation), strSize, nReqd, bendType, GetUse(use), GetFlag(vbLumpSum, 'L'), GetFlag(vbSubstructure, 'S'), GetFlag(vbEpoxy, 'E'), GetFlag(vbVaries, 'V'));

   if (vbVaries == VARIANT_TRUE)
   {
      long nEach;
      pBarRecord->get_NumEach(&nEach);
      CString strEach;
      strEach.Format(_T("%2d "), nEach);
      strBarRecord += strEach;
   }
   else
   {
      strBarRecord += _T("   ");
   }

   CComPtr<IBend> primaryBend;
   pBarRecord->get_PrimaryBend(&primaryBend);
   strBarRecord += ReportBend(primaryBend,false);

   Float64 mass;
   pBarRecord->get_Mass(&mass);
   strBarRecord += _T(" ") + FormatMass(mass, false);


   if (vbVaries == VARIANT_TRUE)
   {
      CComPtr<IBend> variesBend;
      pBarRecord->get_VariesBend(&variesBend);
      strBarRecord += _T("\r\n") + ReportBend(variesBend, true);
   }

   strBarRecord += _T("\r\n");
   return strBarRecord;
}

TCHAR CReport::GetUse(UseType use)
{
   TCHAR c;
   switch (use)
   {
   case utLongitudinal: c = ' '; break;
   case utSeismic:      c = 'S'; break;
   case utTransverse:   c = 'T'; break;
   }
   return c;
}

inline TCHAR CReport::GetFlag(VARIANT_BOOL vbFlag, TCHAR c)
{
   return vbFlag == VARIANT_TRUE ? c : ' ';
}

CString CReport::ReportBend(IBend* pBend,bool bVaries)
{
   CString strBend;

   if (bVaries)
   {
      strBend.Format(_T("%59s"),_T(" "));
   }

   Float64 u, w, x, y, z, t1, t2;
   pBend->get_U(&u);
   pBend->get_W(&w);
   pBend->get_X(&x);
   pBend->get_Y(&y);
   pBend->get_Z(&z);
   pBend->get_T1(&t1);
   pBend->get_T2(&t2);

   strBend += FormatLength(u, false);
   VARIANT_BOOL vbSupported;
   pBend->get_SupportsDimension(dimW, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (_T(" ") + FormatLength(w, false)) : _T("        ");

   pBend->get_SupportsDimension(dimX, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (_T(" ") + FormatLength(x, false)) : _T("        ");

   pBend->get_SupportsDimension(dimY, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (_T(" ") + FormatLength(y, false)) : _T("        ");

   pBend->get_SupportsDimension(dimZ, &vbSupported);
   strBend += (vbSupported == VARIANT_TRUE) ? (_T(" ") + FormatLength(z, false)) : _T("        ");

   CEAFApp* pApp = EAFGetApp();
   const auto* pDisplayUnits = pApp->GetDisplayUnits();

   pBend->get_SupportsDimension(dimT1, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      CString strT1;
      strT1.Format(_T(" %-3.0f"), ::ConvertFromSysUnits(t1, pDisplayUnits->Angle.UnitOfMeasure));
      strBend += strT1;
   }
   else
   {
      strBend += _T("    ");
   }

   pBend->get_SupportsDimension(dimT2, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      CString strT2;
      strT2.Format(_T(" %-3.0f"), ::ConvertFromSysUnits(t2, pDisplayUnits->Angle.UnitOfMeasure));
      strBend += strT2;
   }
   else
   {
      strBend += _T("    ");
   }

   Float64 length;
   pBend->get_Length(&length);
   strBend += _T(" ") + FormatLength(length, false);

   return strBend;
}

CString CReport::ReportQNI(IBarlist* pBarlist)
{
   Float64 traffic, bridge, wall;
   pBarlist->get_TrafficBarrierQuantity(&traffic);
   pBarlist->get_BridgeGrateInletQuantity(&bridge);
   pBarlist->get_RetainingWallQuantity(&wall);

   CString strQNI;
   strQNI.Format(_T("\r\nQuantities Not Included:\r\nTraffic: %s\r\nBridge:  %s\r\nWall:    %s"), FormatMass(traffic), FormatMass(bridge), FormatMass(wall));
   return strQNI;
}

CString CReport::ReportSummary(IBarlist* pBarlist)
{
   Float64 sub, subEpoxy, super, superEpoxy;
   pBarlist->get_SubstructureMass(&sub);
   pBarlist->get_SubstructureMassEpoxy(&subEpoxy);
   pBarlist->get_SuperstructureMass(&super);
   pBarlist->get_SuperstructureMassEpoxy(&superEpoxy);
   Float64 total = sub + subEpoxy + super + superEpoxy;
   CString strSummary;
   strSummary.Format(_T("\r\nSummary of Quantities:\r\nSubstructure Mass:           %s\r\nSubstructure w/Epoxy Mass:   %s\r\nSuperstructure Mass:         %s\r\nSuperstructure w/Epoxy Mass: %s\r\nGrand Total:                 %s"),
      FormatMass(sub), FormatMass(subEpoxy), FormatMass(super), FormatMass(superEpoxy), FormatMass(total));
   return strSummary;
}

