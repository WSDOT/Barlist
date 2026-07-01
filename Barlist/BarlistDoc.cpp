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

// BarlistDoc.cpp : implementation of the CBarlistDoc class
//

#include "stdafx.h"
#include "resource.h"
#include "BarlistApp.h"
#include "BarlistDoc.h"
#include "BarlistFrame.h"
#include "BarlistStatusBar.h"
#include "BendFactory.h"

#include "Barlist.hxx"

#include "OptionsDlg.h"
#include "BarDlg.h"
#include "PropertiesDlg.h"
#include "ReportDlg.h"

#include "Events.h"

#include <Bars\Group.h>
#include <Bars\GroupCollection.h>
#include <Bars\BarRecordCollection.h>
#include <Bars\BarRecord.h>
#include <Bars\BendImpl.h>
#include <Bars\BarException.h>

#include <EAF\EAFMainFrame.h>
#include <EAF\ToolBar.h>

#include <MFCTools\Prompts.h>

#include <System\tokenizer.h>
#include <System\AutoVariable.h>

#include <fstream>

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xalanc/Include/XalanAutoPtr.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMFormatterWalker.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>



//XALAN_USING_XERCES(DOMDocument)

XALAN_USING_XALAN(XalanCompiledStylesheet)
XALAN_USING_XALAN(XalanParsedSource)
XALAN_USING_XALAN(XalanTransformer)

#define MAX_ADDIN_COUNT (LAST_ADDIN_COMMAND - FIRST_ADDIN_COMMAND)

/////////////////////////////////////////////////////////////////////////////
// CBarlistDoc

IMPLEMENT_DYNCREATE(CBarlistDoc, CEAFDocument)

BEGIN_MESSAGE_MAP(CBarlistDoc, CEAFDocument)
	//{{AFX_MSG_MAP(CBarlistDoc)
   ON_COMMAND(ID_VIEW_OPTIONS,OnViewOptions)
   ON_COMMAND(ID_ADD_GROUP, OnAddGroup)
   ON_COMMAND(IDC_ADD_BAR, OnAddBar)
   ON_COMMAND(ID_PROPERTIES, OnProperties)
   ON_COMMAND_RANGE(FIRST_ADDIN_COMMAND,LAST_ADDIN_COMMAND,OnAddin)
   //}}AFX_MSG_MAP
   ON_COMMAND(ID_VIEW_REPORT, &CBarlistDoc::OnViewReport)
END_MESSAGE_MAP()

#define ID_MYTOOLBAR ID_MAINFRAME_TOOLBAR+1
#define PLUGIN_COMMAND_COUNT 256


