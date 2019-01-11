VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Barlist Tester"
   ClientHeight    =   8616
   ClientLeft      =   1920
   ClientTop       =   1320
   ClientWidth     =   7560
   ClipControls    =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8616
   ScaleWidth      =   7560
   Begin VB.CommandButton cmdBarMoveUp 
      Caption         =   "^"
      Height          =   252
      Left            =   2760
      TabIndex        =   11
      Top             =   8280
      Width           =   252
   End
   Begin VB.CommandButton cmdBarMoveDown 
      Caption         =   "V"
      Height          =   252
      Left            =   3120
      TabIndex        =   10
      Top             =   8280
      Width           =   252
   End
   Begin VB.CommandButton cmdGroupMoveDown 
      Caption         =   "V"
      Height          =   252
      Left            =   1560
      TabIndex        =   7
      Top             =   8280
      Width           =   252
   End
   Begin VB.CommandButton cmdGroupMoveUp 
      Caption         =   "^"
      Height          =   252
      Left            =   1200
      TabIndex        =   6
      Top             =   8280
      Width           =   252
   End
   Begin VB.CommandButton cmdLoadFromFile 
      Caption         =   "Load from File"
      Height          =   252
      Left            =   6000
      TabIndex        =   5
      Top             =   7920
      Width           =   1452
   End
   Begin VB.OptionButton rbUS 
      Caption         =   "US Units"
      Height          =   252
      Left            =   4920
      TabIndex        =   4
      Top             =   7920
      Value           =   -1  'True
      Width           =   1212
   End
   Begin VB.OptionButton rbSI 
      Caption         =   "SI Units"
      Height          =   252
      Left            =   3840
      TabIndex        =   3
      Top             =   7920
      Width           =   1092
   End
   Begin VB.CommandButton cmdChangeBar 
      Caption         =   "Change A Bar - Fires an Event"
      Height          =   252
      Left            =   240
      TabIndex        =   2
      Top             =   7920
      Width           =   2412
   End
   Begin MSComctlLib.TreeView tvTree 
      Height          =   3735
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   7335
      _ExtentX        =   12933
      _ExtentY        =   6583
      _Version        =   393217
      HideSelection   =   0   'False
      Indentation     =   176
      LineStyle       =   1
      Style           =   6
      HotTracking     =   -1  'True
      SingleSel       =   -1  'True
      BorderStyle     =   1
      Appearance      =   1
   End
   Begin MSComctlLib.TreeView tvGroups 
      Height          =   3972
      Left            =   120
      TabIndex        =   1
      Top             =   3840
      Width           =   7332
      _ExtentX        =   12933
      _ExtentY        =   7006
      _Version        =   393217
      Indentation     =   176
      LineStyle       =   1
      Style           =   7
      BorderStyle     =   1
      Appearance      =   1
   End
   Begin VB.Label Label1 
      Caption         =   "Bars"
      Height          =   252
      Left            =   2040
      TabIndex        =   9
      Top             =   8280
      Width           =   852
   End
   Begin VB.Label Label2 
      Caption         =   "Groups"
      Height          =   252
      Left            =   360
      TabIndex        =   8
      Top             =   8280
      Width           =   732
   End
   Begin VB.Menu mnuAddins 
      Caption         =   "&Add Ins"
      Begin VB.Menu mnuPlaceholder 
         Caption         =   "Placeholder0"
         Index           =   0
      End
      Begin VB.Menu mnuPlaceholder 
         Caption         =   "Placeholder1"
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuPlaceholder 
         Caption         =   "Placeholder2"
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuPlaceholder 
         Caption         =   "Placeholder3"
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuPlaceholder 
         Caption         =   "Placeholder4"
         Index           =   4
         Visible         =   0   'False
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Global
Dim unitsvr As New UnitServer 'Provides unit conversion facilities
Dim dispUnitsMgr As New DisplayUnitMgr  'Manages display unit objects

'Private
Private barInfoMgr As New barInfoMgr 'Contains specifications for rebar
                                     'including bar weight per length,
                                     'diameter, maximum length, and
                                     'bending information such as bend radius
                                     'and tail lengths of hooks
