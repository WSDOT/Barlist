VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.3#0"; "COMCTL32.OCX"
Begin VB.Form frmBarlist 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Raw Barlist Data"
   ClientHeight    =   4644
   ClientLeft      =   36
   ClientTop       =   264
   ClientWidth     =   6024
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4644
   ScaleWidth      =   6024
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin ComctlLib.TreeView tvGroups 
      Height          =   3972
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   5772
      _ExtentX        =   10181
      _ExtentY        =   7006
      _Version        =   327682
      Indentation     =   176
      LineStyle       =   1
      Style           =   7
      Appearance      =   1
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "&OK"
      Height          =   372
      Left            =   2520
      TabIndex        =   0
      Top             =   4200
      Width           =   972
   End
End
Attribute VB_Name = "frmBarlist"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public barlist As BARSLib.barlist

Private Sub cmdOK_Click()
   Unload Me
End Sub

Private Sub InitBarlistTree()
   
   Dim group As group
   For Each group In barlist.Groups
      tvGroups.Nodes.Add , , group.Name, group.Name
      
      
      Dim barrecord As barrecord
      For Each barrecord In group.BarRecords
         tvGroups.Nodes.Add group.Name, tvwChild, barrecord.Mark, "Mark = " & barrecord.Mark
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Location = " & barrecord.Location
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Size = " & barrecord.Size
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "No. Reqd. = " & barrecord.NumReqd
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Bend Type = " & barrecord.BendType
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Use = " & barrecord.Use
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Lump Sump = " & barrecord.LumpSum
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Substructure = " & barrecord.Substructure
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Epoxy = " & barrecord.Epoxy
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Varies = " & barrecord.Varies

         tvGroups.Nodes.Add barrecord.Mark, tvwChild, barrecord.Mark + "Dim", "Dimensions"
         tvGroups.Nodes.Add barrecord.Mark + "Dim", tvwChild, , "U = " & barrecord.PrimaryBend.U
         tvGroups.Nodes.Add barrecord.Mark + "Dim", tvwChild, , "W = " & barrecord.PrimaryBend.W
         tvGroups.Nodes.Add barrecord.Mark + "Dim", tvwChild, , "X = " & barrecord.PrimaryBend.X
         tvGroups.Nodes.Add barrecord.Mark + "Dim", tvwChild, , "Y = " & barrecord.PrimaryBend.Y
         tvGroups.Nodes.Add barrecord.Mark + "Dim", tvwChild, , "Z = " & barrecord.PrimaryBend.Z
         tvGroups.Nodes.Add barrecord.Mark + "Dim", tvwChild, , "T1 = " & barrecord.PrimaryBend.T1
         tvGroups.Nodes.Add barrecord.Mark + "Dim", tvwChild, , "T2 = " & barrecord.PrimaryBend.T2

         If (barrecord.Varies) Then
            tvGroups.Nodes.Add barrecord.Mark, tvwChild, barrecord.Mark + "VDim", "Varies Dimensions"
            tvGroups.Nodes.Add barrecord.Mark + "VDim", tvwChild, , "U = " & barrecord.VariesBend.U
            tvGroups.Nodes.Add barrecord.Mark + "VDim", tvwChild, , "W = " & barrecord.VariesBend.W
            tvGroups.Nodes.Add barrecord.Mark + "VDim", tvwChild, , "X = " & barrecord.VariesBend.X
            tvGroups.Nodes.Add barrecord.Mark + "VDim", tvwChild, , "Y = " & barrecord.VariesBend.Y
            tvGroups.Nodes.Add barrecord.Mark + "VDim", tvwChild, , "Z = " & barrecord.VariesBend.Z
            tvGroups.Nodes.Add barrecord.Mark + "VDim", tvwChild, , "T1 = " & barrecord.VariesBend.T1
            tvGroups.Nodes.Add barrecord.Mark + "VDim", tvwChild, , "T2 = " & barrecord.VariesBend.T2
         End If

         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Length = " & barrecord.PrimaryBend.Length
         If (barrecord.Varies) Then
            tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Varies Length = " & barrecord.VariesBend.Length
         End If
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Mass = " & barrecord.Mass
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, , "Status = " & barrecord.Status

          'Write out status messages
         tvGroups.Nodes.Add barrecord.Mark, tvwChild, barrecord.Mark + "StatusMsg", "Status Messages"
         Dim msg As StatusMessage
         For Each msg In barrecord.PrimaryBend.StatusMessages
            tvGroups.Nodes.Add barrecord.Mark + "StatusMsg", tvwChild, , msg.Text
         Next
         If (barrecord.Varies) Then
            For Each msg In barrecord.VariesBend.StatusMessages
               tvGroups.Nodes.Add barrecord.Mark + "StatusMsg", tvwChild, , msg.Text
            Next
         End If


      Next

      tvGroups.Nodes.Add group.Name, tvwChild, , "Superstructure Mass = " & group.SuperstructureMass
      tvGroups.Nodes.Add group.Name, tvwChild, , "Superstructure Mass w/ Epoxy = " & group.SuperstructureMassEpoxy
      tvGroups.Nodes.Add group.Name, tvwChild, , "Substructure Mass = " & group.SubstructureMass
      tvGroups.Nodes.Add group.Name, tvwChild, , "Substructure Mass w/ Epoxy = " & group.SubstructureMassEpoxy
      tvGroups.Nodes.Add group.Name, tvwChild, , "Status = " & group.Status
   Next
   
   tvGroups.Nodes.Add , , , "Superstructure Mass = " & barlist.SuperstructureMass
   tvGroups.Nodes.Add , , , "Superstructure Mass w/ Epoxy = " & barlist.SuperstructureMassEpoxy
   tvGroups.Nodes.Add , , , "Substructure Mass = " & barlist.SubstructureMass
   tvGroups.Nodes.Add , , , "Substructure Mass w/ Epoxy = " & barlist.SubstructureMassEpoxy
   tvGroups.Nodes.Add , , , "Status = " & barlist.Status
   
End Sub

Private Sub Form_Load()
   If (barlist Is Nothing) Then
      MsgBox "Barlist is nothing"
   End If
   InitBarlistTree
End Sub
