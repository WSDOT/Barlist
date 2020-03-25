///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2020  Washington State Department of Transportation
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

#include "mfcdual.h"

#include "OptionsDlg.h"
#include "BarDlg.h"
#include "PropertiesDlg.h"
#include "ReportDlg.h"

#include "Events.h"

#include <EAF\EAFMainFrame.h>
#include <EAF\EAFToolBar.h>

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


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

BEGIN_INTERFACE_MAP(CBarlistDoc,CEAFDocument)
   INTERFACE_PART(CBarlistDoc,IID_IBarlistEvents, Events)
END_INTERFACE_MAP()

DELEGATE_CUSTOM_INTERFACE(CBarlistDoc, Events);

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

   m_dwBarlistEventCookie = 0;

   EAFGetApp()->RemoveUnitModeListener(this); // hold off on listening for unit mode change events until after the document is loaded
}

CBarlistDoc::~CBarlistDoc()
{
}

BOOL CBarlistDoc::Init()
{
   HRESULT hr = m_BarInfoMgr.CoCreateInstance(CLSID_BarInfoMgr);
   if (FAILED(hr))
   {
      AfxMessageBox(_T("Failed to create bar information manager"));
      return FALSE;
   }

   hr = m_Barlist.CoCreateInstance(CLSID_Barlist);
   if (FAILED(hr))
   {
      AfxMessageBox(_T("Failed to create barlist"));
      return FALSE;
   }

   hr = m_AddinMgr.CoCreateInstance(CLSID_AddinMgr);
   if (FAILED(hr))
   {
      AfxMessageBox(_T("Failed to create addin manager. Proceeding without Add-ins."));
      return FALSE;
   }

   if (!Formatter::Init())
   {
      return FALSE;
   }

   return __super::Init();
}

