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
#include "QNIDlg.h"
#include "PropertiesDlg.h"
#include "ReportDlg.h"

#include "Events.h"

#include <EAF\EAFMainFrame.h>
#include <EAF\EAFToolBar.h>

#include <MFCTools\Prompts.h>

#include <System\tokenizer.h>
#include <System\AutoVariable.h>

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_ADDIN_COUNT (LAST_ADDIN_COMMAND - FIRST_ADDIN_COMMAND)

/////////////////////////////////////////////////////////////////////////////
// CBarlistDoc

IMPLEMENT_DYNCREATE(CBarlistDoc, CEAFDocument)

BEGIN_MESSAGE_MAP(CBarlistDoc, CEAFDocument)
	//{{AFX_MSG_MAP(CBarlistDoc)
   ON_COMMAND(ID_VIEW_OPTIONS,OnViewOptions)
   ON_COMMAND(ID_ADD_GROUP, OnAddGroup)
   ON_COMMAND(IDC_ADD_BAR, OnAddBar)
   ON_COMMAND(ID_QNI, OnQNI)
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

#pragma Reminder("WORKING HERE - implement printing")

#include <MfcTools\Format.h>
static std::array<unitmgtMassData, 2> gs_WeightUnit{ unitmgtMassData(unitMeasure::Kilogram,0.001,9,0),unitmgtMassData(unitMeasure::PoundMass,0.001,9,0) };
const unitmgtMassData* g_pWeightUnit = nullptr;

static std::array<unitmgtLengthData, 2> gs_LengthUnit{ unitmgtLengthData(unitMeasure::Meter,0.001,9,0),unitmgtLengthData(unitMeasure::Feet,0.001,9,0) };

CString FormatMass(Float64 mass,bool bUnits)
{
   return FormatDimension(mass, *g_pWeightUnit,bUnits);
}

CComPtr<IAnnotatedDisplayUnitFormatter> g_formatter;
CString FormatLength(Float64 length,bool bUnits)
{
   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      return FormatDimension(length, gs_LengthUnit[0], bUnits);
   }
   else
   {
      USES_CONVERSION;
      CComBSTR bstr;
      g_formatter->Format(::ConvertFromSysUnits(length, gs_LengthUnit[1].UnitOfMeasure), bUnits ? _T("") : nullptr, &bstr);
      return OLE2T(bstr);
   }
}

bool IsValidLength(const CString& strValue, Float64* pValue)
{
   sysTokenizer tokenizer(_T(" "));
   tokenizer.push_back(strValue);
   auto size = tokenizer.size();
   if (size == 1)
   {
      // there is only one token so strValue must be decimal length
      return sysTokenizer::ParseDouble(tokenizer[0].c_str(),pValue);
   }
   else if(size == 2)
   {
      // there are 2 tokens... feet inch
      Float64 ft, in;
      if (!sysTokenizer::ParseDouble(tokenizer[0].c_str(), &ft))
      {
         return false;
      }

      if (!sysTokenizer::ParseDouble(tokenizer[1].c_str(), &in))
      {
         return false;
      }

      *pValue = ft + in / 12.0;
      return true;
   }
   return false;
}

bool ParseLength(const CString& strValue,Float64* pValue)
{
   Float64 value;
   if (!IsValidLength(strValue, &value))
   {
      return false;
   }

   unitmgtLengthData* pLength;
   if (EAFGetApp()->GetUnitsMode() == eafTypes::umSI)
   {
      pLength = &gs_LengthUnit[0];
   }
   else
   {
      pLength = &gs_LengthUnit[1];
   }

   *pValue =  ::ConvertToSysUnits(value, pLength->UnitOfMeasure);
   return true;
}


/////////////////////////////////////////////////////////////////////////////
// CBarlistDoc construction/destruction