BOOL LoadXMLResource(int name, int type, DWORD& size, const char*& data)
{
#pragma Reminder("UPDATE: add error handling")
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   HMODULE handle = AfxGetResourceHandle();
   HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
   HGLOBAL rcData = ::LoadResource(handle, rc);
   size = ::SizeofResource(handle, rc);
   data = static_cast<const char*>(::LockResource(rcData));
   return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CBarlistDoc construction/destruction

CBarlistDoc::CBarlistDoc()
{
   EnableUIHints(FALSE); // not using UIHints feature

   // Reserve command IDs for document plug ins
   UINT nCommands = GetPluginCommandManager()->ReserveCommandIDRange(PLUGIN_COMMAND_COUNT);
   ATLASSERT(nCommands == PLUGIN_COMMAND_COUNT);

   m_MarkIncrement = 1;

   m_reportOptions = ReportOptions::REPORT_TOTAL_QUANTITIES;

   EAFGetApp()->RemoveUnitModeListener(this); // hold off on listening for unit mode change events until after the document is loaded
}

CBarlistDoc::~CBarlistDoc()
{
   m_PluginMgr.UnloadPlugins();
}

BOOL CBarlistDoc::Init()
{
   m_BarInfoMgr = std::make_unique<CBarInfoMgr>();
   m_Barlist = std::make_unique<CBarlist>();

   if (!m_PluginMgr.LoadPlugins())
   {
      AfxMessageBox(_T("Failed to load plug-ins"));
      return FALSE;
   }

   return __super::Init();
}


CBarlist& CBarlistDoc::GetBarlist()
{
   return *m_Barlist;
}

CBarInfoMgr& CBarlistDoc::GetBarInfoMgr()
{
   return *m_BarInfoMgr;
}

UINT CBarlistDoc::GetToolbarResourceID()
{
   return IDR_TOOLBAR;
}

void CBarlistDoc::DoIntegrateWithUI(BOOL bIntegrate)
{
   __super::DoIntegrateWithUI(bIntegrate);

   CEAFMainFrame* pFrame = EAFGetMainFrame();
   if (bIntegrate)
   {
      {
         // Setup the toolbar
         AFX_MANAGE_STATE(AfxGetStaticModuleState());
         m_ToolbarID = CreateToolBar(_T("Barlist"));
         auto pToolbar = GetToolBar(m_ToolbarID);
         pToolbar->LoadToolBar(GetToolbarResourceID(), nullptr);
         pToolbar->CreateDropDownButton(ID_FILE_OPEN, nullptr, BTNS_DROPDOWN);

         pToolbar->HideButton(ID_PLACEHOLDER, nullptr, TRUE); // hides the placeholder button that reserves a little extra space
      }

      // Setup the Add-Ins menu
      auto pMainMenu = GetMainMenu();
      UINT addinsPos = pMainMenu->FindMenuItem(_T("Add-Ins"));
      auto pAddins = pMainMenu->GetSubMenu(addinsPos);
      UINT cmdID = FIRST_ADDIN_COMMAND;
      if (pAddins)
      {
         auto nAddins = m_PluginMgr.GetPluginCount();
         nAddins = Min(nAddins, (IndexType)MAX_ADDIN_COUNT);
         if (0 < nAddins)
         {
            pAddins->RemoveMenu(0, MF_BYPOSITION, nullptr); // remove the placeholder
         }

         for (auto i = 0; i < nAddins; i++)
         {
            auto plugin = m_PluginMgr.GetPlugin(i);

            auto strName = plugin->GetMenuItem();

            pAddins->AppendMenu(cmdID++, strName, nullptr);
         }
      }


      // use our own status bar
      CBarlistStatusBar* pSB = new CBarlistStatusBar;
      pSB->Create(pFrame);
      pFrame->SetStatusBar(pSB);
   }
   else
   {
      DestroyToolBar(m_ToolbarID);

      // be a good citizen and clean up our add-ins from the menu
      auto pMainMenu = GetMainMenu();
      UINT addinsPos = pMainMenu->FindMenuItem(_T("Add-Ins"));
      auto pAddins = pMainMenu->GetSubMenu(addinsPos);
      UINT cmdID = FIRST_ADDIN_COMMAND;
      if (pAddins)
      {
         auto nAddins = m_PluginMgr.GetPluginCount();
         nAddins = Min(nAddins, (IndexType)MAX_ADDIN_COUNT);
         for (auto i = 0; i < nAddins; i++)
         {
            pAddins->RemoveMenu(cmdID++, MF_BYCOMMAND, nullptr);
         }
      }

      // reset the status bar
      pFrame->SetStatusBar(nullptr);
   }
}

BOOL CBarlistDoc::OnNewDocument()
{
	if (!CEAFDocument::OnNewDocument())
		return FALSE;

   OnProperties();

   GetBarlistEvents(TRUE);

   return TRUE;
}

BOOL CBarlistDoc::ReadBarlistFromFile(LPCTSTR lpszPathName, CBarlist& barlist)
{
   USES_CONVERSION;

   // The Xalan and Xerses libraries error out when the lpszPathName contains a % character.
   // We are not sure how to fix this problem.
   // As a stop-gap measure, provide an error message the provides instructions to the user
   // rather than a generic error message.
   auto pos = std::_tstring(lpszPathName).find('%');
   if (pos != std::_tstring::npos)
   {
      CString strMsg;
      strMsg.Format(_T("Error opening %s\n\nBarlist files cannot have %% in the name or directory path."), lpszPathName);
      AfxMessageBox(strMsg);
      return FALSE;
   }

   std::auto_ptr<Barlist> barlist_xml;
   try
   {
      XALAN_USING_XERCES(XMLPlatformUtils)

      XMLPlatformUtils::Initialize();
      XalanTransformer::initialize();

      {
         XalanTransformer  theTransformer;
         const XalanParsedSource* theParsedSource = 0;

         int theResult = theTransformer.parseSource(lpszPathName, theParsedSource);

         if (theResult != 0)
         {
            CString strMsg;
            strMsg.Format(_T("Error parsing source document: %s"), A2T(theTransformer.getLastError()));
            AfxMessageBox(strMsg);
            return FALSE;
         }
         else
         {
            ATLASSERT(theParsedSource != 0);

            const XalanCompiledStylesheet*  theCompiledStylesheet = 0;

            const char* lpstrXSLT = nullptr;
            DWORD size = 0;
#pragma Reminder("UPDATE: add error handling")
            if (!LoadXMLResource(IDR_BARLIST_10_TO_20_XSLT, XSLTFILE, size, lpstrXSLT))
            {
               AfxMessageBox(_T("Error loading XSL Transform resource"));
               return FALSE;
            }

            std::string strXSLT(lpstrXSLT, size);
            std::istringstream xsltStream(strXSLT);

            theResult = theTransformer.compileStylesheet(xsltStream, theCompiledStylesheet);
            // Note: create multiple XalanTransformer objects, one for each transformation
            // Version 1.0 -> Version 2.0
            // Version 2.0 -> Version 2.1
            // etc

            if (theResult != 0)
            {
               CString strMsg;
               strMsg.Format(_T("Error compiling stylesheet: %s"), A2T(theTransformer.getLastError()));
               AfxMessageBox(strMsg);
               return FALSE;
            }
            else
            {
               XALAN_USING_XERCES(DOMDocument)
               XALAN_USING_XERCES(DOMImplementation)
               XALAN_USING_XALAN(FormatterToXercesDOM)
               XALAN_USING_XALAN(XalanAutoPtr)

               // This is the document which we'll build...
               const XalanAutoPtr<DOMDocument>     theDocument(DOMImplementation::getImplementation()->createDocument());
               ATLASSERT(theDocument.get() != nullptr);

               // This is a class derived from FormatterListener, which
               // we'll hook up to Xalan's output stage...
               FormatterToXercesDOM    theFormatter(theDocument.get(), 0);

               // Do the transformation...
               int     theResult =
                  theTransformer.transform(
                     *theParsedSource,
                     theCompiledStylesheet,
                     theFormatter);
               // NOTE: do the transformation for each transformer

               if (theResult != 0)
               {
                  CString strMsg;
                  strMsg.Format(_T("Error transforming: %s"), theTransformer.getLastError());
                  AfxMessageBox(strMsg);
                  return FALSE;
               }
               else
               {
                  // Now that theDocument has been converted to the latest format
                  // create the C++ binding
                  xml_schema::properties props;
                  CEAFApp* pApp = EAFGetApp();
                  CString strSchemaFile(pApp->GetAppLocation());
                  strSchemaFile.Append(_T("Barlist.xsd"));
                  props.no_namespace_schema_location(strSchemaFile.GetBuffer());
                  barlist_xml = Barlist_(*theDocument.get()/*lpszPathName*/, 0, props);
               }
            }
         }
      }
      XalanTransformer::terminate();
      XMLPlatformUtils::Terminate();
   }
   catch (const xml_schema::exception& e)
   {
      AfxMessageBox(CString(e.what()), MB_OK);
      return FALSE;
   }

   CreateBarlist(*barlist_xml, barlist);
   return TRUE;
}

BOOL CBarlistDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   if (!ReadBarlistFromFile(lpszPathName, *m_Barlist))
   {
      return FALSE;
   }

   // start getting events after the barlist is loaded, otherwise
   // there will be tons of events fired during loading (we don't want that)
   GetBarlistEvents(TRUE);

   EAFGetApp()->AddUnitModeListener(this); // resume listening for unit mode change events (see constructor)

   return TRUE;
}

