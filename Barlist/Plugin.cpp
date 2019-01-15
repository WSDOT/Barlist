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

// Plugin.cpp : Implementation of CPlugin
#include "stdafx.h"
#include "resource.h"
#include "Barlist_i.h"
#include "Plugin.h"

#include "BarlistFrame.h"
#include "BarlistTreeView.h"
#include "BarlistDoc.h"
#include "BarlistDocTemplate.h"
#include "CollaborationDoc.h"
#include "CollaborationDocTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPlugin
HRESULT CPlugin::FinalConstruct()
{
   return S_OK;
}

void CPlugin::FinalRelease()
{
}

BOOL CPlugin::Init(CEAFApp* pParent)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   m_DocumentationImpl.Init(this);
   return TRUE;
}

void CPlugin::Terminate()
{
}

void CPlugin::IntegrateWithUI(BOOL bIntegrate)
{
}

std::vector<CEAFDocTemplate*> CPlugin::CreateDocTemplates()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());

   std::vector<CEAFDocTemplate*> templates;

   CEAFDocTemplate* pDocTemplate;
	pDocTemplate = new CBarlistDocTemplate(
      IDR_BARLIST,
      nullptr,
		RUNTIME_CLASS(CBarlistDoc),
		RUNTIME_CLASS(CBarlistFrame),
		RUNTIME_CLASS(CBarlistTreeView),
      nullptr,1);

   pDocTemplate->SetPlugin(this);

   templates.push_back(pDocTemplate);

   pDocTemplate = new CCollaborationDocTemplate(
      IDR_COLLABORATION,
      nullptr,
      RUNTIME_CLASS(CCollaborationDoc),
      RUNTIME_CLASS(CBarlistFrame),
      RUNTIME_CLASS(CBarlistTreeView),
      nullptr, 1);

   pDocTemplate->SetPlugin(this);

   templates.push_back(pDocTemplate);

   return templates;
}

HMENU CPlugin::GetSharedMenuHandle()
{
   return nullptr;
}

CString CPlugin::GetName()
{
   return CString("Barlist");
}

CString CPlugin::GetDocumentationSetName()
{
   return GetName();
}

CString CPlugin::GetDocumentationURL()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return m_DocumentationImpl.GetDocumentationURL();
}

CString CPlugin::GetDocumentationMapFile()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return m_DocumentationImpl.GetDocumentationMapFile();
}

void CPlugin::LoadDocumentationMap()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return m_DocumentationImpl.LoadDocumentationMap();
}

eafTypes::HelpResult CPlugin::GetDocumentLocation(LPCTSTR lpszDocSetName,UINT nID,CString& strURL)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return m_DocumentationImpl.GetDocumentLocation(lpszDocSetName,nID,strURL);
}
