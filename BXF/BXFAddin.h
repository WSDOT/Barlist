///////////////////////////////////////////////////////////////////////
// BXF - Barlist Exchange File
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


// BXFAddin.h : Declaration of the CBXFAddin

#pragma once

#include "BarlistPlugin.h"
#include <EAF\ComponentObject.h>
#include "Barlst.h"

class CBXFApp : public CWinApp
{
public:
   virtual BOOL InitInstance() override;
   virtual int ExitInstance() override;
};

/////////////////////////////////////////////////////////////////////////////
// CBXFAddin
class CBXFAddin : public WBFL::EAF::ComponentObject,
   public IBarlistPlugin
{
public:
	CBXFAddin()
	{
	}

// IBarlistPlugin
public:
   void Init(CEAFDocument* pDoc) override;
   void Terminate() override;
   void Go(IBarlist* pBarlist) override;
   CString GetMenuItem() const override;

private:
   void CreateBarlistExchangeFile(const CString& strFile, IBarlist* pBarlist);
   void ExchangeBarRecords(CStdioFile* pFile, IGroup* pGroup);
   void ExchangeBarRecord(CStdioFile* pFile, IBarRecord* pBarRecord);
   CString ReportBend(IBend* pBend, bool bVaries);
   void ReportErrors(CStdioFile* pFile, IBend* pBend);
   CString GetMaterial(MaterialType material, VARIANT_BOOL vbEpoxy);
};