Barlist CBarlistDoc::CreateXML(CBarlist& barlist)
{
   // The XSD-generated types below are unconditionally wide (wstring),
   // regardless of this app's TCHAR-ness -- that was a fixed choice by the
   // CodeSynthesis XSD generator. CT2CW adapts our native std::_tstring
   // (TCHAR, varies with the build) to the always-wide XSD side.
   USES_CONVERSION;

   // Properties
   PropertiesType properties(_T(""), _T(""), _T(""), _T(""), _T(""));
   properties.Project() = CT2CW(barlist.GetProject().c_str());
   properties.JobNumber() = CT2CW(barlist.GetJobNumber().c_str());
   properties.Engineer() = CT2CW(barlist.GetEngineer().c_str());
   properties.Company() = CT2CW(barlist.GetCompany().c_str());
   properties.Comments() = CT2CW(barlist.GetComments().c_str());

   Barlist barlistXML(properties);

   for (auto& group : barlist.GetGroups())
   {
      barlistXML.Group().push_back(GroupType((LPCWSTR)CT2CW(group->GetName().c_str())));

      auto& groupXML = barlistXML.Group().back();

      for (auto& bar : group->GetBarRecords())
      {
         MaterialType material = bar->GetMaterial();
         BarRecordType::Material_type mat((BarRecordType::Material_type::value)material);

         BarRecordType::Mark_type mark = (LPCWSTR)CT2CW(bar->GetMark().c_str());
         BarRecordType::Location_type location = (LPCWSTR)CT2CW(bar->GetLocation().c_str());
         BarRecordType::Size_type size = (LPCWSTR)CT2CW(bar->GetSize().c_str());
         BarRecordType::NoReqd_type nReqd = bar->GetNumReqd();
         BarRecordType::BendType_type bendType = bar->GetBendType();

         UseType bar_usage = bar->GetUse();
         BarRecordType::Use_type use((BarRecordType::Use_type)bar_usage);

         // "True"/"False" are compared against the XSD's (always-wide)
         // Substructure_type/Epoxy_type/Varies_type below -- use plain wide
         // literals here rather than _T(), since that boundary is fixed-wide
         // independent of this app's TCHAR-ness.
         BarRecordType::Substructure_type substructure(bar->GetSubstructure() ? L"True" : L"False");
         BarRecordType::Epoxy_type epoxy(bar->GetEpoxy() ? L"True" : L"False");
         bool bVaries = bar->GetVaries();
         BarRecordType::Varies_type varies(bVaries ? L"True" : L"False");

         BarRecordType::NoEach_type nEach = bar->GetNumEach();

         auto primaryBend = bar->GetPrimaryBend();
         BarRecordType::PrimaryBend_type primaryBendXML(primaryBend->GetU(), primaryBend->GetW(), primaryBend->GetX(), primaryBend->GetY(), primaryBend->GetZ(), primaryBend->GetT1(), primaryBend->GetT2());

         BarRecordType barRecordXML(mat, mark, location, size, nReqd, bendType, use, substructure, epoxy, varies, nEach, primaryBendXML);

         if (bVaries)
         {
            auto variesBend = bar->GetVariesBend();
            BarRecordType::VariesBend_type variesBendXML(variesBend->GetU(), variesBend->GetW(), variesBend->GetX(), variesBend->GetY(), variesBend->GetZ(), variesBend->GetT1(), variesBend->GetT2());

            barRecordXML.VariesBend() = variesBendXML;
         }

         groupXML.BarRecord().push_back(barRecordXML);
      }
   }

   return barlistXML;
}

