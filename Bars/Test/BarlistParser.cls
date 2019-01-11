VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "BarlistParser"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Description = "Read/Writes barlist files"
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Private lastError As String

Public Function GetLastError() As String
Attribute GetLastError.VB_Description = "Returns the last error that the parser generated."
   GetLastError = lastError
End Function

Public Function WriteBarlistFile(barlist As IBarlist, fname As String) As Boolean
Attribute WriteBarlistFile.VB_Description = "Writes a barlist to the specified file.  Returns True if successful.  Call GetLastError if return value is False"
   
   Dim doc As New MSXML.DOMDocument
   Dim xmlblist As IXMLDOMNode
   Set xmlblist = doc.createNode(NODE_ELEMENT, "Barlist", "")
   doc.appendChild xmlblist
   
   SaveGroups barlist, xmlblist, doc
   doc.save fname
   Set doc = Nothing
   
   lastError = "No Error"

End Function

'Loads the barlist file from disk and creates a barlist object
'
'NOTE: When the bend factory exists, it will have to be passed to
'      this method.
Public Function ReadBarlistFile(barInfoMgr As barInfoMgr, fname As String) As barlist
Attribute ReadBarlistFile.VB_Description = "Reads a barlist from a file. If successful a new barlist object is return,  otherwise call GetLastError to get the error message."
   
   'NOTE: This method assumes version 4 format.  Robert, you will
   '      have to enhance this method to detect the various file
   '      formats and then read each one accordingly
   Dim doc As New MSXML.DOMDocument
   If (Not doc.Load(fname)) Then
      lastError = "Error loading " & fname
      Set ReadBarlistFile = Nothing
      Exit Function
   End If
   
   Dim barlist As New barlist
   ReadGroups barInfoMgr, barlist, doc.documentElement.childNodes
   Set ReadBarlistFile = barlist
   Set barlist = Nothing
   Set doc = Nothing
   
   lastError = "No Error"
   
End Function

'Reads barlist groups
Private Sub ReadGroups(barInfoMgr As barInfoMgr, blist As barlist, xmlgroups As IXMLDOMNodeList)
   Dim xmlgroup As IXMLDOMNode
   For Each xmlgroup In xmlgroups
      Dim xmlgroupelement As IXMLDOMNode
      For Each xmlgroupelement In xmlgroup.childNodes
      
         If xmlgroupelement.nodeName = "Name" Then
            blist.Groups.Add xmlgroupelement.nodeTypedValue
         Else
            ReadBarRecords barInfoMgr, blist.Groups(blist.Groups.count - 1), xmlgroupelement
         End If
         
      Next
   Next
End Sub

'Dummy method to simulate bend factory for reading bar records
'Remove this method when bend factory exists!!!
Private Function CreateBend(bendType As Long) As IBend
   'Don't use a Select/Case here!!!
   Set CreateBend = New type50
End Function

'Reads bar records
Private Sub ReadBarRecords(barInfoMgr As barInfoMgr, group As group, xmlbarrecord As IXMLDOMNode)
   Dim barrecord As New barrecord
   
   barrecord.Mark = xmlbarrecord.selectSingleNode("Mark").nodeTypedValue
   barrecord.Location = xmlbarrecord.selectSingleNode("Location").nodeTypedValue
   barrecord.NumReqd = xmlbarrecord.selectSingleNode("NoReqd").nodeTypedValue
   barrecord.use = xmlbarrecord.selectSingleNode("Use").nodeTypedValue
   barrecord.LumpSum = xmlbarrecord.selectSingleNode("LumpSum").nodeTypedValue
   barrecord.Epoxy = xmlbarrecord.selectSingleNode("Epoxy").nodeTypedValue
   barrecord.Substructure = xmlbarrecord.selectSingleNode("Substructure").nodeTypedValue
   
   barrecord.bardata = barInfoMgr.bars(xmlbarrecord.selectSingleNode("Size").nodeTypedValue)
   
   'NOTE: Use bend factory object instead of CreateBend call on the next line
   barrecord.PrimaryBend = CreateBend(xmlbarrecord.selectSingleNode("BendType").nodeTypedValue)
   ReadBendDimensions barrecord.PrimaryBend, xmlbarrecord.selectSingleNode("PrimaryBend")
   
   Dim bVaries As Boolean
   bVaries = xmlbarrecord.selectSingleNode("Varies").nodeTypedValue
   If (bVaries) Then
      'NOTE: Use bend factory object instead of CreateBend call on the next line
      barrecord.VariesBend = CreateBend(xmlbarrecord.selectSingleNode("BendType").nodeTypedValue)
      ReadBendDimensions barrecord.VariesBend, xmlbarrecord.selectSingleNode("VariesBend")
   End If
   
   group.BarRecords.Add barrecord
   
   Set barrecord = Nothing