Private WithEvents barlist As barlist 'Manages the barlist data
Attribute barlist.VB_VarHelpID = -1
Private addinMgr As New addinMgr 'This is the addin manager


'Configures the Addin menu
Private Sub ConfigureAddinMenu()
   If addinMgr.count = 0 Then
      mnuAddins.Visible = False
      Exit Sub
   End If
   
   Dim max_addins As Long
   max_addins = 5
   Dim count As Long
   count = 0
   Dim temp 'Temporary object
   For Each temp In addinMgr
      Dim addin As IBarlistAddin
      Set addin = temp
      If (count < max_addins) Then
         mnuPlaceholder(count).Caption = addin.MenuItem
         mnuPlaceholder(count).Visible = True
      End If
      
      count = count + 1
      Set addin = Nothing
   Next
   
End Sub

'Configures the display units manager by associating it with our
'units server and creating indirect measures using DisplayUnits objects
Private Sub ConfigureDisplayUnits()
   'Associate a units server with the display units manager
   Dim svr As ISupportUnitServer
   Set svr = dispUnitsMgr
   svr.UnitServer = unitsvr
   Set svr = Nothing
   
   'Add indirect units of measure to the display units manager
   
   'Length Units for bend dimensions (U, W, X, Y, etc)
   dispUnitsMgr.CreateDisplayUnit "Dim", unitsvr.UnitTypes("Length").Units("m"), 3
   Dim du As New FeetInchDisplayUnit
   du.Accuracy = 1
   dispUnitsMgr.AddDisplayUnit "Dim", du
   Set du = Nothing
   
   'Length Units for Computed Length of the Bar
   dispUnitsMgr.CreateDisplayUnit "Length", unitsvr.UnitTypes("Length").Units("m"), 3
   Set du = New FeetInchDisplayUnit
   du.Accuracy = 0
   dispUnitsMgr.AddDisplayUnit "Length", du
   Set du = Nothing
   
   'Mass
   dispUnitsMgr.CreateDisplayUnit "Mass", unitsvr.UnitTypes("Mass").Units("kg"), 0
   dispUnitsMgr.CreateDisplayUnit "Mass", unitsvr.UnitTypes("Mass").Units("lb"), 0
   
   'Angle (Used for type 79, 80, 81, 82, and 84 bends
   dispUnitsMgr.CreateDisplayUnit "Angle", unitsvr.UnitTypes("Angle").Units("deg"), 0
   
   'Mass Per Length Units
   '(Used only for displaying mass property of BarData object - Not needed in barlist program)
   dispUnitsMgr.CreateDisplayUnit "MPL", unitsvr.UnitTypes("MassPerLength").Units("kg/m"), 0
   dispUnitsMgr.CreateDisplayUnit "MPL", unitsvr.UnitTypes("MassPerLength").Units("lb/ft"), 0
End Sub

'Takes a value and a DisplayUnits object and creates an output string.
'The value is in base units and is converted to display units.
'The unit tag is appended to the string
Private Function FormatUnitValue(val As Double, dispUnit As DisplayUnit) As String
   Dim text As String
   text = dispUnit.Format(val)
   FormatUnitValue = text
End Function

'Taks a StatusMessage object and formats it into a text string
Private Function FormatStatusMessage(msg As StatusMessage) As String
   Dim text As String
   text = msg.text
   
   Dim v1 As String
   Dim v2 As String
   
   If (VarType(msg.Val1) = vbDouble) Then
      v1 = FormatUnitValue(msg.Val1, dispUnitsMgr.DisplayUnit("Length"))
   Else
      v1 = msg.Val1
   End If
   
   If (VarType(msg.Val2) = vbDouble) Then
      v2 = FormatUnitValue(msg.Val2, dispUnitsMgr.DisplayUnit("Length"))
   Else
      v2 = msg.Val2
   End If
   
   text = Replace(text, "%1", v1, , , vbTextCompare)
   text = Replace(text, "%2", v2, , , vbTextCompare)
     
   FormatStatusMessage = text
End Function