void CBarlistDoc::GetBarlist(IBarlist** ppBarlist)
{
   m_Barlist.CopyTo(ppBarlist);
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
         CEAFToolBar* pToolbar = GetToolBar(m_ToolbarID);
         pToolbar->LoadToolBar(GetToolbarResourceID(), nullptr);
         pToolbar->CreateDropDownButton(ID_FILE_OPEN, nullptr, BTNS_DROPDOWN);

         pToolbar->HideButton(ID_PLACEHOLDER, nullptr, TRUE); // hides the placeholder button that reserves a little extra space
      }

      // Setup the Add-Ins menu
      CEAFMenu* pMainMenu = GetMainMenu();
      UINT addinsPos = pMainMenu->FindMenuItem(_T("Add-Ins"));
      CEAFMenu* pAddins = pMainMenu->GetSubMenu(addinsPos);
      UINT cmdID = FIRST_ADDIN_COMMAND;
      if (pAddins)
      {
         long nAddins;
         m_AddinMgr->get_Count(&nAddins);
         nAddins = Min(nAddins, (long)MAX_ADDIN_COUNT);
         if (0 < nAddins)
         {
            pAddins->RemoveMenu(0, MF_BYPOSITION, nullptr); // remove the placeholder
         }

         for (long i = 0; i < nAddins; i++)
         {
            CComPtr<IBarlistAddin> addin;
            m_AddinMgr->get_Item((short)i, &addin);

            CComBSTR bstrName;
            addin->get_MenuItem(&bstrName);

            pAddins->AppendMenu(cmdID++, OLE2T(bstrName), nullptr);
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
      CEAFMenu* pMainMenu = GetMainMenu();
      UINT addinsPos = pMainMenu->FindMenuItem(_T("Add-Ins"));
      CEAFMenu* pAddins = pMainMenu->GetSubMenu(addinsPos);
      UINT cmdID = FIRST_ADDIN_COMMAND;
      if (pAddins)
      {
         long nAddins;
         m_AddinMgr->get_Count(&nAddins);
         nAddins = Min(nAddins, (long)MAX_ADDIN_COUNT);
         for (long i = 0; i < nAddins; i++)
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

   GetBarlistEvents(TRUE);

   return TRUE;
}

BOOL CBarlistDoc::ReadBarlistFromFile(LPCTSTR lpszPathName, IBarlist** ppBarlist)
{
   USES_CONVERSION;

   CComPtr<IBarlist> barlist;
   if (*ppBarlist == nullptr)
   {
      barlist.CoCreateInstance(CLSID_Barlist);
      barlist.CopyTo(ppBarlist);
   }
   else
   {
      barlist = *ppBarlist;
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

   auto properties = barlist_xml->Properties();
   barlist->put_Project(CComBSTR(properties.Project().c_str()));
   barlist->put_JobNumber(CComBSTR(properties.JobNumber().c_str()));
   barlist->put_Engineer(CComBSTR(properties.Engineer().c_str()));
   barlist->put_Company(CComBSTR(properties.Company().c_str()));
   barlist->put_Comments(CComBSTR(properties.Comments().c_str()));

   CComPtr<IGroupCollection> groups;
   barlist->get_Groups(&groups);
   int nGroups = 0;
   auto& grps = barlist_xml->Group();
   for (auto& grp : grps)
   {
      groups->Add(CComBSTR(grp.Name().c_str()));

      CComPtr<IGroup> group;
      groups->get_Item(CComVariant(nGroups++), &group);

      CComPtr<IBarRecordCollection> barRecords;
      group->get_BarRecords(&barRecords);

      auto& barrecs = grp.BarRecord();
      for (auto& barrec : barrecs)
      {
         CComPtr<IBarRecord> barRecord;
         barRecord.CoCreateInstance(CLSID_BarRecord);

         MaterialType material = (MaterialType)(BarRecordType::Material_type::value)barrec.Material();

         CComPtr<IBarCollection> bars;
         m_BarInfoMgr->get_Bars(material, &bars);

         CComPtr<IBarData> barData;
         bars->get_Item(CComVariant(barrec.Size().c_str()), &barData);
         barRecord->put_BarData(barData);

         barRecord->put_Material(material);

         barRecord->put_Mark(CComBSTR(barrec.Mark().c_str()));
         barRecord->put_Location(CComBSTR(barrec.Location().c_str()));
         barRecord->put_NumReqd((long)barrec.NoReqd());

         UseType use = (UseType)barrec.Use();
         barRecord->put_Use(use);


         LCID lcid = 0x0409; // US English
         VARIANT_BOOL vbSubstructure;
         VarBoolFromStr(CComBSTR(barrec.Substructure().c_str()), lcid, VAR_LOCALBOOL, &vbSubstructure);
         barRecord->put_Substructure(vbSubstructure);

         VARIANT_BOOL vbEpoxy;
         VarBoolFromStr(CComBSTR(barrec.Epoxy().c_str()), lcid, VAR_LOCALBOOL, &vbEpoxy);
         barRecord->put_Epoxy(vbEpoxy);

         CComPtr<IBend> primaryBend;
         CBendFactory::CreateBend((long)barrec.BendType(), &primaryBend);
         auto& primaryBendDimensions = barrec.PrimaryBend();
         primaryBend->put_U(primaryBendDimensions.U());
         primaryBend->put_W(primaryBendDimensions.W());
         primaryBend->put_X(primaryBendDimensions.X());
         primaryBend->put_Y(primaryBendDimensions.Y());
         primaryBend->put_Z(primaryBendDimensions.Z());
         primaryBend->put_T1(primaryBendDimensions.T1());
         primaryBend->put_T2(primaryBendDimensions.T2());
         barRecord->put_PrimaryBend(primaryBend);

         VARIANT_BOOL vbVaries;
         VarBoolFromStr(CComBSTR(barrec.Varies().c_str()), 0x0409, VAR_LOCALBOOL, &vbVaries);
         if (vbVaries == VARIANT_TRUE)
         {
            CComPtr<IBend> variesBend;
            CBendFactory::CreateBend((long)barrec.BendType(), &variesBend);
            auto& variesBendDimensions = barrec.VariesBend();
            ATLASSERT(variesBendDimensions.present());
            variesBend->put_U(variesBendDimensions.get().U());
            variesBend->put_W(variesBendDimensions.get().W());
            variesBend->put_X(variesBendDimensions.get().X());
            variesBend->put_Y(variesBendDimensions.get().Y());
            variesBend->put_Z(variesBendDimensions.get().Z());
            variesBend->put_T1(variesBendDimensions.get().T1());
            variesBend->put_T2(variesBendDimensions.get().T2());
            barRecord->put_VariesBend(variesBend);

            barRecord->put_NumEach((long)barrec.NoEach());
         }


         barRecords->Add(barRecord);
      }
   }
   return TRUE;
}

BOOL CBarlistDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   if (!ReadBarlistFromFile(lpszPathName, &m_Barlist.p))
   {
      return FALSE;
   }

   // start getting events after the barlist is loaded, otherwise
   // there will be tons of events fired during loading (we don't want that)
   GetBarlistEvents(TRUE); 

   EAFGetApp()->AddUnitModeListener(this); // resume listening for unit mode change events (see constructor)

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
      CComBSTR bstr;

      // Properties
      PropertiesType properties(_T(""), _T(""), _T(""), _T(""), _T(""));
      m_Barlist->get_Project(&bstr);
      properties.Project() = OLE2T(bstr);

      m_Barlist->get_JobNumber(&bstr);
      properties.JobNumber() = OLE2T(bstr);

      m_Barlist->get_Engineer(&bstr);
      properties.Engineer() = OLE2T(bstr);

      m_Barlist->get_Company(&bstr);
      properties.Company() = OLE2T(bstr);

      m_Barlist->get_Comments(&bstr);
      properties.Comments() = OLE2T(bstr);

      Barlist barlist(properties);

      CComPtr<IGroupCollection> groups;
      m_Barlist->get_Groups(&groups);
      long nGroups;
      groups->get_Count(&nGroups);
      for (long groupIdx = 0; groupIdx < nGroups; groupIdx++)
      {
         CComPtr<IGroup> pGroup;
         groups->get_Item(CComVariant(groupIdx), &pGroup);

         pGroup->get_Name(&bstr);
         barlist.Group().push_back(GroupType(OLE2T(bstr)));

         auto& group = barlist.Group().back();

         CComPtr<IBarRecordCollection> bars;
         pGroup->get_BarRecords(&bars);
         long nBars;
         bars->get_Count(&nBars);
         for (long barIdx = 0; barIdx < nBars; barIdx++)
         {
            CComPtr<IBarRecord> pBar;
            bars->get_Item(CComVariant(barIdx), &pBar);

            MaterialType material;
            pBar->get_Material(&material);
            BarRecordType::Material_type mat((BarRecordType::Material_type::value)material);

            pBar->get_Mark(&bstr);
            BarRecordType::Mark_type mark = OLE2T(bstr);

            pBar->get_Location(&bstr);
            BarRecordType::Location_type location = OLE2T(bstr);

            pBar->get_Size(&bstr);
            BarRecordType::Size_type size = OLE2T(bstr);

            long lValue;
            pBar->get_NumReqd(&lValue);
            BarRecordType::NoReqd_type nReqd = lValue;

            pBar->get_BendType(&lValue);
            BarRecordType::BendType_type bendType = lValue;

            UseType bar_usage;
            pBar->get_Use(&bar_usage);
            BarRecordType::Use_type use((BarRecordType::Use_type)bar_usage);

            VARIANT_BOOL vb;
            pBar->get_Substructure(&vb);
            BarRecordType::Substructure_type substructure(vb == VARIANT_TRUE ? _T("True") : _T("False"));

            pBar->get_Epoxy(&vb);
            BarRecordType::Epoxy_type epoxy(vb == VARIANT_TRUE ? _T("True") : _T("False"));

            VARIANT_BOOL vbVaries;
            pBar->get_Varies(&vbVaries);
            BarRecordType::Varies_type varies(vbVaries == VARIANT_TRUE ? _T("True") : _T("False"));

            pBar->get_NumEach(&lValue);
            BarRecordType::NoEach_type nEach = lValue;

            Float64 u, w, x, y, z, t1, t2;
            CComPtr<IBend> pPrimaryBend;
            pBar->get_PrimaryBend(&pPrimaryBend);
            pPrimaryBend->get_U(&u);
            pPrimaryBend->get_W(&w);
            pPrimaryBend->get_X(&x);
            pPrimaryBend->get_Y(&y);
            pPrimaryBend->get_Z(&z);
            pPrimaryBend->get_T1(&t1);
            pPrimaryBend->get_T2(&t2);

            BarRecordType::PrimaryBend_type primaryBend(u, w, x, y, z, t1, t2);


            BarRecordType barRecord(mat, mark, location, size, nReqd, bendType, use, substructure, epoxy, varies, nEach, primaryBend);

            if (vbVaries == VARIANT_TRUE)
            {
               CComPtr<IBend> pVariesBend;
               pBar->get_VariesBend(&pVariesBend);
               pVariesBend->get_U(&u);
               pVariesBend->get_W(&w);
               pVariesBend->get_X(&x);
               pVariesBend->get_Y(&y);
               pVariesBend->get_Z(&z);
               pVariesBend->get_T1(&t1);
               pVariesBend->get_T2(&t2);
               BarRecordType::VariesBend_type variesBend(u, w, x, y, z, t1, t2);

               barRecord.VariesBend() = variesBend;
            }

            group.BarRecord().push_back(barRecord);
         }
      }

      // the no_xml_declaration flag prevents the <?xml...?> processing instruction from being written at the top of the
      // file. the processing instruction was not used in version 4 so we will skip it here so previous versions can open files
      // created with this version
      Barlist_(ofile, barlist, xml_schema::namespace_infomap(), _T("UTF-8"), xml_schema::flags::no_xml_declaration);

      return TRUE;
   }
}

CString CBarlistDoc::GetToolbarSectionName()
{
   return "Barlist";
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

   CEAFApp* pApp = EAFGetApp();
   m_MarkIncrement = pApp->GetProfileInt(_T("Settings"), _T("MarkIncrement"), 1);

   eafTypes::UnitMode unitMode = (eafTypes::UnitMode)(pApp->GetProfileInt(_T("Settings"), _T("Units"), (int)eafTypes::umUS));
   pApp->SetUnitsMode(unitMode);
}

void CBarlistDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   __super::SaveDocumentSettings();

   CEAFApp* pApp = EAFGetApp();
   VERIFY(pApp->WriteProfileInt(_T("Settings"), _T("MarkIncrement"), m_MarkIncrement));
   VERIFY(pApp->WriteProfileInt(_T("Settings"), _T("Units"), (int)(pApp->GetUnitsMode())));
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
   BOOL bResult = AfxQuestion(_T("New Group"),_T("Group Name:"),_T(""),strGroup);
   if (bResult)
   {
      CComPtr<IGroupCollection> groups;
      m_Barlist->get_Groups(&groups);
      if (FAILED(groups->Add(CComBSTR(strGroup))))
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
      CComPtr<IGroupCollection> groups;
      m_Barlist->get_Groups(&groups);
      long nGroups;
      groups->get_Count(&nGroups);
      if (nGroups == 0)
      {
         AFX_MANAGE_STATE(AfxGetStaticModuleState());
         CString strGroup;
         BOOL bResult = AfxQuestion(_T("New Group"), _T("The barlist must have at least one group before bars can be added.\nGroup Name:"), _T(""), strGroup);
         if (bResult)
         {
            VERIFY(SUCCEEDED(groups->Add(CComBSTR(strGroup))));
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
   CBarDlg dlg(m_BarInfoMgr,m_Barlist,groupIdx,barIdx);
   dlg.DoModal();
}

void CBarlistDoc::OnProperties()
{
   bool bUpdate = false;
   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState()); // this is scoped intentionally
      CPropertiesDlg dlg;
      m_Barlist->get_Project(&dlg.m_Project);
      m_Barlist->get_JobNumber(&dlg.m_JobNumber);
      m_Barlist->get_Engineer(&dlg.m_Engineer);
      m_Barlist->get_Company(&dlg.m_Company);
      m_Barlist->get_Comments(&dlg.m_Comments);
      if (dlg.DoModal() == IDOK)
      {
         m_Barlist->put_Project(dlg.m_Project);
         m_Barlist->put_JobNumber(dlg.m_JobNumber);
         m_Barlist->put_Engineer(dlg.m_Engineer);
         m_Barlist->put_Company(dlg.m_Company);
         m_Barlist->put_Comments(dlg.m_Comments);
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
   CComPtr<IBarlistAddin> addin;
   m_AddinMgr->get_Item(idx, &addin);
   addin->Go(m_Barlist);
}

HINSTANCE CBarlistDoc::GetResourceInstance()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return AfxGetInstanceHandle();
}

void CBarlistDoc::GetBarlistEvents(BOOL bListenForEvents)
{
   CComQIPtr<IConnectionPointContainer> pCPC(m_Barlist);
   CComPtr<IConnectionPoint> pCP;
   HRESULT hr = pCPC->FindConnectionPoint(IID_IBarlistEvents, &pCP);
   if (bListenForEvents)
   {
      hr = pCP->Advise(GetControllingUnknown(), &m_dwBarlistEventCookie);
   }
   else
   {
      if (0 < m_dwBarlistEventCookie)
      {
         hr = pCP->Unadvise(m_dwBarlistEventCookie);
      }
   }
}

void CBarlistDoc::OnUnitsModeChanged(eafTypes::UnitMode newUnitMode)
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
      m_Barlist.Release();
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
   sysAutoVariable<bool> autoVar(&m_bGettingDocumentationMapFile, true);
   return __super::GetDocumentationMapFile();
}

CString CBarlistDoc::AutoIncrementMark(const CString& strMark) const
{
   CString strNewMark(strMark);
   ULONG mark;
   if (sysTokenizer::ParseULong(strMark, &mark))
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

void CBarlistDoc::CopyBar(IBarRecord* pSource, IBarRecord** ppClone) const
{
   CComPtr<IBarRecord> clone;
   clone.CoCreateInstance(CLSID_BarRecord);

   MaterialType material;
   CComBSTR bstrMark;
   CComBSTR bstrLocation;
   long nReqd;
   long nEach;
   UseType use;
   VARIANT_BOOL vbSubstructure;
   VARIANT_BOOL vbEpoxy;
   VARIANT_BOOL vbVaries;
   pSource->get_Material(&material);
   pSource->get_Mark(&bstrMark);
   pSource->get_Location(&bstrLocation);
   pSource->get_NumReqd(&nReqd);
   pSource->get_Use(&use);
   pSource->get_Substructure(&vbSubstructure);
   pSource->get_Epoxy(&vbEpoxy);
   pSource->get_Varies(&vbVaries);
   pSource->get_NumEach(&nEach);

   clone->put_Material(material);
   clone->put_Mark(bstrMark);
   clone->put_Location(bstrLocation);
   clone->put_NumReqd(nReqd);
   clone->put_Use(use);
   clone->put_Substructure(vbSubstructure);
   clone->put_Epoxy(vbEpoxy);
   clone->put_NumEach(nEach);


   CComPtr<IBarData> barData;
   pSource->get_BarData(&barData);
   clone->put_BarData(barData);

   CComPtr<IBend> primaryBend;
   long bendType;
   Float64 u, w, x, y, z, t1, t2;
   pSource->get_PrimaryBend(&primaryBend);
   primaryBend->get_BendType(&bendType);
   primaryBend->get_U(&u);
   primaryBend->get_W(&w);
   primaryBend->get_X(&x);
   primaryBend->get_Y(&y);
   primaryBend->get_Z(&z);
   primaryBend->get_T1(&t1);
   primaryBend->get_T2(&t2);

   CComPtr<IBend> clonePrimaryBend;
   CBendFactory::CreateBend(bendType, &clonePrimaryBend);
   clonePrimaryBend->put_U(u);
   clonePrimaryBend->put_W(w);
   clonePrimaryBend->put_X(x);
   clonePrimaryBend->put_Y(y);
   clonePrimaryBend->put_Z(z);
   clonePrimaryBend->put_T1(t1);
   clonePrimaryBend->put_T2(t2);
   clone->put_PrimaryBend(clonePrimaryBend);

   if (vbVaries == VARIANT_TRUE)
   {
      CComPtr<IBend> variesBend;
      pSource->get_VariesBend(&variesBend);

      variesBend->get_U(&u);
      variesBend->get_W(&w);
      variesBend->get_X(&x);
      variesBend->get_Y(&y);
      variesBend->get_Z(&z);
      variesBend->get_T1(&t1);
      variesBend->get_T2(&t2);

      CComPtr<IBend> cloneVariesBend;
      CBendFactory::CreateBend(bendType, &cloneVariesBend);
      cloneVariesBend->put_U(u);
      cloneVariesBend->put_W(w);
      cloneVariesBend->put_X(x);
      cloneVariesBend->put_Y(y);
      cloneVariesBend->put_Z(z);
      cloneVariesBend->put_T1(t1);
      cloneVariesBend->put_T2(t2);
      clone->put_VariesBend(cloneVariesBend);
   }

   clone.CopyTo(ppClone);
}

CReport& CBarlistDoc::GetReport()
{
   if (m_bDirtyReport)
   {
      m_Report.BuildReport(m_Barlist);
      m_bDirtyReport = false;
   }
   return m_Report;
}

STDMETHODIMP CBarlistDoc::XEvents::OnGroupAdded(IGroup* pGroup)
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   CGroupEventHint hint(pGroup);
   pThis->UpdateAllViews(nullptr, HINT_GROUP_ADDED, &hint);
   return S_OK;
}

STDMETHODIMP CBarlistDoc::XEvents::OnGroupRemoved(BSTR Name)
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   pThis->UpdateAllViews(nullptr, HINT_GROUP_REMOVED, (CObject*)Name);
   return S_OK;
}

