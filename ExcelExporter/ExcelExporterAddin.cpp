///////////////////////////////////////////////////////////////////////
// ExcelExporter - Barlist Exchange File
// Copyright © 1999-2023  Washington State Department of Transportation
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

#include "ExcelExporter_i.h"
#include "ExcelExporterAddin.h"

#include <EAF\EAFApp.h>
#include <EAF\EAFDocument.h>

#include "..\Common\Formatter.h"

CExcelExporterApp theApp;
CComModule _Module;

// Some constants to make the IDispatch calls easier
COleVariant ovOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  // optional parameter
COleVariant ovTrue((short)TRUE); // true
COleVariant ovFalse((short)FALSE); // false

inline bool IsA706(MaterialType material) { return (int)A706_Grade60 <= (int)material && (int)material <= (int)A706_Grade80; }
inline bool IsMMFX(MaterialType material) { return (int)A1035_Grade100 <= (int)material && (int)material <= (int)A1035_Grade120; }
inline bool IsGalvanized(MaterialType material) { return (int)A767_A1094_Grade60 <= (int)material && (int)material <= (int)A767_A1094_Grade100; }
inline bool IsStainless(MaterialType material) { return (int)A955_Grade60 <= (int)material && (int)material <= (int)A955_Grade80; }
inline bool IsGFRP(MaterialType materialType) { return (int)D7957 == (int)materialType; }

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
   eafTypes::UnitMode unitMode = EAFGetApp()->GetUnitsMode();
   CString strGrade;
   switch (material)
   {
   case A706_Grade60:
   case A767_A1094_Grade60:
   case A955_Grade60:
      strGrade = (unitMode == eafTypes::umUS ? _T("  ") : _T("41"));
      break;

   case A955_Grade75:
      strGrade = (unitMode == eafTypes::umUS ? _T("75") : _T("52"));
      break;

   case A706_Grade80:
   case A767_A1094_Grade80:
   case A955_Grade80:
      strGrade = (unitMode == eafTypes::umUS ? _T("80") : _T("55"));
      break;

   case A1035_Grade100:
   case A767_A1094_Grade100:
      strGrade = (unitMode == eafTypes::umUS ? _T("1X") : _T("69"));
      break;

   case A1035_Grade120:
      strGrade = (unitMode == eafTypes::umUS ? _T("12") : _T("83"));
      break;

   case D7957:
      strGrade = _T("  ");
      break;

   default:
      ATLASSERT(false); // is there a new material
      strGrade = _T("??");
   }

   return strGrade;
}


BEGIN_OBJECT_MAP(ObjectMap)
   OBJECT_ENTRY(CLSID_ExcelExporterAddin, CExcelExporterAddin)
END_OBJECT_MAP()

BOOL CExcelExporterApp::InitInstance()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   _Module.Init(ObjectMap, m_hInstance);

   if (!Formatter::Init())
   {
      return FALSE;
   }


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
   case utLongitudinal: c = ' '; break;
   case utSeismic:      c = 'S'; break;
   case utTransverse:   c = 'T'; break;
   }
   return c;
}

inline TCHAR GetFlag(VARIANT_BOOL vbFlag, TCHAR c)
{
   return vbFlag == VARIANT_TRUE ? c : ' ';
}

/////////////////////////////////////////////////////////////////////////////
// CExcelExporterAddin

HRESULT CExcelExporterAddin::FinalConstruct()
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

   return S_OK;
}

STDMETHODIMP CExcelExporterAddin::Go(IBarlist* pBarlist)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (pBarlist == NULL)
   {
      ::MessageBox(0, _T("An invalid barlist was provided."), _T(""), MB_OK);
      return S_FALSE;
   }

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

      ExportToExcel(strFile, pBarlist);

      // don't show a completion window - the excel file will open on completion
      //CString strMsg;
      //strMsg.Format(_T("Barlist exported to Excel\r\n%s"),strFile);
      //AfxMessageBox(strMsg,MB_ICONINFORMATION | MB_OK);
   }
   return S_OK;
}

STDMETHODIMP CExcelExporterAddin::get_MenuItem(BSTR *pVal)
{
   CComBSTR menuItem("Export Barlist to Excel");
   *pVal = menuItem.Copy();
   return S_OK;
}