BOOL CBarlistDoc::CreateBarlist(LPCSTR strXML, CBarlist& barlist)
{
   xml_schema::properties props;
   CEAFApp* pApp = EAFGetApp();
   CString strSchemaFile(pApp->GetAppLocation());
   strSchemaFile.Append(_T("Barlist.xsd"));
   props.no_namespace_schema_location(strSchemaFile.GetBuffer());
   std::istringstream is(strXML);
   std::auto_ptr<Barlist> barlistXML = Barlist_(is, 0, props);
   return CreateBarlist(*barlistXML, barlist);
}

BOOL CBarlistDoc::CreateBarlist(Barlist& barlistXML, CBarlist& barlist)
{
   USES_CONVERSION;

   auto properties = barlistXML.Properties();
   barlist.SetProject(std::_tstring((LPCTSTR)CW2CT(properties.Project().c_str())));
   barlist.SetJobNumber(std::_tstring((LPCTSTR)CW2CT(properties.JobNumber().c_str())));
   barlist.SetEngineer(std::_tstring((LPCTSTR)CW2CT(properties.Engineer().c_str())));
   barlist.SetCompany(std::_tstring((LPCTSTR)CW2CT(properties.Company().c_str())));
   barlist.SetComments(std::_tstring((LPCTSTR)CW2CT(properties.Comments().c_str())));

   auto& groups = barlist.GetGroups();
   auto& grps = barlistXML.Group();
   for (auto& grp : grps)
   {
      auto group = groups.Add(std::_tstring((LPCTSTR)CW2CT(grp.Name().c_str())));

      auto& barRecords = group->GetBarRecords();

      auto& barrecs = grp.BarRecord();
      for (auto& barrec : barrecs)
      {
         auto barRecord = std::make_shared<CBarRecord>();

         MaterialType material = (MaterialType)(BarRecordType::Material_type::value)barrec.Material();

         const CBarCollection& bars = m_BarInfoMgr->GetBars(material);
         const CBarData* barData = bars.Find(std::_tstring((LPCTSTR)CW2CT(barrec.Size().c_str())));
         barRecord->SetBarData(barData);

         barRecord->SetMaterial(material);

         barRecord->SetMark(std::_tstring((LPCTSTR)CW2CT(barrec.Mark().c_str())));
         barRecord->SetLocation(std::_tstring((LPCTSTR)CW2CT(barrec.Location().c_str())));
         barRecord->SetNumReqd((long)barrec.NoReqd());

         UseType use = (UseType)barrec.Use();
         barRecord->SetUse(use);

         // Compared against the XSD's (always-wide) Substructure_type/
         // Epoxy_type/Varies_type -- plain wide literal, not _T(), matches
         // the fixed-wide boundary (see CreateXML above).
         bool bSubstructure = (barrec.Substructure() == L"True");
         barRecord->SetSubstructure(bSubstructure);

         bool bEpoxy = (barrec.Epoxy() == L"True");
         barRecord->SetEpoxy(bEpoxy);

         auto primaryBend = CBendFactory::CreateBend((long)barrec.BendType());
         auto& primaryBendDimensions = barrec.PrimaryBend();
         primaryBend->SetU(primaryBendDimensions.U());
         primaryBend->SetW(primaryBendDimensions.W());
         primaryBend->SetX(primaryBendDimensions.X());
         primaryBend->SetY(primaryBendDimensions.Y());
         primaryBend->SetZ(primaryBendDimensions.Z());
         primaryBend->SetT1(primaryBendDimensions.T1());
         primaryBend->SetT2(primaryBendDimensions.T2());
         barRecord->SetPrimaryBend(primaryBend);

         bool bVaries = (barrec.Varies() == L"True");
         if (bVaries)
         {
            auto variesBend = CBendFactory::CreateBend((long)barrec.BendType());
            auto& variesBendDimensions = barrec.VariesBend();
            ATLASSERT(variesBendDimensions.present());
            variesBend->SetU(variesBendDimensions.get().U());
            variesBend->SetW(variesBendDimensions.get().W());
            variesBend->SetX(variesBendDimensions.get().X());
            variesBend->SetY(variesBendDimensions.get().Y());
            variesBend->SetZ(variesBendDimensions.get().Z());
            variesBend->SetT1(variesBendDimensions.get().T1());
            variesBend->SetT2(variesBendDimensions.get().T2());
            barRecord->SetVariesBend(variesBend);

            barRecord->SetNumEach((long)barrec.NoEach());
         }

         barRecords.Add(barRecord);
      }
   }

   return TRUE;
}

