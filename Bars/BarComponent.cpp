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


// BarComponent.cpp: implementation of the CBarComponent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BarComponent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarComponent::CBarComponent()
{

}

CBarComponent::~CBarComponent()
{
   Clear();
}

void CBarComponent::Clear()
{
   std::vector<CBarComponent*>::iterator iter;
   for ( iter = m_Components.begin(); iter != m_Components.end(); iter++ )
   {
      CBarComponent* pComponent = *iter;
      delete pComponent;
   }
   m_Components.clear();
}

bool CBarComponent::Build()
{
   Clear();
   return BuildComponent();
}

void CBarComponent::AddBarComponent(CBarComponent* pComponent)
{
   m_Components.push_back(pComponent);
}

Float64 CBarComponent::Length()
{
   Build();

   Float64 length = 0;
   std::vector<CBarComponent*>::iterator iter = m_Components.begin();
   std::vector<CBarComponent*>::iterator end = m_Components.end();
   for ( ; iter != end; iter++)
   {
      CBarComponent* pComponent = *iter;
      length += pComponent->Length();
   }

   return length;
}