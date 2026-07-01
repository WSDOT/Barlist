///////////////////////////////////////////////////////////////////////
// ExcelExporter - Barlist Exchange File
// Copyright � 1999-2026  Washington State Department of Transportation
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

// ExcelExporterAddin.cpp : Implementation of CExcelExporterAddin
#include "stdafx.h"

#include "ExcelExporterAddin.h"

#include <EAF\EAFApp.h>
#include <EAF\EAFDocument.h>

#include "..\Common\Formatter.h"
#include "CLSID.h"

#include <EAF\ComponentModule.h>

#include <Bars\Barlist.h>
#include <Bars\Group.h>
#include <Bars\GroupCollection.h>
#include <Bars\BarRecordCollection.h>
#include <Bars\BarRecord.h>
#include <Bars\BendImpl.h>
#include <Bars\StatusMessageCollection.h>

CExcelExporterApp theApp;
WBFL::EAF::ComponentModule _Module;

// Some constants to make the IDispatch calls easier
COleVariant ovOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  // optional parameter
COleVariant ovTrue((short)TRUE); // true
COleVariant ovFalse((short)FALSE); // false

inline bool IsA706(MaterialType material) { return (int)MaterialType::A706_Grade60 <= (int)material && (int)material <= (int)MaterialType::A706_Grade80; }
inline bool IsMMFX(MaterialType material) { return (int)MaterialType::A1035_Grade100 <= (int)material && (int)material <= (int)MaterialType::A1035_Grade120; }
inline bool IsGalvanized(MaterialType material) { return (int)MaterialType::A767_A1094_Grade60 <= (int)material && (int)material <= (int)MaterialType::A767_A1094_Grade100; }
inline bool IsStainless(MaterialType material) { return (int)MaterialType::A955_Grade60 <= (int)material && (int)material <= (int)MaterialType::A955_Grade80; }
inline bool IsGFRP(MaterialType materialType) { return (int)MaterialType::D7957 == (int)materialType; }

CString GetMaterialDesignation(MaterialType material)
{
   if (IsA706(material))
   {
      return _T("  ");
   }
   else if (IsMMFX(material))
   {
      return _T("CR");
   }
   else if (IsGalvanized(material))
   {
      return _T("G ");
   }
   else if (IsStainless(material))
   {
      return _T("SS");
   }
   else if (IsGFRP(material))
   {
      return _T("GF");
   }

   ATLASSERT(false);// is there a new type?
   return _T("??");
}

CString GetMaterialGrade(MaterialType material)
{
   WBFL::EAF::UnitMode unitMode = EAFGetApp()->GetUnitsMode();
   CString strGrade;
   switch (material)
   {
   case MaterialType::A706_Grade60:
   case MaterialType::A767_A1094_Grade60:
   case MaterialType::A955_Grade60:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("  ") : _T("41"));
      break;

   case MaterialType::A955_Grade75:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("75") : _T("52"));
      break;

   case MaterialType::A706_Grade80:
   case MaterialType::A767_A1094_Grade80:
   case MaterialType::A955_Grade80:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("80") : _T("55"));
      break;

   case MaterialType::A1035_Grade100:
   case MaterialType::A767_A1094_Grade100:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("1X") : _T("69"));
      break;

   case MaterialType::A1035_Grade120:
      strGrade = (unitMode == WBFL::EAF::UnitMode::US ? _T("12") : _T("83"));
      break;

   case MaterialType::D7957:
      strGrade = _T("  ");
      break;

   default:
      ATLASSERT(false); // is there a new material
      strGrade = _T("??");
   }

   return strGrade;
}


EAF_BEGIN_OBJECT_MAP(ObjectMap)
   EAF_OBJECT_ENTRY(CLSID_ExcelExporterAddin, CExcelExporterAddin)
EAF_END_OBJECT_MAP()

BOOL CExcelExporterApp::InitInstance()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   _Module.Init(ObjectMap);

   return CWinApp::InitInstance();
}

int CExcelExporterApp::ExitInstance()
{
   _Module.Term();
   return CWinApp::ExitInstance();
}

TCHAR GetUse(UseType use)
{
   TCHAR c;
   switch (use)
   {
   case UseType::utLongitudinal: c = ' '; break;
   case UseType::utSeismic:      c = 'S'; break;
   case UseType::utTransverse:   c = 'T'; break;
   }
   return c;
}

