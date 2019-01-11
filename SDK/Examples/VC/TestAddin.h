// TestAddin.h : Declaration of the CTestAddin

#ifndef __TESTADDIN_H_
#define __TESTADDIN_H_

#include "resource.h"       // main symbols

#include "BarlistAddinCatid.h"

/////////////////////////////////////////////////////////////////////////////
// CTestAddin
class ATL_NO_VTABLE CTestAddin : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTestAddin, &CLSID_TestAddin>,
   public IBarlistAddin
{
public:
	CTestAddin()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TESTADDIN)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTestAddin)
	COM_INTERFACE_ENTRY(IBarlistAddin)
END_COM_MAP()

BEGIN_CATEGORY_MAP(CTestAddin)
   IMPLEMENTED_CATEGORY(CATID_BarlistAddin)
END_CATEGORY_MAP()

// IBarlistAddin
public:
   STDMETHOD(Go)(/*[in]*/ IBarlist* pBarlist);
   STDMETHOD(get_MenuItem)(/*[out, retval]*/ BSTR *pVal);
};

#endif //__TESTADDIN_H_