STDMETHODIMP CBarlistDoc::XEvents::OnGroupChanged(IGroup* pGroup)
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   CGroupEventHint hint(pGroup);
   pThis->UpdateAllViews(nullptr, HINT_GROUP_CHANGED, &hint);
   return S_OK;
}

STDMETHODIMP CBarlistDoc::XEvents::OnGroupMoved(IGroup* pGroup, long idxFrom, long idxTo)
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   CGroupEventHint hint(pGroup,idxFrom,idxTo);
   pThis->UpdateAllViews(nullptr, HINT_GROUP_MOVED, &hint);
   return S_OK;
}

STDMETHODIMP CBarlistDoc::XEvents::OnBarRecordAdded(IGroup* pGroup, IBarRecord* pNewRecord)
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   CBarRecordEventHint hint(pGroup, pNewRecord);
   pThis->UpdateAllViews(nullptr, HINT_BAR_ADDED, &hint);
   return S_OK;
}

STDMETHODIMP CBarlistDoc::XEvents::OnBarRecordChanged(IGroup* pGroup, IBarRecord* pBarRecord)
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   CBarRecordEventHint hint(pGroup, pBarRecord);
   pThis->UpdateAllViews(nullptr, HINT_BAR_CHANGED, &hint);
   return S_OK;
}

STDMETHODIMP CBarlistDoc::XEvents::OnBarRecordRemoved(IGroup* pGroup, BSTR bstrMark)
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   CGroupEventHint hint(pGroup);
   pThis->UpdateAllViews(nullptr, HINT_BAR_REMOVED, &hint);
   return S_OK;
}

STDMETHODIMP CBarlistDoc::XEvents::OnBarRecordsSorted(IGroup* pGroup)
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   CGroupEventHint hint(pGroup);
   pThis->UpdateAllViews(nullptr, HINT_GROUP_SORTED, &hint);
   return S_OK;
}

STDMETHODIMP CBarlistDoc::XEvents::OnBarRecordMoved(IGroup* pGroup, IBarRecord* pBarRecord, long idxFrom, long idxTo) 
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   CBarRecordEventHint hint(pGroup, pBarRecord, idxFrom, idxTo);
   pThis->UpdateAllViews(nullptr, HINT_BAR_MOVED, &hint);
   return S_OK;
}