BOOL CBarlistDoc::SaveTheDocument(LPCTSTR lpszPathName)
{
   CString strPathName(lpszPathName);
   std::ofstream ofile(strPathName);
   if (ofile.fail() || !ofile.is_open())
   {
      CString strMsg;
      strMsg.Format(_T("Unable to open %s for writing."), lpszPathName);
      AfxMessageBox(strMsg, MB_ICONERROR | MB_OK);
      return FALSE;
   }
   else
   {
      Barlist barlist = CreateXML(*m_Barlist);

      // the no_xml_declaration flag prevents the <?xml...?> processing instruction from being written at the top of the
      // file. the processing instruction was not used in version 4 so we will skip it here so previous versions can open files
      // created with this version
      Barlist_(ofile, barlist, xml_schema::namespace_infomap(), _T("UTF-8"), xml_schema::flags::no_xml_declaration);

      return TRUE;
   }
}

CString CBarlistDoc::GetToolbarSectionName()
{
   return _T("Barlist");
}

BOOL CBarlistDoc::GetStatusBarMessageString(UINT nID,CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return __super::GetStatusBarMessageString(nID,rMessage);
}

BOOL CBarlistDoc::GetToolTipMessageString(UINT nID, CString& rMessage) const
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return __super::GetToolTipMessageString(nID,rMessage);
}

void CBarlistDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::LoadDocumentSettings();

   CWinApp* pApp = AfxGetApp();
   m_MarkIncrement = pApp->GetProfileInt(_T("Settings"), _T("MarkIncrement"), 1);

   WBFL::EAF::UnitMode unitMode = (WBFL::EAF::UnitMode)(pApp->GetProfileInt(_T("Settings"), _T("Units"), (int)WBFL::EAF::UnitMode::US));
   EAFGetApp()->SetUnitsMode(unitMode);

   auto report_options_string = pApp->GetProfileString(_T("Settings"), _T("ReportOptions"),_T("Report Total Quantities"));

   if (report_options_string == _T("Report Total Quantities"))
   {
       m_reportOptions = ReportOptions::REPORT_TOTAL_QUANTITIES;
   }
   else if (report_options_string == _T("Report Total and Group Quantities"))
   {
       m_reportOptions = ReportOptions::REPORT_TOTAL_AND_GROUP_QUANTITIES;
   }
}

void CBarlistDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();

   CWinApp* pApp = AfxGetApp();
   VERIFY(pApp->WriteProfileInt(_T("Settings"), _T("MarkIncrement"), m_MarkIncrement));
   VERIFY(pApp->WriteProfileInt(_T("Settings"), _T("Units"), (int)(EAFGetApp()->GetUnitsMode())));

   if (m_reportOptions == ReportOptions::REPORT_TOTAL_QUANTITIES)
   {
       VERIFY(pApp->WriteProfileString(_T("Settings"), _T("ReportOptions"),_T("Report Total Quantities")));
   }
   else if (m_reportOptions == ReportOptions::REPORT_TOTAL_AND_GROUP_QUANTITIES)
   {
       VERIFY(pApp->WriteProfileString(_T("Settings"), _T("ReportOptions"), _T("Report Total and Group Quantities")));
   }



}

void CBarlistDoc::SetModifiedFlag(BOOL bModified)
{
   __super::SetModifiedFlag(bModified);
   m_bDirtyReport = true;
}


