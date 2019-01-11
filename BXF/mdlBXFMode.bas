Attribute VB_Name = "mdlBXFMode"
'///////////////////////////////////////////////////////////////////////
'// Barlist - Reinforcing Steel Weight Estimations
'// Copyright (C) 1999, Washington State Department of Transportation
'//                     Bridge and Structures Office
'//
'// This software was developed as part of the Alternate Route Project
'//
'// This program is free software; you can redistribute it and/or modify
'// it under the terms of the Alternate Route Open Source License as
'// published by the Washington State Department of Transportation,
'// Bridge and Structures Office.
'//
'// This program is distributed in the hope that it will be useful,
'// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
'// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
'// PURPOSE.  See the Alternate Route Open Source License for more details.
'//
'// You should have received a copy of the Alternate Open Source License
'// along with this program; if not, write to the Washington State
'// Department of Transportation, Bridge and Structures Office,
'// 4500 3rd Ave SE, P.O. Box 47340, Olympia, WA 98503, USA or e-mail
'// Bridge_Support@wsdot.wa.gov
'///////////////////////////////////////////////////////////////////////

'This module is needed because:
'  For the form to communicate with the class, the form would
'  have to create a class...BUT the class was created with the
'  frmexplorer form (barlist 4), and since I want this add-in
'  to be completely independent, I couldn't reference the
'  frmexplorer form.  SO to let the class know that the form
'  had been closed (it returns from the vbmodal call) and that
'  an option has been set, I have the form place the data into
'  the following bool member which the class can then look at.

Option Explicit
Public SIMode As Boolean
Public bCancel As Boolean