'Configures the display units manage for a particual set of system units
Private Sub SetDisplayUnitsMode(um As UnitSystemType)
   Dim su As ISupportUnitModes 'Interface pointer
   Set su = dispUnitsMgr 'Get ISupportUnitModes interface from dispUnitsMgr
   su.UnitMode = um 'Set the display units mode
End Sub

Private Sub ClearBarInfoTree()
   tvTree.Nodes.Clear
End Sub

Private Sub InitBarInfoTree()
   'Create root node
   tvTree.Nodes.Add , , "Root", barInfoMgr.Specification
   
   'Create collection of bars
   Dim bars As BarCollection
   'Make these bars belong to the barInfoMgr
   Set bars = barInfoMgr.bars
   Dim bar As bardata
   'For each bardata available, create a child-node and then more
   '  child nodes beneath that displaying the bar data items
   For Each bar In bars
      'Note: key is set to bar.name or (bar.name + title)
      tvTree.Nodes.Add "Root", tvwChild, bar.name, bar.name
      tvTree.Nodes.Add bar.name, tvwChild, bar.name + "Bends", "Bends"
      tvTree.Nodes.Add bar.name, tvwChild, bar.name + "Hooks", "Hooks"
      
      'Note: no key is set for these:
      'Note: these have no children and are children of the
      '  bar.name node, and display all their data
      tvTree.Nodes.Add bar.name, tvwChild, , "db = " + FormatUnitValue(bar.Diameter, dispUnitsMgr.DisplayUnit("Dim"))
      tvTree.Nodes.Add bar.name, tvwChild, , "Mass Per Length = " + FormatUnitValue(bar.Mass, dispUnitsMgr.DisplayUnit("MPL"))
      tvTree.Nodes.Add bar.name, tvwChild, , "Length = " + FormatUnitValue(bar.NormalLength, dispUnitsMgr.DisplayUnit("Dim"))
      tvTree.Nodes.Add bar.name, tvwChild, , "Max Length = " + FormatUnitValue(bar.MaxLength, dispUnitsMgr.DisplayUnit("Dim"))
      
      'Create 3 "use" objects, give each a "type", name, child-
      '  nodes, and populate with data
      For i = 1 To 3
      
      Dim name As String
      Dim use As UseType
      
      Select Case i
      Case 3
         use = utLongitudinal
         name = "Longitudinal"
         
      Case 2
         use = utTransverse
         name = "Transverse"
         
      Case 1
         use = utSeismic
         name = "Seismic"
         
      End Select
      
      'Instantiate some benddata objects and set data to
      '  appropriate types via a function call which returns the
      '  correct data for each type of "use" object.
      Dim bend As BendData
      Set bend = bar.BendData(use)
      
      Dim key As String
      key = bar.name + name
      If Not (bend Is Nothing) Then
         'Create a node for each "bend" and create child nodes
         '  for these w/data from the use objects
         tvTree.Nodes.Add bar.name + "Bends", tvwChild, key, name
         tvTree.Nodes.Add key, tvwChild, , "Use = " & bend.use
         tvTree.Nodes.Add key, tvwChild, , "ID/db = " & bend.InsideDiameter
      End If
      
      'For each "hook", create a node, and then cycle three time,
      '  creating childnodes, each populated with "use" object
      '  nodes, and create further childnodes of these and
      '  populate them with use object data for that type.
      tvTree.Nodes.Add bar.name + "Hooks", tvwChild, bar.name + name + "Hooks", name
      For j = 1 To 3
      Dim hookType As hookType
      Dim hookName As String

      Select Case j

      Case 1
         'Note: the choices of hooktypes are limited to
         '  "ht" + the type number
         hookType = ht90
         hookName = "90"
      Case 2
         hookType = ht135
         hookName = "135"
      Case 3
         hookType = ht180
         hookName = "180"
      End Select

      Dim hookKey As String
      hookKey = bar.name + name + hookName

      Dim hook As HookData
      Set hook = bar.HookData(use, hookType)
      If Not (hook Is Nothing) Then
         tvTree.Nodes.Add bar.name + name + "Hooks", tvwChild, hookKey, hookName
         tvTree.Nodes.Add hookKey, tvwChild, , "Type = " & hook.Type
         tvTree.Nodes.Add hookKey, tvwChild, , "Use = " & hook.use
         tvTree.Nodes.Add hookKey, tvwChild, , "T/db = " & hook.Tail
         tvTree.Nodes.Add hookKey, tvwChild, , "Min Tail = " + FormatUnitValue(hook.MinTail, dispUnitsMgr.DisplayUnit("Dim"))
     End If


      Next j
      
      Next i
      
   Next
   