inline TCHAR GetFlag(bool bFlag, TCHAR c)
{
   return bFlag ? c : ' ';
}

/////////////////////////////////////////////////////////////////////////////
// CExcelExporterAddin

CExcelExporterAddin::CExcelExporterAddin()
{
   // Get the Excel template file folder
   CEAFApp* pApp = EAFGetApp();
   CString str = pApp->GetAppLocation();

   CString strDefaultLocation;
   if (-1 != str.Find(_T("RegFreeCOM")))
   {
      // application is on a development box
      strDefaultLocation = str.Left(2) + CString(_T("\\ARP\\Barlist\\ExcelExporter\\"));
   }
   else
   {
      // make sure we have a trailing backslash
      if (_T('\\') != str.GetAt(str.GetLength() - 1))
      {
         str += _T("\\");
      }
      strDefaultLocation = str;
   }

   // Get the user's setting, using the local machine setting as the default if not present
   m_strExcelTemplateFolder = pApp->GetProfileString(_T("Settings"), _T("ExcelTemplateFolder"), strDefaultLocation);

   // make sure we have a trailing backslash
   if (_T('\\') != m_strExcelTemplateFolder.GetAt(m_strExcelTemplateFolder.GetLength() - 1))
   {
      m_strExcelTemplateFolder += _T("\\");
   }

   m_MaxRows = 56;
}

void CExcelExporterAddin::Init(CEAFDocument* pDoc)
{
}

void CExcelExporterAddin::Terminate()
{
}

void CExcelExporterAddin::Go(CBarlist& barlist)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   CString strBarlistFile;
   CEAFDocument* pDoc = EAFGetDocument();
   if (pDoc)
   {
      strBarlistFile = pDoc->GetPathName();
      strBarlistFile.Replace(_T(".bar"), _T("")); // regular bar files
      strBarlistFile.Replace(_T(".cbm"), _T("")); // collaboration files
   }

   CString strFile;
   CFileDialog dlg(FALSE, _T("xlsx"), strBarlistFile, OFN_HIDEREADONLY /* | OFN_OVERWRITEPROMPT*/, _T("Microsoft Excel (*.xlsx)|*.xlsx|"));
   // don't prompt for overwrite - Excel will prompt when we save the file
   if (dlg.DoModal() == IDOK)
   {
      POSITION pos = dlg.GetStartPosition();
      while (pos)
      {
         strFile = dlg.GetNextPathName(pos);
      }

      CWaitCursor cursor;

      ExportToExcel(strFile, barlist);

      // don't show a completion window - the excel file will open on completion
      //CString strMsg;
      //strMsg.Format(_T("Barlist exported to Excel\r\n%s"),strFile);
      //AfxMessageBox(strMsg,MB_ICONINFORMATION | MB_OK);
   }
}

CString CExcelExporterAddin::GetMenuItem() const
{
   return CString("Export Barlist to Excel");
}

void CExcelExporterAddin::ExportToExcel(const CString& strFilename, CBarlist& barlist)
{
   CString strTemplateFolder = GetExcelTemplateFolder();
   CString strTemplateName = strTemplateFolder + _T("Barlist_Template.xltx");

   TRY
   {
      if (m_Excel == nullptr)
      {
         // if excel wasn't previously created.... create a new instance
         if (!m_Excel.CreateDispatch(_T("Excel.Application")))
         {
            AfxMessageBox(_T("An error occured while attempting to run Excel. Excel files cannot be created unless Microsoft Excel is installed."));
            return;
         }
      }

      // get the spreadsheet setup
      Workbooks workbooks = m_Excel.GetWorkbooks();
      _Workbook workbook = workbooks.Add(COleVariant(strTemplateName)); // creates a new Excel file from the template

      m_Worksheets = workbook.GetWorksheets();

      // Here is where you find the Excel API documentation
      // https://docs.microsoft.com/en-us/office/vba/api/overview/excel

      m_WorksheetIdx = 1;
      m_RowIdx = 0;

      // making Excel visible here works like a progress window.
      // You can see the spreadsheet being populated 
      m_Excel.SetVisible(TRUE);

      CGroupCollection& groups = barlist.GetGroups();
      for (auto& group : groups)
      {
         BeforeGroup(); // this call makes sure the last row in the worksheet isn't a group title... it starts are new worksheet if that's the case

         CString strName(group->GetName().c_str());
         strName.MakeUpper();

         CString strGroup;
         strGroup.Format(_T("%-28s"), strName.GetString());
         WriteStringToCell( _T("Location"), strGroup);
         NextRow();

         ExportBarRecords(*group);

         NextRow();
      }



      // Save the spreadsheet
      // This looks like it is going to save only one worksheet, but it saves the entire spreadsheet
      _Worksheet ws;
      ws = m_Worksheets.GetItem(COleVariant(1L));
      ws.SaveAs(strFilename,ovOptional,ovOptional,ovOptional,ovOptional,ovOptional,ovOptional,ovOptional,ovOptional,ovOptional);

      // select the first worksheet
      ws.Select(ovTrue);
   }
   CATCH(COleDispatchException, e)
   {
      m_Excel.Quit();
      throw e;
   }
   END_CATCH
}

