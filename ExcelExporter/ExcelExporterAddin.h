///////////////////////////////////////////////////////////////////////
// ExcelExporter - Barlist Exchange File
// Copyright © 1999-2025  Washington State Department of Transportation
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

// ExcelExporterAddin.h : Declaration of the CExcelExporterAddin

#include "BarlistPlugin.h"
#include <MfcTools\ExcelWrapper.h>
#include <EAF\ComponentObject.h>
#include "..\Bars\Bars.h"

class CExcelExporterApp : public CWinApp
{
public:
   virtual BOOL InitInstance() override;
   virtual int ExitInstance() override;
};

/////////////////////////////////////////////////////////////////////////////
// CExcelExporterAddin
class CExcelExporterAddin : public WBFL::EAF::ComponentObject,
   public IBarlistPlugin
{
public:
   CExcelExporterAddin();

// IBarlistPlugin
public:
   void Init(CEAFDocument* pDoc) override;
   void Terminate() override;
   void Go(IBarlist* pBarlist) override;
   CString GetMenuItem() const override;

private:
   void ExportToExcel(const CString& strFile, IBarlist* pBarlist);
   void ExportBarRecords(IGroup* pGroup);
   void ExportBarRecord(IBarRecord* pBarRecord);
   void ExportBend(IBend* pBend);
   void ReportErrors(IBend* pBend);
   CString GetMaterial(MaterialType material, VARIANT_BOOL vbEpoxy);

   CString m_strExcelTemplateFolder;
   const CString& GetExcelTemplateFolder() const { return m_strExcelTemplateFolder; }

   _Application m_Excel;
   Worksheets m_Worksheets;

   IndexType m_WorksheetIdx;
   IndexType m_RowIdx;
   IndexType m_MaxRows;
   void NextRow();
   void CreateNewWorksheet();
   void BeforeGroup();
   void BeforeVaries();
   IndexType GetRemainingRows();

   Range GetRangeAtLocation(IndexType worksheetIdx, LPCTSTR strRangeName, IndexType rowIdx, IndexType nRows = INVALID_INDEX);
   void WriteStringToCell(LPCTSTR strRangeName, LPCTSTR strString);
};