End Sub

'Reads bend dimensions
Private Sub ReadBendDimensions(bend As IBend, xmlbend As IXMLDOMNode)
   bend.U = xmlbend.selectSingleNode("U").nodeTypedValue
   bend.W = xmlbend.selectSingleNode("W").nodeTypedValue
   bend.X = xmlbend.selectSingleNode("X").nodeTypedValue
   bend.Y = xmlbend.selectSingleNode("Y").nodeTypedValue
   bend.Z = xmlbend.selectSingleNode("Z").nodeTypedValue
   bend.T1 = xmlbend.selectSingleNode("T1").nodeTypedValue
   bend.T2 = xmlbend.selectSingleNode("T2").nodeTypedValue
End Sub

'Saves the groups of a barlist
Private Sub SaveGroups(blist As barlist, xmlblist As IXMLDOMNode, doc As DOMDocument)
  Dim group As group
  For Each group In blist.Groups
     Dim xmlgroup As IXMLDOMNode
     Set xmlgroup = doc.createNode(NODE_ELEMENT, "Group", "")
     xmlblist.appendChild xmlgroup
     
     Dim xmlname As IXMLDOMNode
     Set xmlname = doc.createNode(NODE_ELEMENT, "Name", "")
     xmlname.text = group.Name
     xmlgroup.appendChild xmlname
     
     Dim barrecord As barrecord
     For Each barrecord In group.BarRecords
        Dim xmlbarrecord As IXMLDOMNode
        Set xmlbarrecord = doc.createNode(NODE_ELEMENT, "BarRecord", "")
        xmlgroup.appendChild xmlbarrecord
        SaveBarRecord barrecord, xmlbarrecord, doc
     Next
     
  Next
End Sub

'Saves a bar record
Private Sub SaveBarRecord(barrecord As barrecord, xmlbarrecord As IXMLDOMNode, doc As DOMDocument)
   Dim xmlnode As IXMLDOMNode 'general purpose node
   
   SaveElement "Mark", barrecord.Mark, xmlbarrecord, doc
   SaveElement "Location", barrecord.Location, xmlbarrecord, doc
   SaveElement "Size", barrecord.Size, xmlbarrecord, doc
   SaveElement "NoReqd", barrecord.NumReqd, xmlbarrecord, doc
   SaveElement "BendType", barrecord.bendType, xmlbarrecord, doc
   SaveElement "Use", barrecord.use, xmlbarrecord, doc
   SaveElement "LumpSum", barrecord.LumpSum, xmlbarrecord, doc
   SaveElement "Substructure", barrecord.Substructure, xmlbarrecord, doc
   SaveElement "Epoxy", barrecord.Epoxy, xmlbarrecord, doc
   SaveElement "Varies", barrecord.Varies, xmlbarrecord, doc
   SaveElement "NoEach", barrecord.NumEach, xmlbarrecord, doc
   
   SaveBend "PrimaryBend", barrecord.PrimaryBend, xmlbarrecord, doc
   
   If (barrecord.Varies) Then
      SaveBend "VariesBend", barrecord.VariesBend, xmlbarrecord, doc
   End If

End Sub

'Save a bend object
Private Sub SaveBend(element As String, bend As IBend, xmlbarrecord As IXMLDOMNode, doc As DOMDocument)
   Dim xmlbend As IXMLDOMNode
   Set xmlbend = doc.createNode(NODE_ELEMENT, element, "")
   xmlbarrecord.appendChild xmlbend
   
   SaveElement "U", bend.U, xmlbend, doc
   SaveElement "W", bend.W, xmlbend, doc
   SaveElement "X", bend.X, xmlbend, doc
   SaveElement "Y", bend.Y, xmlbend, doc
   SaveElement "Z", bend.Z, xmlbend, doc
   SaveElement "T1", bend.T1, xmlbend, doc
   SaveElement "T2", bend.T2, xmlbend, doc
End Sub

'Saves an element
Private Sub SaveElement(element As String, value As Variant, xmlparent As IXMLDOMNode, doc As DOMDocument)
   Dim xmlnode As IXMLDOMNode 'general purpose node
   Set xmlnode = doc.createNode(NODE_ELEMENT, element, "")
   xmlnode.text = value
   xmlparent.appendChild xmlnode
End Sub