void CExcelExporterAddin::ExportBarRecords(CGroup& group)
{
   CBarRecordCollection& bars = group.GetBarRecords();
   for (auto& bar : bars)
   {
      ExportBarRecord(*bar);
      NextRow();
   }
}

void CExcelExporterAddin::ExportBarRecord(CBarRecord& barRecord)
{
   bool bVaries = barRecord.GetVaries();

   if (bVaries)
   {
      // The current bar record is a varies bar...
      // this call makes sure both lines will fit on the worksheet
      // otherwise the bar record will start on a new worksheet
      BeforeVaries();
   }

   CString strMark;
   strMark.Format(_T("%4s"), (LPCTSTR)CString(barRecord.GetMark().c_str()));
   WriteStringToCell(_T("Mark"), strMark);

   CString strLocation;
   strLocation.Format(_T("%-28s"), (LPCTSTR)CString(barRecord.GetLocation().c_str()));
   WriteStringToCell(_T("Location"), strLocation);

   CString strSize(barRecord.GetSize().c_str());
   strSize.Remove(_T('#'));
   WriteStringToCell(_T("Size"), strSize);

   long nReqd = barRecord.GetNumReqd();
   CString strNumReqd;
   strNumReqd.Format(_T("%4d"), nReqd);
   WriteStringToCell(_T("Qty"), strNumReqd);

   long bendType = barRecord.GetBendType();
   CString strBendType;
   strBendType.Format(_T("%2d"), bendType);
   WriteStringToCell(_T("BendType"), strBendType);

   UseType use = barRecord.GetUse();
   CString strUse(GetUse(use));
   WriteStringToCell(_T("Use"), strUse);

   bool bSubstructure = barRecord.GetSubstructure();
   CString strSubstructure(GetFlag(bSubstructure, 'S'));
   WriteStringToCell(_T("Substructure"), strSubstructure);

   bool bEpoxy = barRecord.GetEpoxy();
   MaterialType material = barRecord.GetMaterial();

   CString strCoating(GetMaterial(material, bEpoxy));
   WriteStringToCell(_T("Coating"), strCoating);

   CString strVaries(GetFlag(bVaries, 'V'));
   WriteStringToCell(_T("Varies"), strVaries);

   if (bVaries)
   {
      CString strEach;
      long nEach = barRecord.GetNumEach();
      strEach.Format(_T("%2d"), nEach);
      WriteStringToCell( _T("NoEach"), strEach);
   }

   auto primaryBend = barRecord.GetPrimaryBend();
   ExportBend(primaryBend.get());

   Float64 mass = barRecord.GetMass();
   CString strMass(Formatter::FormatMass(mass, false));
   WriteStringToCell(_T("Weight"), strMass);

   std::shared_ptr<CBend> variesBend;
   if (bVaries)
   {
      NextRow(); // write varies on next row
      variesBend = barRecord.GetVariesBend();
      ExportBend(variesBend.get());
   }

   ReportErrors(primaryBend.get());
   ReportErrors(variesBend.get());
}