CString CBarlistDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}

/////////////////////////////////////////////////////////////////////////////
// CBarlistDoc diagnostics

#ifdef _DEBUG
void CBarlistDoc::AssertValid() const
{
	CEAFDocument::AssertValid();
}

void CBarlistDoc::Dump(CDumpContext& dc) const
{
	CEAFDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBarlistDoc commands

void CBarlistDoc::OnViewOptions()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   COptionsDlg options;
   CEAFApp* pApp = EAFGetApp();
   options.m_UnitMode = pApp->GetUnitsMode();
   options.m_MarkIncrement = m_MarkIncrement;
   if (options.DoModal() == IDOK)
   {
      pApp->SetUnitsMode(options.m_UnitMode);
      m_MarkIncrement = options.m_MarkIncrement;
   }
}

void CBarlistDoc::OnAddGroup()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CString strGroup;
   BOOL bResult = AfxQuestion(_T("New Group"), _T("Group Name:"), _T("Unnamed"), strGroup);
   if (bResult && strGroup == _T(""))
   {
      AfxMessageBox(_T("The group must have a name"), MB_ICONINFORMATION | MB_OK);
      bResult = false;
   }

   if (bResult)
   {
      try
      {
         m_Barlist->GetGroups().Add(std::_tstring((LPCTSTR)strGroup));
      }
      catch (const CBarException&)
      {
         AfxMessageBox(_T("The barlist already contains a group with the same name."), MB_ICONINFORMATION | MB_OK);
      }
   }
}

void CBarlistDoc::OnAddBar()
{
   long groupIdx = -1;
   long barIdx = -1;
   CView* pView = EAFGetActiveView();
   if (pView)
   {
      CBarlistFrame* pFrame = (CBarlistFrame*)(pView->GetParentFrame());
      if (pFrame)
      {
         CBarlistTreeView* pTree = pFrame->GetTreeView();
         CBarlistListView* pList = pFrame->GetListView();

         groupIdx = pTree->GetSelectedGroup();
         barIdx = pList->GetSelectedBar();
      }
   }

   if (groupIdx == -1)
   {
      if (m_Barlist->GetGroups().Count() == 0)
      {
         AFX_MANAGE_STATE(AfxGetStaticModuleState());
         CString strGroup;
         BOOL bResult = AfxQuestion(_T("New Group"), _T("The barlist must have at least one group before bars can be added.\nGroup Name:"), _T("Unnamed"), strGroup);
         if (bResult)
         {
            m_Barlist->GetGroups().Add(std::_tstring((LPCTSTR)strGroup));
         }
         else
         {
            return;
         }
      }
   }


   EditBar(groupIdx, barIdx);
}

void CBarlistDoc::EditBar(long groupIdx, long barIdx)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBarDlg dlg(*m_BarInfoMgr, *m_Barlist, groupIdx, barIdx);
   dlg.DoModal();
}

void CBarlistDoc::OnProperties()
{
   bool bUpdate = false;
   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState()); // this is scoped intentionally
      CPropertiesDlg dlg;
      dlg.m_Project = m_Barlist->GetProject();
      dlg.m_JobNumber = m_Barlist->GetJobNumber();
      dlg.m_Engineer = m_Barlist->GetEngineer();
      dlg.m_Company = m_Barlist->GetCompany();
      dlg.m_Comments = m_Barlist->GetComments();
      if (dlg.DoModal() == IDOK)
      {
         m_Barlist->SetProject(dlg.m_Project);
         m_Barlist->SetJobNumber(dlg.m_JobNumber);
         m_Barlist->SetEngineer(dlg.m_Engineer);
         m_Barlist->SetCompany(dlg.m_Company);
         m_Barlist->SetComments(dlg.m_Comments);
         bUpdate = true;
      }
   }

   if (bUpdate)
   {
      SetModifiedFlag();
      UpdateAllViews(nullptr, HINT_PROJECT_CHANGED, nullptr); // asserts fire if this is called in the scope of AfxGetStaticModuleState
   }
}

void CBarlistDoc::OnViewReport()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CReportDlg dlg;
   dlg.DoModal();
}

void CBarlistDoc::OnAddin(UINT cmd)
{
   short idx = (short)(cmd - FIRST_ADDIN_COMMAND);
   auto plugin = m_PluginMgr.GetPlugin(idx);
   plugin->Go(*m_Barlist);
}

HINSTANCE CBarlistDoc::GetResourceInstance()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return AfxGetInstanceHandle();
}

