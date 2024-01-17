///////////////////////////////////////////////////////////////////////
// Bars.dll - Automation Engine for Reinforcing Steel Weight Estimations
// Copyright © 1999-2024  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This software was developed as part of the Alternate Route Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation,
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful,
// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
// PURPOSE.  See the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Open Source License
// along with this program; if not, write to the Washington State
// Department of Transportation, Bridge and Structures Office,
// 4500 3rd Ave SE, P.O. Box 47340, Olympia, WA 98503, USA or e-mail
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////


// BarException.h: interface for the CBarException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAREXCEPTION_H__F0FE7689_2F3E_11D3_8958_006097C68A9C__INCLUDED_)
#define AFX_BAREXCEPTION_H__F0FE7689_2F3E_11D3_8958_006097C68A9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CBarException  
{
public:
	CBarException(const char* lpszReason);
	virtual ~CBarException();

   std::string GetReason() const;

private:

   std::string m_Reason;
};

#endif // !defined(AFX_BAREXCEPTION_H__F0FE7689_2F3E_11D3_8958_006097C68A9C__INCLUDED_)
