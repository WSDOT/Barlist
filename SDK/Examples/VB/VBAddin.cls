VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Addin"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Implements IBarlistAddin

Private Sub IBarlistAddin_Go(ByVal pBarlist As BARSLib.IBarlist)
   Set frmBarlist.barlist = pBarlist
   Load frmBarlist
   frmBarlist.Show Modal
   Set frmBarlist.barlist = Nothing
End Sub

Private Property Get IBarlistAddin_MenuItem() As String
   IBarlistAddin_MenuItem = "VB Addin 2"
End Property