void CBarlistDoc::GetBarlistEvents(BOOL bListenForEvents)
{
   if (bListenForEvents)
   {
      if (m_bListeningForBarlistEvents)
      {
         return;
      }

      m_GroupAddedToken = m_Barlist->OnGroupAdded.Connect([this](CGroup& g) { OnGroupAdded(g); });
      m_GroupRemovedToken = m_Barlist->OnGroupRemoved.Connect([this](const std::_tstring& n) { OnGroupRemoved(n); });
      m_GroupChangedToken = m_Barlist->OnGroupChanged.Connect([this](CGroup& g) { OnGroupChanged(g); });
      m_GroupMovedToken = m_Barlist->OnGroupMoved.Connect([this](CGroup& g, long f, long t) { OnGroupMoved(g, f, t); });
      m_BarRecordAddedToken = m_Barlist->OnBarRecordAdded.Connect([this](CGroup& g, CBarRecord& r) { OnBarRecordAdded(g, r); });
      m_BarRecordChangedToken = m_Barlist->OnBarRecordChanged.Connect([this](CGroup& g, CBarRecord& r) { OnBarRecordChanged(g, r); });
      m_BarRecordRemovedToken = m_Barlist->OnBarRecordRemoved.Connect([this](CGroup& g, const std::_tstring& m) { OnBarRecordRemoved(g, m); });
      m_BarRecordsSortedToken = m_Barlist->OnBarRecordsSorted.Connect([this](CGroup& g) { OnBarRecordsSorted(g); });
      m_BarRecordMovedToken = m_Barlist->OnBarRecordMoved.Connect([this](CGroup& g, CBarRecord& r, long f, long t) { OnBarRecordMoved(g, r, f, t); });

      m_bListeningForBarlistEvents = true;
   }
   else
   {
      if (!m_bListeningForBarlistEvents)
      {
         return;
      }

      m_Barlist->OnGroupAdded.Disconnect(m_GroupAddedToken);
      m_Barlist->OnGroupRemoved.Disconnect(m_GroupRemovedToken);
      m_Barlist->OnGroupChanged.Disconnect(m_GroupChangedToken);
      m_Barlist->OnGroupMoved.Disconnect(m_GroupMovedToken);
      m_Barlist->OnBarRecordAdded.Disconnect(m_BarRecordAddedToken);
      m_Barlist->OnBarRecordChanged.Disconnect(m_BarRecordChangedToken);
      m_Barlist->OnBarRecordRemoved.Disconnect(m_BarRecordRemovedToken);
      m_Barlist->OnBarRecordsSorted.Disconnect(m_BarRecordsSortedToken);
      m_Barlist->OnBarRecordMoved.Disconnect(m_BarRecordMovedToken);

      m_bListeningForBarlistEvents = false;
   }
}

void CBarlistDoc::OnUnitsModeChanged(WBFL::EAF::UnitMode newUnitMode)
{
   m_bDirtyReport = true;
   __super::OnUnitsModeChanged(newUnitMode);
   SetModifiedFlag();
}

void CBarlistDoc::DeleteContents()
{
   if (m_Barlist)
   {
      GetBarlistEvents(FALSE);
      m_Barlist.reset();
   }

   __super::DeleteContents();
}

CString CBarlistDoc::GetDocumentationURL()
{
   CString strURL = __super::GetDocumentationURL();
   strURL.Replace(_T("Barlist"), _T("Barlist/Barlist"));
   return strURL;
}

CString CBarlistDoc::GetDocumentationSetName()
{
   CString strDocSetName = __super::GetDocumentationSetName();
   if (m_bGettingDocumentationMapFile)
   {
      strDocSetName += _T("Docs");
   }
   return strDocSetName;
}

CString CBarlistDoc::GetDocumentationMapFile()
{
   WBFL::System::AutoVariable<bool> autoVar(&m_bGettingDocumentationMapFile, true);
   return __super::GetDocumentationMapFile();
}

CString CBarlistDoc::AutoIncrementMark(const CString& strMark) const
{
   CString strNewMark(strMark);
   ULONG mark;
   if (WBFL::System::Tokenizer::ParseULong(strMark, &mark))
   {
      mark += m_MarkIncrement;
      strNewMark.Format(_T("%ld"), mark);
   }
   return strNewMark;
}

void CBarlistDoc::SelectGroup(long groupIdx)
{
   POSITION pos = GetFirstViewPosition();
   while (pos != NULL)
   {
      CView* pView = GetNextView(pos);
      if (pView->IsKindOf(RUNTIME_CLASS(CBarlistTreeView)))
      {
         CBarlistTreeView* pTreeView = (CBarlistTreeView*)pView;
         pTreeView->SelectGroup(groupIdx);
         pos = NULL;
      }
   }
}

