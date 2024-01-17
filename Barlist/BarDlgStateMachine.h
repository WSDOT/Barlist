///////////////////////////////////////////////////////////////////////
// Barlist
// Copyright © 1999-2024  Washington State Department of Transportation
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

#pragma once

class CBarDlg;

enum class State 
{ 
   Editing, // dialog is in edit mode, but nothing has been changed
   EditingWithChange, // dialog is in edit mode and some parameters have been changed
   NewBar, // dialog is in edit mode and we are editing a new bar record (by inference there are pending changes)
   NoBars, // dialog is in edit mode but there aren't any bars in the group and no edits have been made
   EmptyList // there are no bars in the barlist... the only thing that can be done is creating a new group
};

enum class Action 
{
   AddGroup, // a group was added [Add Group]
   AddRecord, // a bar record was added [Add Bar]
   UpdateRecord, // a bar record was updated [Update]
   DeleteRecord, // a bar record was deleted [Delete]
   EditRecord, // some editing action happened (changed mark number, location, bend type, dimension, etc)
   SelectGroup // a group was selected
};

class CBarDlgStateMachine
{
public:
   CBarDlgStateMachine(CBarDlg* pBarDlg);
   void StateChange(Action action);
   void SetState(State state);
   State GetState() const;

private:
   CBarDlg* m_pBarDlg;
   State m_State;
};