End Sub

Private Sub InitBarlist()
   
   Dim group As group
   
   Dim bardata As bardata
   Set bardata = barInfoMgr.bars("#6")
   
   'Create a group titled "Pier 1"
   barlist.Groups.Add "Pier 1"
   'Get a handle to it so it's easier to handle (a pun!)
   Set group = barlist.Groups("Pier 1")
   'Create a new BarRecord and populate, BUT NOT associated w/any
   '  Group (yet?)
   Dim barrecord As New barrecord
   barrecord.mark = "M123"
   barrecord.Location = "Top Bar"
   barrecord.NumReqd = 10
   barrecord.Epoxy = False
   barrecord.LumpSum = False
   barrecord.Substructure = False
   barrecord.use = utLongitudinal
   barrecord.bardata = bardata
   
   Dim type51 As New type51
   type51.U = 10
   barrecord.PrimaryBend = type51
   Set type51 = Nothing
   
   group.BarRecords.Add barrecord
   Set barrecord = Nothing
   
   Set barrecord = New barrecord
   barrecord.mark = "M234"
   barrecord.Location = "Bottom Bar"
   barrecord.NumReqd = 10
   barrecord.Epoxy = False
   barrecord.LumpSum = False
   barrecord.Substructure = False
   barrecord.use = utLongitudinal
   barrecord.bardata = bardata
   
   'Create new bend type, set the data, and and assign to
   '  a bar record
   Dim type50 As New type50
   type50.U = 30
   barrecord.PrimaryBend = type50
   Set type50 = Nothing
   
   'Add the barrecord to the group, and then release
   group.BarRecords.Add barrecord
   Set barrecord = Nothing
   
   Set barrecord = New barrecord
   barrecord.mark = "M456"
   barrecord.Location = "Bottom Bar"
   barrecord.NumReqd = 10
   barrecord.Epoxy = False
   barrecord.LumpSum = False
   barrecord.Substructure = False
   barrecord.use = utLongitudinal
   barrecord.bardata = bardata
   
   'Create new bend type, set the data, and and assign to
   '  a bar record
   Set type50 = New type50
   type50.U = 30
   barrecord.PrimaryBend = type50
   Set type50 = Nothing
   
   'Add the barrecord to the group, and then release
   group.BarRecords.Add barrecord
   Set barrecord = Nothing
   
   'Release other data
   Set group = Nothing
   Set bardata = Nothing
   
   barlist.Groups.Add "Pier 2"
   barlist.Groups.Add "Pier 3"
   barlist.Groups.Add "Pier 4"
   barlist.Groups.Add "Pier 5"
End Sub

Private Function GetUseString(use As UseType) As String
  Dim name As String
  
  Select Case use
     Case utLongitudinal
     name = "Longitudinal"
     
     Case utTransverse
     name = "Transverse"
     
     Case utSeismic
     name = "Seismic"
  End Select
  
  GetUseString = name
End Function

Private Function GetStatusString(status As StatusType) As String
   Dim name As String
   
   Select Case status
      Case stOK
         name = "OK"
      Case stWarning
         name = "Warning"
      Case stError
         name = "Error"
   End Select
   GetStatusString = name
End Function

Private Sub ClearBarlistTree()
   tvGroups.Nodes.Clear
End Sub

