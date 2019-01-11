VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form bxffrmUnits 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Select BXF Units"
   ClientHeight    =   1575
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4305
   Icon            =   "bxffrmUnits.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1575
   ScaleWidth      =   4305
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton cmdCancel 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   3240
      TabIndex        =   4
      Top             =   1080
      Width           =   975
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "&OK"
      Height          =   375
      Left            =   3240
      TabIndex        =   3
      Top             =   600
      Width           =   975
   End
   Begin VB.Frame Frame1 
      Caption         =   "Select System of Units"
      Height          =   1332
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2892
      Begin VB.OptionButton optSI 
         Caption         =   "SI Units"
         Height          =   252
         Left            =   360
         TabIndex        =   2
         Top             =   360
         Value           =   -1  'True
         Width           =   972
      End
      Begin VB.OptionButton optUS 
         Caption         =   "Traditional U.S. Units"
         Height          =   252
         Left            =   360
         TabIndex        =   1
         Top             =   720
         Width           =   2292
      End
   End
   Begin MSComDlg.CommonDialog dlgCommonDialog 
      Left            =   2880
      Top             =   0
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
End
Attribute VB_Name = "bxffrmUnits"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
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

Option Explicit


Private Sub cmdCancel_Click()
    bCancel = True
    Unload Me
    
End Sub

Private Sub cmdOK_Click()
    If optSI.Value = True Then
        SIMode = True
    ElseIf optUS.Value = True Then
        SIMode = False
    Else
    End If
    Unload Me
End Sub