void CExcelExporterAddin::ExportBend(CBend* pBend)
{
   long bendType = pBend->GetBendType();

   Float64 u = pBend->GetU();
   Float64 w = pBend->GetW();
   Float64 x = pBend->GetX();
   Float64 y = pBend->GetY();
   Float64 z = pBend->GetZ();
   Float64 t1 = pBend->GetT1();
   Float64 t2 = pBend->GetT2();

   CString strValue;

   bool bSupported = pBend->SupportsDimension(DimensionType::dimU);
   if (bSupported && bendType < 90) // never report U for bendType 90-99
   {
      auto [feet, inches] = Formatter::USLength(u);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("U_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("U_IN"), strValue);
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimW);
   if (bSupported)
   {
      auto [feet, inches] = Formatter::USLength(w);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("W_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("W_IN"), strValue);
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimX);
   if (bSupported)
   {
      auto [feet, inches] = Formatter::USLength(x);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("X_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("X_IN"), strValue);
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimY);
   if (bSupported)
   {
      auto [feet, inches] = Formatter::USLength(y);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("Y_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("Y_IN"), strValue);
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimZ);
   if (bSupported)
   {
      auto [feet, inches] = Formatter::USLength(z);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("Z_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("Z_IN"), strValue);
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimT1);
   if (bSupported)
   {
      strValue.Format(_T("%3.0f"), WBFL::Units::ConvertFromSysUnits(t1, WBFL::Units::Measure::Degree));
      WriteStringToCell(_T("Theta1"), strValue);
   }

   bSupported = pBend->SupportsDimension(DimensionType::dimT2);
   if (bSupported)
   {
      strValue.Format(_T("%3.0f"), WBFL::Units::ConvertFromSysUnits(t2, WBFL::Units::Measure::Degree));
      WriteStringToCell(_T("Theta2"), strValue);
   }

   Float64 length = pBend->GetLength();
   auto [feet, inches] = Formatter::USLength(length);
   strValue.Format(_T("%-3d"), feet);
   WriteStringToCell(_T("Length_FT"), strValue);
   strValue.Format(_T("%2.0f"), inches);
   WriteStringToCell(_T("Length_IN"), strValue);
}

void CExcelExporterAddin::ReportErrors(CBend* pBend)
{
   if (pBend)
   {
      for (const auto& statusMessage : pBend->GetStatusMessages())
      {
         CString strStatusMessage = Formatter::FormatStatusMessage(statusMessage);
         strStatusMessage += _T("\n");

         if (strStatusMessage.Left(5).CompareNoCase(_T("Error")) == 0)
         {
            // only report errors... warnings don't go into the barlist drawing
            NextRow();
            WriteStringToCell(_T("Location"), strStatusMessage);

            //// This works, except that when a new sheet is created by making a copy of this
            //// the red text is retained.... skip this for now
            // 
            //// make the error message red
            //Range range = GetRangeAtLocation(m_WorksheetIdx, _T("Location"), m_RowIdx);
            //VARIANT varStyle = range.GetStyle();
            //Style style(varStyle.pdispVal);
            //_Font font = style.GetFont();
            //font.SetColorIndex(COleVariant((short)3)); // red
            //range.SetStyle(varStyle);
         }
      }
   }
}

CString CExcelExporterAddin::GetMaterial(MaterialType material, bool bEpoxy)
{
   CString strMaterial;
   if (bEpoxy)
   {
      strMaterial.Format(_T("%2s%2s"), GetMaterialDesignation(material).GetString(), GetMaterialGrade(material).GetString());
      strMaterial.SetAt(0, _T('E'));
   }
   else
   {
      strMaterial.Format(_T("%2s%2s"), GetMaterialDesignation(material).GetString(), GetMaterialGrade(material).GetString());
   }
   return strMaterial;
}




Range CExcelExporterAddin::GetRangeAtLocation(IndexType worksheetIdx, LPCTSTR strRangeName, IndexType rowIdx,IndexType nRows)
{
   _Worksheet ws = m_Worksheets.GetItem(COleVariant((long)worksheetIdx));

   Range range;
   try
   {
      range = ws.GetRange(COleVariant(strRangeName), COleVariant(strRangeName));
   }
   catch (...)
   {
      CString msg;
      msg.Format(_T("Error - The range \'%s\' was not found on sheet %ld of the template spreadsheet."), strRangeName, worksheetIdx);
      ::AfxMessageBox(msg);
      ATLASSERT(0);
      throw;
   }

   // Get the address of the range in absolute excel format. will look something like: "'[CADExport-Straight]StandardPattern'!$A$5"
   CString address = range.GetAddressLocal(CComVariant(TRUE), CComVariant(TRUE), 1, CComVariant(TRUE), CComVariant());

   // Strip out the file and worksheet names. range name can only contain like $A$4 in the GetRange call
   int cnt = address.GetLength();
   int pn = address.ReverseFind(_T('!'));

   CString trunc_address = address.Right(cnt - pn - 1);

   cnt = trunc_address.GetLength();

   // The row number is the last number in the address
   pn = trunc_address.ReverseFind(_T('$'));

   CString strrow = trunc_address.Right(cnt - pn - 1);

   long rownum;
   if (WBFL::System::Tokenizer::ParseLong(strrow, &rownum))
   {
      rownum += (long)rowIdx;

      CString strrow;
      strrow.Format(_T("%d"), rownum);

      // Build range name using new row number
      CString strFirstRow = trunc_address.Left(pn + 1) + strrow;
      CString strLastRow = strFirstRow;
      if (nRows != INVALID_INDEX)
      {
         strrow.Format(_T("%ld"), rownum + nRows);
         strLastRow = trunc_address.Left(pn + 1) + strrow;
      }

      try
      {
         Range rangenew = ws.GetRange(COleVariant(strFirstRow), COleVariant(strLastRow));

         return rangenew;
      }
      catch (...)
      {
         CString msg;
         msg.Format(_T("Error - The range \'%s\' at row %d was not found on sheet %ld of the template spreadsheet."), strRangeName, rownum, worksheetIdx);
         ::AfxMessageBox(msg);
         ATLASSERT(0);
         throw;
      }
   }
   else
   {
      ATLASSERT(0);
      throw;
   }
}


void CExcelExporterAddin::WriteStringToCell(LPCTSTR strRangeName, LPCTSTR strString)
{
   try
   {
      Range range = GetRangeAtLocation(m_WorksheetIdx, strRangeName, m_RowIdx);
      range.SetValue2(COleVariant(strString));
      range.SetWrapText(ovFalse); // don't text wrap
   }
   catch (...)
   {
      ATLASSERT(false);
      return;
   }
}

void CExcelExporterAddin::NextRow()
{
   if (m_RowIdx == m_MaxRows)
   {
      // no more room, create a new worksheet
      CreateNewWorksheet();
   }
   else
   {
      m_RowIdx++;
   }
}

void CExcelExporterAddin::CreateNewWorksheet()
{
   _Worksheet ws = m_Worksheets.GetItem(COleVariant((long)m_WorksheetIdx));
   ws.Copy(ovOptional, CComVariant(ws));
   m_WorksheetIdx++;
   m_RowIdx = 0;

   _Worksheet new_ws = m_Worksheets.GetItem(COleVariant((long)m_WorksheetIdx));
   CString strName;
   strName.Format(_T("Sheet%ld"), m_WorksheetIdx);
   new_ws.SetName(strName);
   // need to delete contents that got copied

   std::vector<CString> strNames{ _T("Mark"),_T("Location"),_T("Size"), _T("Qty"), _T("BendType"), _T("Use"), _T("Substructure"), _T("Coating"), _T("Varies"), _T("NoEach"),
      _T("U_FT"), _T("U_IN"), _T("W_FT"), _T("W_IN"), _T("X_FT"), _T("X_IN"), _T("Y_FT"), _T("Y_IN"), _T("Z_FT"), _T("Z_IN"), _T("Theta1"), _T("Theta2"), _T("Length_FT"), _T("Length_IN"),
      _T("Weight") };

   for (const CString& strName : strNames)
   {
      Range range = GetRangeAtLocation(m_WorksheetIdx, strName, m_RowIdx, m_MaxRows);
      range.ClearContents();
   }
}

void CExcelExporterAddin::BeforeGroup()
{
   if (GetRemainingRows() < 2)
   {
      // there is only room for the group title on this worksheet
      // start a new worksheet so there isn't a dangling group title
      CreateNewWorksheet();
   }
}

void CExcelExporterAddin::BeforeVaries()
{
   if (GetRemainingRows() < 2)
   {
      // the varies bar won't fit on this worksheet, so start a new one
      CreateNewWorksheet();
   }
}

IndexType CExcelExporterAddin::GetRemainingRows()
{
   // returns the number of rows remaining in the worksheet
   return m_MaxRows - m_RowIdx + 1;
}