void CExcelExporterAddin::ExportToExcel(const CString& strFilename, IBarlist* pBarlist)
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

      CComPtr<IGroupCollection> groups;
      pBarlist->get_Groups(&groups);
      long nGroups;
      groups->get_Count(&nGroups);
      for (long grpIdx = 0; grpIdx < nGroups; grpIdx++)
      {
         BeforeGroup(); // this call makes sure the last row in the worksheet isn't a group title... it starts are new worksheet if that's the case

         CComPtr<IGroup> group;
         groups->get_Item(CComVariant(grpIdx), &group);

         CComBSTR bstrName;
         group->get_Name(&bstrName);

         CString strName(bstrName);
         strName.MakeUpper();

         CString strGroup;
         strGroup.Format(_T("%-28s"), strName.GetString());
         WriteStringToCell( _T("Location"), strGroup);
         NextRow();

         ExportBarRecords(group);

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

void CExcelExporterAddin::ExportBarRecords(IGroup* pGroup)
{
   CComPtr<IBarRecordCollection> bars;
   pGroup->get_BarRecords(&bars);
   long nBars;
   bars->get_Count(&nBars);
   for (long barIdx = 0; barIdx < nBars; barIdx++)
   {
      CComPtr<IBarRecord> bar;
      bars->get_Item(CComVariant(barIdx), &bar);
      ExportBarRecord(bar);
      NextRow();
   }
}

void CExcelExporterAddin::ExportBarRecord(IBarRecord* pBarRecord)
{
   USES_CONVERSION;

   VARIANT_BOOL vbVaries;
   pBarRecord->get_Varies(&vbVaries);

   if (vbVaries == VARIANT_TRUE)
   {
      // The current bar record is a varies bar...
      // this call makes sure both lines will fit on the worksheet
      // otherwise the bar record will start on a new worksheet
      BeforeVaries();
   }

   CComBSTR bstrMark;
   pBarRecord->get_Mark(&bstrMark);
   CString strMark;
   strMark.Format(_T("%4s"), OLE2T(bstrMark));
   WriteStringToCell(_T("Mark"), strMark);


   CComBSTR bstrLocation;
   pBarRecord->get_Location(&bstrLocation);
   CString strLocation;
   strLocation.Format(_T("%-28s"), OLE2T(bstrLocation));
   WriteStringToCell(_T("Location"), strLocation);

   CComBSTR bstrSize;
   pBarRecord->get_Size(&bstrSize);
   CString strSize(bstrSize);
   strSize.Remove(_T('#'));
   WriteStringToCell(_T("Size"), strSize);

   long nReqd;
   pBarRecord->get_NumReqd(&nReqd);
   CString strNumReqd;
   strNumReqd.Format(_T("%4d"), nReqd);
   WriteStringToCell(_T("Qty"), strNumReqd);

   long bendType;
   pBarRecord->get_BendType(&bendType);
   CString strBendType;
   strBendType.Format(_T("%2d"), bendType);
   WriteStringToCell(_T("BendType"), strBendType);

   UseType use;
   pBarRecord->get_Use(&use);
   CString strUse(GetUse(use));
   WriteStringToCell(_T("Use"), strUse);

   VARIANT_BOOL vbSubstructure;
   pBarRecord->get_Substructure(&vbSubstructure);
   CString strSubstructure(GetFlag(vbSubstructure, 'S'));
   WriteStringToCell(_T("Substructure"), strSubstructure);

   VARIANT_BOOL vbEpoxy;
   pBarRecord->get_Epoxy(&vbEpoxy);

   MaterialType material;
   pBarRecord->get_Material(&material);

   CString strCoating(GetMaterial(material, vbEpoxy));
   WriteStringToCell(_T("Coating"), strCoating);

   CString strVaries(GetFlag(vbVaries, 'V'));
   WriteStringToCell(_T("Varies"), strVaries);

   if (vbVaries == VARIANT_TRUE)
   {
      CString strEach;
      long nEach;
      pBarRecord->get_NumEach(&nEach);
      strEach.Format(_T("%2d"), nEach);
      WriteStringToCell( _T("NoEach"), strEach);
   }

   CComPtr<IBend> primaryBend;
   pBarRecord->get_PrimaryBend(&primaryBend);
   ExportBend(primaryBend);

   Float64 mass;
   pBarRecord->get_Mass(&mass);
   CString strMass(Formatter::FormatMass(mass, false));
   WriteStringToCell(_T("Weight"), strMass);


   CComPtr<IBend> variesBend;
   if (vbVaries == VARIANT_TRUE)
   {
      NextRow(); // write varies on next row
      pBarRecord->get_VariesBend(&variesBend);
      ExportBend(variesBend);
   }

   ReportErrors(primaryBend);
   ReportErrors(variesBend);
}

void CExcelExporterAddin::ExportBend(IBend* pBend)
{
   long bendType;
   pBend->get_BendType(&bendType);

   Float64 u, w, x, y, z, t1, t2;
   pBend->get_U(&u);
   pBend->get_W(&w);
   pBend->get_X(&x);
   pBend->get_Y(&y);
   pBend->get_Z(&z);
   pBend->get_T1(&t1);
   pBend->get_T2(&t2);

   CString strValue;

   VARIANT_BOOL vbSupported;
   pBend->get_SupportsDimension(dimU, &vbSupported);
   if (vbSupported == VARIANT_TRUE && bendType < 90) // never report U for bendType 90-99
   {
      Int32 feet;
      Float64 inches;
      Formatter::USLength(u, &feet, &inches);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("U_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("U_IN"), strValue);
   }

   pBend->get_SupportsDimension(dimW, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      Int32 feet;
      Float64 inches;
      Formatter::USLength(w, &feet, &inches);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("W_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("W_IN"), strValue);
   }

   pBend->get_SupportsDimension(dimX, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      Int32 feet;
      Float64 inches;
      Formatter::USLength(x, &feet, &inches);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("X_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("X_IN"), strValue);
   }

   pBend->get_SupportsDimension(dimY, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      Int32 feet;
      Float64 inches;
      Formatter::USLength(y, &feet, &inches);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("Y_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("Y_IN"), strValue);
   }

   pBend->get_SupportsDimension(dimZ, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      Int32 feet;
      Float64 inches;
      Formatter::USLength(z, &feet, &inches);
      strValue.Format(_T("%-3d"), feet);
      WriteStringToCell(_T("Z_FT"), strValue);
      strValue.Format(_T("%2.0f"), inches);
      WriteStringToCell(_T("Z_IN"), strValue);
   }

   pBend->get_SupportsDimension(dimT1, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      strValue.Format(_T("%3.0f"), WBFL::Units::ConvertFromSysUnits(t1, WBFL::Units::Measure::Degree));
      WriteStringToCell(_T("Theta1"), strValue);
   }

   pBend->get_SupportsDimension(dimT2, &vbSupported);
   if (vbSupported == VARIANT_TRUE)
   {
      strValue.Format(_T("%3.0f"), WBFL::Units::ConvertFromSysUnits(t2, WBFL::Units::Measure::Degree));
      WriteStringToCell(_T("Theta2"), strValue);
   }

   Float64 length;
   pBend->get_Length(&length);
   Int32 feet;
   Float64 inches;
   Formatter::USLength(length, &feet, &inches);
   strValue.Format(_T("%-3d"), feet);
   WriteStringToCell(_T("Length_FT"), strValue);
   strValue.Format(_T("%2.0f"), inches);
   WriteStringToCell(_T("Length_IN"), strValue);
}

void CExcelExporterAddin::ReportErrors(IBend* pBend)
{
   if (pBend)
   {
      CComPtr<IStatusMessageCollection> statusMessages;
      pBend->get_StatusMessages(&statusMessages);
      long nMessages;
      statusMessages->get_Count(&nMessages);
      for (long i = 0; i < nMessages; i++)
      {
         CComPtr<IStatusMessage> statusMessage;
         statusMessages->get_Item(i, &statusMessage);

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

CString CExcelExporterAddin::GetMaterial(MaterialType material, VARIANT_BOOL vbEpoxy)
{
   CString strMaterial;
   if (vbEpoxy == VARIANT_TRUE)
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