CBarlistDoc::CBarlistDoc()
{
   UIHints(FALSE); // not using UIHints feature

   // Reserve command IDs for document plug ins
   UINT nCommands = GetPluginCommandManager()->ReserveCommandIDRange(PLUGIN_COMMAND_COUNT);
   ATLASSERT(nCommands == PLUGIN_COMMAND_COUNT);

   m_MarkIncrement = 1;
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
   }

   if (g_formatter == nullptr)
   {
      g_formatter.CoCreateInstance(CLSID_AnnotatedDisplayUnitFormatter);
      g_formatter->put_Annotation(_T("'-,\""));
      g_formatter->put_Multiplier(12.0);
      g_formatter->put_OffsetDigits(0);
      g_formatter->FormatSpecifiers(7, 1, tjRight, nftFixed, 0.0001);
   }
   g_pWeightUnit = &gs_WeightUnit[EAFGetApp()->GetUnitsMode() == eafTypes::umSI ? 0 : 1];

   return __super::Init();
}

void CBarlistDoc::GetBarlist(IBarlist** ppBarlist)
{
   m_Barlist.CopyTo(ppBarlist);
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
         pToolbar->LoadToolBar(IDR_TOOLBAR, nullptr);
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

BOOL CBarlistDoc::OpenTheDocument(LPCTSTR lpszPathName)
{
   CString strPathName(lpszPathName);
   std::ifstream ifile(strPathName);
   if (ifile.bad())
   {
      return FALSE;
   }
   else
   {
      std::auto_ptr<BarlistType> barlist;
      try
      {
         xml_schema::properties props;
         CEAFApp* pApp = EAFGetApp();
         CString strSchemaFile(pApp->GetAppLocation());
         strSchemaFile.Append(_T("Barlist.xsd"));
         props.no_namespace_schema_location(strSchemaFile.GetBuffer());
         barlist = Barlist(ifile,0,props);
      }
      catch (const xml_schema::exception& e)
      {
         AfxMessageBox(CString(e.what()),MB_OK);
         return FALSE;
      }

      auto properties = barlist->Properties();
      m_Barlist->put_Project(CComBSTR(properties.Project().c_str()));
      m_Barlist->put_JobNumber(CComBSTR(properties.JobNumber().c_str()));
      m_Barlist->put_Engineer(CComBSTR(properties.Engineer().c_str()));
      m_Barlist->put_Company(CComBSTR(properties.Company().c_str()));
      m_Barlist->put_Comments(CComBSTR(properties.Comments().c_str()));

      auto qni = barlist->QNI();
      m_Barlist->put_BridgeGrateInletQuantity(qni.Bridge());
      m_Barlist->put_RetainingWallQuantity(qni.Wall());
      m_Barlist->put_TrafficBarrierQuantity(qni.Traffic());

      CComPtr<IGroupCollection> groups;
      m_Barlist->get_Groups(&groups);
      int nGroups = 0;
      auto& grps = barlist->Group();
      for (auto& grp : grps)
      {
         groups->Add(CComBSTR(grp.Name().c_str()));

         CComPtr<IGroup> group;
         groups->get_Item(CComVariant(nGroups++),&group);

         CComPtr<IBarRecordCollection> barRecords;
         group->get_BarRecords(&barRecords);

         CComPtr<IBarCollection> bars;
         m_BarInfoMgr->get_Bars(&bars);

         auto& barrecs = grp.BarRecord();
         for (auto& barrec : barrecs)
         {
            CComPtr<IBarRecord> barRecord;
            barRecord.CoCreateInstance(CLSID_BarRecord);

            CComPtr<IBarData> barData;
            bars->get_Item(CComVariant(barrec.Size().c_str()),&barData);
            barRecord->put_BarData(barData);

            barRecord->put_Mark(CComBSTR(barrec.Mark().c_str()));
            barRecord->put_Location(CComBSTR(barrec.Location().c_str()));
            barRecord->put_NumReqd((long)barrec.NoReqd());

            UseType use = (UseType)barrec.Use();
            barRecord->put_Use(use);

            
            LCID lcid = 0x0409; // US English
            VARIANT_BOOL vbLumpSum;
            VarBoolFromStr(CComBSTR(barrec.LumpSum().c_str()), lcid, VAR_LOCALBOOL, &vbLumpSum);
            barRecord->put_LumpSum(vbLumpSum);

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

      // start getting events after the barlist is loaded, otherwise
      // there will be loads of events fired during loading (we don't want that)
      GetBarlistEvents(TRUE); 

      return TRUE;
   }
}

BOOL CBarlistDoc::SaveTheDocument(LPCTSTR lpszPathName)
{
   CString strPathName(lpszPathName);
   std::ofstream ofile(strPathName);
   if (ofile.bad())
   {
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

      // Quantities not included
      QNIType qni(0, 0, 0);
      Float64 value;
      m_Barlist->get_BridgeGrateInletQuantity(&value);
      qni.Bridge() = value;

      m_Barlist->get_RetainingWallQuantity(&value);
      qni.Wall() = value;

      m_Barlist->get_TrafficBarrierQuantity(&value);
      qni.Traffic() = value;

      BarlistType barlist(qni,properties);

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

            BarRecordType::Use_type use;
            pBar->get_Use((UseType*)&use);

            VARIANT_BOOL vb;
            pBar->get_LumpSum(&vb);
            BarRecordType::LumpSum_type lumpSum(vb == VARIANT_TRUE ? _T("True") : _T("False"));

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


            BarRecordType barRecord(mark, location, size, nReqd, bendType, use, lumpSum, substructure, epoxy, varies, nEach, primaryBend);

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

      Barlist(ofile, barlist);

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

void CBarlistDoc::OnQNI()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CQNIDlg dlg;
   m_Barlist->get_BridgeGrateInletQuantity(&dlg.m_Bridge);
   m_Barlist->get_TrafficBarrierQuantity(&dlg.m_Traffic);
   m_Barlist->get_RetainingWallQuantity(&dlg.m_Wall);
   if (dlg.DoModal() == IDOK)
   {
      m_Barlist->put_BridgeGrateInletQuantity(dlg.m_Bridge);
      m_Barlist->put_TrafficBarrierQuantity(dlg.m_Traffic);
      m_Barlist->put_RetainingWallQuantity(dlg.m_Wall);
      SetModifiedFlag();
   }
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
   if (bListenForEvents)
   {
      CComQIPtr<IConnectionPointContainer> pCPC(m_Barlist);
      CComPtr<IConnectionPoint> pCP;
      HRESULT hr = pCPC->FindConnectionPoint(IID_IBarlistEvents, &pCP);
      ATLASSERT(SUCCEEDED(hr));
      hr = pCP->Advise(GetControllingUnknown(), &m_dwBarlistEventCookie);
      ATLASSERT(SUCCEEDED(hr));
   }
   else
   {
      CComQIPtr<IConnectionPointContainer> pCPC(m_Barlist);
      CComPtr<IConnectionPoint> pCP;
      HRESULT hr = pCPC->FindConnectionPoint(IID_IBarlistEvents, &pCP);
      ATLASSERT(SUCCEEDED(hr));
      hr = pCP->Unadvise(m_dwBarlistEventCookie);
      ATLASSERT(SUCCEEDED(hr));
   }
}

void CBarlistDoc::OnUnitsModeChanged(eafTypes::UnitMode newUnitMode)
{
   g_pWeightUnit = &gs_WeightUnit[newUnitMode == eafTypes::umSI ? 0 : 1];
   __super::OnUnitsModeChanged(newUnitMode);
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

   CComBSTR bstrMark;
   CComBSTR bstrLocation;
   long nReqd;
   long nEach;
   UseType use;
   VARIANT_BOOL vbLumpSum;
   VARIANT_BOOL vbSubstructure;
   VARIANT_BOOL vbEpoxy;
   VARIANT_BOOL vbVaries;
   pSource->get_Mark(&bstrMark);
   pSource->get_Location(&bstrLocation);
   pSource->get_NumReqd(&nReqd);
   pSource->get_Use(&use);
   pSource->get_LumpSum(&vbLumpSum);
   pSource->get_Substructure(&vbSubstructure);
   pSource->get_Epoxy(&vbEpoxy);
   pSource->get_Varies(&vbVaries);
   pSource->get_NumEach(&nEach);

   clone->put_Mark(bstrMark);
   clone->put_Location(bstrLocation);
   clone->put_NumReqd(nReqd);
   clone->put_Use(use);
   clone->put_LumpSum(vbLumpSum);
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

STDMETHODIMP CBarlistDoc::XEvents::OnNotIncludedQuantitiesChanged()
{
   METHOD_PROLOGUE(CBarlistDoc, Events);
   pThis->SetModifiedFlag();
   return S_OK;
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