Private Sub InitBarlistTree()
   tvGroups.Nodes.Clear
   
   Dim group As group
   For Each group In barlist.Groups
      tvGroups.Nodes.Add , , group.name, group.name
      
      
      Dim barrecord As barrecord
      For Each barrecord In group.BarRecords
         tvGroups.Nodes.Add group.name, tvwChild, "m" & barrecord.mark, "Mark = " & barrecord.mark
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Location = " & barrecord.Location
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Size = " & barrecord.Size
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "No. Reqd. = " & barrecord.NumReqd
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Bend Type = " & barrecord.bendType
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Use = " & GetUseString(barrecord.use)
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Lump Sump = " & barrecord.LumpSum
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Substructure = " & barrecord.Substructure
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Epoxy = " & barrecord.Epoxy
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Varies = " & barrecord.Varies
         
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, barrecord.mark + "Dim", "Dimensions"
         tvGroups.Nodes.Add barrecord.mark + "Dim", tvwChild, , "U = " + FormatUnitValue(barrecord.PrimaryBend.U, dispUnitsMgr.DisplayUnit("Dim"))
         tvGroups.Nodes.Add barrecord.mark + "Dim", tvwChild, , "W = " + FormatUnitValue(barrecord.PrimaryBend.W, dispUnitsMgr.DisplayUnit("Dim"))
         tvGroups.Nodes.Add barrecord.mark + "Dim", tvwChild, , "X = " + FormatUnitValue(barrecord.PrimaryBend.X, dispUnitsMgr.DisplayUnit("Dim"))
         tvGroups.Nodes.Add barrecord.mark + "Dim", tvwChild, , "Y = " + FormatUnitValue(barrecord.PrimaryBend.Y, dispUnitsMgr.DisplayUnit("Dim"))
         tvGroups.Nodes.Add barrecord.mark + "Dim", tvwChild, , "Z = " + FormatUnitValue(barrecord.PrimaryBend.Z, dispUnitsMgr.DisplayUnit("Dim"))
         tvGroups.Nodes.Add barrecord.mark + "Dim", tvwChild, , "T1 = " + FormatUnitValue(barrecord.PrimaryBend.T1, dispUnitsMgr.DisplayUnit("Dim"))
         tvGroups.Nodes.Add barrecord.mark + "Dim", tvwChild, , "T2 = " + FormatUnitValue(barrecord.PrimaryBend.T2, dispUnitsMgr.DisplayUnit("Dim"))
         
         If (barrecord.Varies) Then
            tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, barrecord.mark + "VDim", "Varies Dimensions"
            tvGroups.Nodes.Add barrecord.mark + "VDim", tvwChild, , "U = " + FormatUnitValue(barrecord.VariesBend.U, dispUnitsMgr.DisplayUnit("Dim"))
            tvGroups.Nodes.Add barrecord.mark + "VDim", tvwChild, , "W = " + FormatUnitValue(barrecord.VariesBend.W, dispUnitsMgr.DisplayUnit("Dim"))
            tvGroups.Nodes.Add barrecord.mark + "VDim", tvwChild, , "X = " + FormatUnitValue(barrecord.VariesBend.X, dispUnitsMgr.DisplayUnit("Dim"))
            tvGroups.Nodes.Add barrecord.mark + "VDim", tvwChild, , "Y = " + FormatUnitValue(barrecord.VariesBend.Y, dispUnitsMgr.DisplayUnit("Dim"))
            tvGroups.Nodes.Add barrecord.mark + "VDim", tvwChild, , "Z = " + FormatUnitValue(barrecord.VariesBend.Z, dispUnitsMgr.DisplayUnit("Dim"))
            tvGroups.Nodes.Add barrecord.mark + "VDim", tvwChild, , "T1 = " + FormatUnitValue(barrecord.VariesBend.T1, dispUnitsMgr.DisplayUnit("Dim"))
            tvGroups.Nodes.Add barrecord.mark + "VDim", tvwChild, , "T2 = " + FormatUnitValue(barrecord.VariesBend.T2, dispUnitsMgr.DisplayUnit("Dim"))
         End If
         
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Length = " + FormatUnitValue(barrecord.PrimaryBend.Length, dispUnitsMgr.DisplayUnit("Length"))
         If (barrecord.Varies) Then
            tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Varies Length = " + FormatUnitValue(barrecord.VariesBend.Length, dispUnitsMgr.DisplayUnit("Length"))
         End If
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Mass = " + FormatUnitValue(barrecord.Mass, dispUnitsMgr.DisplayUnit("Mass"))
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, , "Status = " & GetStatusString(barrecord.status)
         
          'Write out status messages
         tvGroups.Nodes.Add "m" & barrecord.mark, tvwChild, barrecord.mark + "StatusMsg", "Status Messages"
         Dim msg As StatusMessage
         For Each msg In barrecord.PrimaryBend.StatusMessages
            tvGroups.Nodes.Add barrecord.mark + "StatusMsg", tvwChild, , FormatStatusMessage(msg)
         Next
         If (barrecord.Varies) Then
            For Each msg In barrecord.VariesBend.StatusMessages
               tvGroups.Nodes.Add barrecord.mark + "StatusMsg", tvwChild, , FormatStatusMessage(msg)
            Next
         End If
         
         
      Next
         
      tvGroups.Nodes.Add group.name, tvwChild, , "Superstructure Mass = " + FormatUnitValue(group.SuperstructureMass, dispUnitsMgr.DisplayUnit("Mass"))
      tvGroups.Nodes.Add group.name, tvwChild, , "Superstructure Mass w/ Epoxy = " + FormatUnitValue(group.SuperstructureMassEpoxy, dispUnitsMgr.DisplayUnit("Mass"))
      tvGroups.Nodes.Add group.name, tvwChild, , "Substructure Mass = " + FormatUnitValue(group.SubstructureMass, dispUnitsMgr.DisplayUnit("Mass"))
      tvGroups.Nodes.Add group.name, tvwChild, , "Substructure Mass w/ Epoxy = " + FormatUnitValue(group.SubstructureMassEpoxy, dispUnitsMgr.DisplayUnit("Mass"))
      tvGroups.Nodes.Add group.name, tvwChild, , "Status = " & GetStatusString(group.status)
   Next
   
   tvGroups.Nodes.Add , , , "Superstructure Mass = " + FormatUnitValue(barlist.SuperstructureMass, dispUnitsMgr.DisplayUnit("Mass"))
   tvGroups.Nodes.Add , , , "Superstructure Mass w/ Epoxy = " + FormatUnitValue(barlist.SuperstructureMassEpoxy, dispUnitsMgr.DisplayUnit("Mass"))
   tvGroups.Nodes.Add , , , "Substructure Mass = " + FormatUnitValue(barlist.SubstructureMass, dispUnitsMgr.DisplayUnit("Mass"))
   tvGroups.Nodes.Add , , , "Substructure Mass w/ Epoxy = " + FormatUnitValue(barlist.SubstructureMassEpoxy, dispUnitsMgr.DisplayUnit("Mass"))
   tvGroups.Nodes.Add , , , "Status = " & GetStatusString(barlist.status)
   
