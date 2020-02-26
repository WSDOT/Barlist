///////////////////////////////////////////////////////////////////////
// BXF - Barlist Exchange File
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


// BXFAddin.h : Declaration of the CBXFAddin

#ifndef __BXFADDIN_H_
#define __BXFADDIN_H_

#include "resource.h"       // main symbols

#include "BarlistAddinCatid.h"

class CBXFApp : public CWinApp
{
public:
   virtual BOOL InitInstance() override;
   virtual int ExitInstance() override;
};

/////////////////////////////////////////////////////////////////////////////
// CBXFAddin
class ATL_NO_VTABLE CBXFAddin : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBXFAddin, &CLSID_BXFAddin>,
   public IBarlistAddin
{
public:
	CBXFAddin()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BXFADDIN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBXFAddin)
	COM_INTERFACE_ENTRY(IBarlistAddin)
END_COM_MAP()

BEGIN_CATEGORY_MAP(CBXFAddin)
   IMPLEMENTED_CATEGORY(CATID_BarlistAddin)
END_CATEGORY_MAP()


// IBarlistAddin
public:
   STDMETHOD(Go)(/*[in]*/ IBarlist* pBarlist);
   STDMETHOD(get_MenuItem)(/*[out, retval]*/ BSTR *pVal);
private:
   void CreateBarlistExchangeFile(const CString& strFile, IBarlist* pBarlist);
   void ExchangeBarRecords(CStdioFile* pFile, IGroup* pGroup);
   void ExchangeBarRecord(CStdioFile* pFile, IBarRecord* pBarRecord);
   CString ReportBend(IBend* pBend, bool bVaries);
   void ReportErrors(CStdioFile* pFile, IBend* pBend);
   CString GetMaterial(MaterialType material, VARIANT_BOOL vbEpoxy);
};

#endif //__BXFADDIN_H_