std::shared_ptr<CBarRecord> CBarlistDoc::CopyBar(CBarRecord& source) const
{
   auto clone = std::make_shared<CBarRecord>();

   clone->SetMaterial(source.GetMaterial());
   clone->SetMark(source.GetMark());
   clone->SetLocation(source.GetLocation());
   clone->SetNumReqd(source.GetNumReqd());
   clone->SetUse(source.GetUse());
   clone->SetSubstructure(source.GetSubstructure());
   clone->SetEpoxy(source.GetEpoxy());
   clone->SetNumEach(source.GetNumEach());

   clone->SetBarData(source.GetBarData());

   auto primaryBend = source.GetPrimaryBend();
   long bendType = primaryBend->GetBendType();

   auto clonePrimaryBend = CBendFactory::CreateBend(bendType);
   clonePrimaryBend->SetU(primaryBend->GetU());
   clonePrimaryBend->SetW(primaryBend->GetW());
   clonePrimaryBend->SetX(primaryBend->GetX());
   clonePrimaryBend->SetY(primaryBend->GetY());
   clonePrimaryBend->SetZ(primaryBend->GetZ());
   clonePrimaryBend->SetT1(primaryBend->GetT1());
   clonePrimaryBend->SetT2(primaryBend->GetT2());
   clone->SetPrimaryBend(clonePrimaryBend);

   if (source.GetVaries())
   {
      auto variesBend = source.GetVariesBend();

      auto cloneVariesBend = CBendFactory::CreateBend(bendType);
      cloneVariesBend->SetU(variesBend->GetU());
      cloneVariesBend->SetW(variesBend->GetW());
      cloneVariesBend->SetX(variesBend->GetX());
      cloneVariesBend->SetY(variesBend->GetY());
      cloneVariesBend->SetZ(variesBend->GetZ());
      cloneVariesBend->SetT1(variesBend->GetT1());
      cloneVariesBend->SetT2(variesBend->GetT2());
      clone->SetVariesBend(cloneVariesBend);
   }

   return clone;
}

void CBarlistDoc::SetReportOptions(const CBarlistDoc::ReportOptions& reportOptions)
{
    m_reportOptions = reportOptions;
}

CBarlistDoc::ReportOptions CBarlistDoc::GetReportOptions() const
{
    return m_reportOptions;
}

CReport& CBarlistDoc::GetReport()
{
   if (m_bDirtyReport)
   {
      m_Report.BuildReport(*m_Barlist);
      m_bDirtyReport = true;
   }
   return m_Report;
}

void CBarlistDoc::OnGroupAdded(CGroup& group)
{
   SetModifiedFlag();
   CGroupEventHint hint(&group);
   UpdateAllViews(nullptr, HINT_GROUP_ADDED, &hint);
}

void CBarlistDoc::OnGroupRemoved(const std::_tstring& name)
{
   SetModifiedFlag();
   UpdateAllViews(nullptr, HINT_GROUP_REMOVED, nullptr);
}

void CBarlistDoc::OnGroupChanged(CGroup& group)
{
   SetModifiedFlag();
   CGroupEventHint hint(&group);
   UpdateAllViews(nullptr, HINT_GROUP_CHANGED, &hint);
}

void CBarlistDoc::OnGroupMoved(CGroup& group, long idxFrom, long idxTo)
{
   SetModifiedFlag();
   CGroupEventHint hint(&group, idxFrom, idxTo);
   UpdateAllViews(nullptr, HINT_GROUP_MOVED, &hint);
}

void CBarlistDoc::OnBarRecordAdded(CGroup& group, CBarRecord& newRecord)
{
   SetModifiedFlag();
   CBarRecordEventHint hint(&group, &newRecord);
   UpdateAllViews(nullptr, HINT_BAR_ADDED, &hint);
}

void CBarlistDoc::OnBarRecordChanged(CGroup& group, CBarRecord& barRecord)
{
   SetModifiedFlag();
   CBarRecordEventHint hint(&group, &barRecord);
   UpdateAllViews(nullptr, HINT_BAR_CHANGED, &hint);
}

void CBarlistDoc::OnBarRecordRemoved(CGroup& group, const std::_tstring& mark)
{
   SetModifiedFlag();
   CGroupEventHint hint(&group);
   UpdateAllViews(nullptr, HINT_BAR_REMOVED, &hint);
}

void CBarlistDoc::OnBarRecordsSorted(CGroup& group)
{
   SetModifiedFlag();
   CGroupEventHint hint(&group);
   UpdateAllViews(nullptr, HINT_GROUP_SORTED, &hint);
}

void CBarlistDoc::OnBarRecordMoved(CGroup& group, CBarRecord& barRecord, long idxFrom, long idxTo)
{
   SetModifiedFlag();
   CBarRecordEventHint hint(&group, &barRecord, idxFrom, idxTo);
   UpdateAllViews(nullptr, HINT_BAR_MOVED, &hint);
}