End Sub

Private Sub barlist_OnBarRecordAdded(ByVal pGroup As BARSLib.IGroup, ByVal pNewRecord As BARSLib.IBarRecord)
'For some reason, I have to have this method or BARSLib crashes
End Sub

Private Sub barlist_OnBarRecordChanged(ByVal pGroup As BARSLib.IGroup, ByVal pBarRecord As BARSLib.IBarRecord)
   MsgBox "Event Received - Bar " & pBarRecord.mark & " of Group " & pGroup.name & " changed"
   ClearBarlistTree 'Clear the old data out of the tree
   InitBarlistTree  'Rebuild the tree
End Sub

Private Sub barlist_OnBarRecordMoved(ByVal pGroup As BARSLib.IGroup, ByVal pBarRecord As BARSLib.IBarRecord, ByVal idxFrom As Long, ByVal idxTo As Long)
   Dim name As String
   name = pGroup.name
   
   Dim mark As String
   mark = pBarRecord.mark
   MsgBox "Mark " & mark & " of Group " & name & " moved from index " & idxFrom & " to " & idxTo
   
   ClearBarlistTree 'Clear the old data out of the tree
   InitBarlistTree  'Rebuild the tree
End Sub

Private Sub barlist_OnBarRecordRemoved(ByVal pGroup As BARSLib.IGroup, ByVal bstrMark As String)
'For some reason, I have to have this method or BARSLib crashes
End Sub

Private Sub barlist_OnGroupAdded(ByVal pGroup As BARSLib.IGroup)
'For some reason, I have to have this method or BARSLib crashes
End Sub

