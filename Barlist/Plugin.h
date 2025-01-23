///////////////////////////////////////////////////////////////////////
// Barlist
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

// Plugin.h : Declaration of the CPlugin

#ifndef __PLUGIN_H_
#define __PLUGIN_H_

#include "resource.h"       // main symbols
#include <EAF\EAFAppPlugIn.h>
#include <EAF\EAFAppPluginDocumentationImpl.h>

/////////////////////////////////////////////////////////////////////////////
// CPlugin
class ATL_NO_VTABLE CPlugin : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPlugin, &CLSID_Plugin>,
	public IEAFAppPlugin
{
public:
	CPlugin()
	{
	}

   HRESULT FinalConstruct();
   void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_PLUGIN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPlugin)
	COM_INTERFACE_ENTRY(IEAFAppPlugin)
END_COM_MAP()

// IEAFAppPlugin
public:
   virtual BOOL Init(CEAFApp* pParent) override;
   virtual void Terminate() override;
   virtual void IntegrateWithUI(BOOL bIntegrate) override;
   virtual std::vector<CEAFDocTemplate*> CreateDocTemplates() override;
   virtual HMENU GetSharedMenuHandle() override;
   virtual CString GetName() override;
   virtual CString GetDocumentationSetName() override;
   virtual CString GetDocumentationURL() override;
   virtual CString GetDocumentationMapFile() override;
   virtual void LoadDocumentationMap() override;
   virtual eafTypes::HelpResult GetDocumentLocation(LPCTSTR lpszDocSetName,UINT nID,CString& strURL) override;

private:

   CEAFAppPluginDocumentationImpl m_DocumentationImpl;
};


OBJECT_ENTRY_AUTO(__uuidof(Plugin), CPlugin)

#endif //__PLUGIN_H_
