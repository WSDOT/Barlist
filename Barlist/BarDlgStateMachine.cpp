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

#include "stdafx.h"
#include "BarDlgStateMachine.h"
#include "BarDlg.h"

CBarDlgStateMachine::CBarDlgStateMachine(CBarDlg* pBarDlg) :
   m_State(State::EmptyList),
   m_pBarDlg(pBarDlg)
{

}

void CBarDlgStateMachine::StateChange(Action action)
{
   switch (action)
   {
   case Action::AddGroup:
      m_State = State::NewBar;
      m_pBarDlg->OnStateChanged();
      break;

   case Action::AddRecord:
      m_State = State::Editing;
      m_pBarDlg->OnStateChanged();
      break;

   case Action::UpdateRecord:
      m_State = State::Editing;
      m_pBarDlg->OnStateChanged();
      break;

   case Action::DeleteRecord:
   case Action::SelectGroup:
      if (m_pBarDlg->GetBarRecordCount() == 0)
      {
         // there aren't any more bar records
         m_State = State::NoBars;
      }
      else
      {
         m_State = State::Editing;
      }
      m_pBarDlg->OnStateChanged();
      break;

   case Action::EditRecord:
      if (m_State == State::Editing)
      {
         m_State = State::EditingWithChange;
         m_pBarDlg->OnStateChanged();
      }
      break;

   default:
      break; // do nothing
   }
}

void CBarDlgStateMachine::SetState(State state)
{
   if (m_State != state)
   {
      m_State = state;
      m_pBarDlg->OnStateChanged();
   }
}

State CBarDlgStateMachine::GetState() const
{
   return m_State;
}