Private Sub barlist_OnGroupChanged(ByVal pGroup As BARSLib.IGroup)
'For some reason, I have to have this method or BARSLib crashes
End Sub

Private Sub barlist_OnGroupMoved(ByVal pGroup As BARSLib.IGroup, ByVal idxFrom As Long, ByVal idxTo As Long)
   Dim name As String
   name = pGroup.name
   MsgBox "Group " & name & " moved from index " & idxFrom & " to " & idxTo
   
   ClearBarlistTree
   InitBarlistTree
End Sub

Private Sub barlist_OnGroupRemoved(ByVal name As String)
'For some reason, I have to have this method or BARSLib crashes
End Sub

Private Sub barlist_OnNotIncludedQuantitiesChanged()
'For some reason, I have to have this method or BARSLib crashes
End Sub

Private Sub cmdBarMoveDown_Click()
   barlist.Groups("Pier 1").BarRecords.MoveDown "M234"
End Sub

Private Sub cmdBarMoveUp_Click()
   barlist.Groups("Pier 1").BarRecords.MoveUp "M234"
End Sub

Private Sub cmdChangeBar_Click()
   barlist.Groups(0).BarRecords(0).PrimaryBend.U = 15
   
   Dim bardata As bardata
   Set bardata = barInfoMgr.bars("#6")
   
   'Attempt a replace operation
   Dim barrecord As New barrecord
   barrecord.mark = "R123"
   barrecord.Location = "Top Bar"
   barrecord.NumReqd = 9
   barrecord.Epoxy = False
   barrecord.LumpSum = False
   barrecord.Substructure = False
   barrecord.use = utLongitudinal
   barrecord.bardata = bardata
   
   Dim type51 As New type51
   type51.U = 10
   barrecord.PrimaryBend = type51
   Set type51 = Nothing
   
   barlist.Groups(0).BarRecords.Replace 1, barrecord
   
   Set barrecord = Nothing
   Set bardata = Nothing
   
End Sub

Private Sub cmdLoadFromFile_Click()
   Dim parser As New BarlistParser
   Set barlist = Nothing
   Set barlist = parser.ReadBarlistFile(barInfoMgr, "c:\test.bar")
   If barlist Is Nothing Then
      MsgBox parser.GetLastError
   End If
   Set parser = Nothing
   
   ClearBarlistTree
   InitBarlistTree
End Sub

Private Sub cmdGroupMoveDown_Click()
   barlist.Groups.MoveDown "Pier 2"
End Sub

Private Sub cmdGroupMoveUp_Click()
   barlist.Groups.MoveUp "Pier 2"
End Sub

Private Sub Form_Load()
   'Configure the addin menu
   ConfigureAddinMenu
   
   'Set up display units
   ConfigureDisplayUnits
   SetDisplayUnitsMode unitsUS
   
   Set barlist = New barlist 'Have to do New here because of WithEvents keyword in declaration
   
   rbUS.value = True 'Check the US Units radio button
   
   InitBarInfoTree
   InitBarlist
   InitBarlistTree
End Sub


Private Sub Form_Unload(Cancel As Integer)
'   Dim parser As New BarlistParser
'   parser.WriteBarlistFile barlist, "C:\test2.bar"
'   Set parser = Nothing
   
   Set barInfoMgr = Nothing
   Set barlist = Nothing
   Set unitsvr = Nothing
   Set dispUnitsMgr = Nothing
   Set addinMgr = Nothing
End Sub

Private Sub mnuPlaceholder_Click(Index As Integer)
   Dim addin As IBarlistAddin
   Set addin = addinMgr.Item(Index)
   addin.Go barlist
   Set addin = Nothing
End Sub

Private Sub rbSI_Click()
   SetDisplayUnitsMode unitsSI
   ClearBarInfoTree
   ClearBarlistTree
   
   InitBarInfoTree
   InitBarlistTree
End Sub

Private Sub rbUS_Click()
   SetDisplayUnitsMode unitsUS
   ClearBarInfoTree
   ClearBarlistTree
   
   InitBarInfoTree
   InitBarlistTree
End Sub
