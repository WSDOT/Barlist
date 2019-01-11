VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "BXF"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
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

'A note to anyone who reads this and/or wants to change the code
'  in this class: DONT.  This was created this way (poorly) due to
'  lack of time.  It is a copy and paste from other places (most
'  notibly the "report" form in Barlist4.  I thought that since the
'  BXF is similiar to the report, and that most fields seemed
'  similiar to other fields, that I could group them and manipulate
'  them by passing members of groups into "group" functions.  This is
'  WRONG.  Almost every member of the BXF is an independent entity due
'  to spacing requirements, etc.  There are a couple exceptions, though
'  not noteable.  For anyone who is going to change this code:
'  toss all of this out, and do it again, manipulating each piece of
'  data independently.  There should be two functions for each piece of
'  data: one for manipulating in SI mode and one for manipulating in
'  US mode.  However, if the layout of the BXF does not need changing,
'  this code will work fine.

Private strBXFReport As String
'Private ReportText As String
Private unitSystem As New WBFLUnitServer.unitSystem 'Provides unit conversion facilities
Private dispUnitsMgr As WBFLUnitServer.DisplayUnitMgr 'Manages display unit objects
Private iUnitConvert2 As WBFLUnitServer.iUnitConvert2
Private blist As BARSLib.IBarlist
Implements IBarlistAddin

Private Sub Class_Initialize()
    Set iUnitConvert2 = unitSystem.UnitServer
    Set dispUnitsMgr = unitSystem.DisplayUnitMgr
    
    ConfigureDisplayUnits
End Sub

Private Sub Class_Terminate()
    Set iUnitConvert2 = Nothing
End Sub

Public Sub IBarlistAddin_Go(ByVal pBarlist As BARSLib.IBarlist)
    strBXFReport = ""
    bCancel = False
    
    Set blist = pBarlist
    
    Load bxffrmUnits
    bxffrmUnits.Show vbModal
    If bCancel = False Then
        If SIMode = True Then
            unitSystem.UnitModeController.unitMode = umSI
        Else
            unitSystem.UnitModeController.unitMode = umUS
        End If
        Process
    End If
    
    Set blist = Nothing
    Set pBarlist = Nothing
    Unload bxffrmUnits
End Sub
Public Sub Process()
    ReportGroups blist
    SaveBXF blist
    
End Sub

Public Property Get IBarlistAddin_MenuItem() As String
   IBarlistAddin_MenuItem = "Create Barlist &Exchange File (BXF)"
   
End Property
'Reports the groups of a barlist
Private Sub ReportGroups(blist As Barlist)
  Dim group As group
  Dim line As String
  Dim count As Integer

  For count = 0 To ((blist.Groups.count) - 1)
    line = "     " & UCase(blist.Groups(count).Name)
     AddLine line
     
     Dim barrecord As barrecord
     For Each barrecord In blist.Groups(count).BarRecords
        line = ReportBarRecord(barrecord)
        AddLine line
        
        If (barrecord.varies) Then
            ReportVaries barrecord.VariesBend, barrecord.mass, barrecord.lumpsum
        End If
        
        If (barrecord.Status = stError) Then
           ReportAllErrors barrecord
        End If
     Next
  Next count
  
End Sub

'Reports a bar record
Private Function ReportBarRecord(barrecord As barrecord) As String
    Dim BarString As String
    Dim line As String
    'Note: length wanted (numeric entry in function call) is length of total
    '  string PLUS the following space
    'Having the "fixlength" first, and then the actual data should align data right.
    line = FixMark(barrecord.Mark)
    line = line + FixLength(barrecord.Location, Len(barrecord.Location), 29)
    line = line + FixSize(barrecord.size)
    If barrecord.lumpsum = True Then
        line = line + "   $"
    Else
        Dim tmpNumReq As Integer
        tmpNumReq = Len(GetRidOfSpaces(Str(barrecord.NumReqd)))
        If tmpNumReq < 4 Then
            line = line + FixLength(" ", Len(Str(barrecord.NumReqd)), 4) + Mid$(Str(barrecord.NumReqd), 2)
        Else
            line = line + GetRidOfSpaces(Str(barrecord.NumReqd))
        End If
'        line = line + FixLength(" ", Len(Str(barrecord.NumReqd)), 4) + Mid$(Str(barrecord.NumReqd), 2)
    End If
    line = line + Str(barrecord.BendType) + " "

    
    line = line + ConvertUse(barrecord.Use)
    
    line = line + ConvertBool(barrecord.lumpsum, "L")
    line = line + ConvertBool(barrecord.Substructure, "S")
    line = line + ConvertBool(barrecord.Epoxy, "E")
    line = line + ConvertBool(barrecord.varies, "V")
    
    If ((barrecord.lumpsum = True) And (barrecord.varies = True)) Then
        line = line + " $ "
    Else
        line = line + FixNumReqd(barrecord.NumEach)
    End If
    
    line = ReportBend(barrecord.PrimaryBend, line, False, barrecord.lumpsum)    'line passed by reference & modified
    
    If barrecord.lumpsum = True Then
        If unitSystem.UnitModeController.unitMode = umUS Then
            line = line + "  $  $"
        Else
            line = line + "    $   "
        End If
    Else
        Dim length As String    'The " " is to keep this format (which is good) in sync
                                ' with the format of the "str" function (used earlier)
                                ' which leaves an xtra space at start of string
        length = FormatUnitValueWOTag(barrecord.PrimaryBend.length, "Dim")
        If unitSystem.UnitModeController.unitMode = umUS Then
            length = ConvertUSLengthField(length)
            line = line + length
        Else
            line = line + FixDim(length, Len(length), 7)
        End If
    End If

    If barrecord.varies = False Then
        Dim weight As String
        weight = FormatUnitValueWOTag(barrecord.mass, "Mass")
        If val(weight) = 0 Then
            line = line + FixLength(" ", Len(weight), 7) + " "
        Else
            line = line + FixLength(" ", Len(weight), 7) + weight
        End If
    End If
            
    ReportBarRecord = line

End Function
Private Function FixMark(text As String) As String
    Dim tmpString As String
    
    Select Case Len(text)
        Case 1
            '3 leading spaces
            tmpString = "   " + text + " "
        Case 2
            '2 leading spaces
            tmpString = "  " + text + " "
        Case 3
            '1 leading space
            tmpString = " " + text + " "
        Case 4
            'no leading spaces
            tmpString = text + " "
    End Select
    
    FixMark = tmpString

End Function
'Report a bend object
Private Function ReportBend(bend As IBend, line As String, varies As Boolean, lumpsum As Boolean) As String
    Dim mdu, mdw, mdx, mdy, mdz As Integer
    mdu = 0
    mdw = 1
    mdx = 2
    mdy = 3
    mdz = 4

    Dim count As Integer
    Dim tmpnumber As String
    If varies = True Then
        For count = 0 To 57
            line = line + " "
        Next count
    End If
    
    If (lumpsum = True) Then
        line = line + FillDollarSigns
    Else
        If unitSystem.UnitModeController.unitMode = umUS Then
            If (Not bend.BendType > 89) Then
                line = line + BendValueString(bend, mdu, 8, bend.U) + " "
            Else
                line = line + "         "
            End If
        Else
            If (Not bend.BendType > 89) Then
                line = line + BendValueString(bend, mdu, 9, bend.U)
            Else
                line = line + "         "
            End If
        End If

        If (unitSystem.UnitModeController.unitMode = umUS) And (bend.W = 0) Then
            line = line + BendValueString(bend, mdw, 8, bend.W)
        Else
            line = line + BendValueString(bend, mdw, 9, bend.W)
        End If
        line = line + BendValueString(bend, mdx, 8, bend.X)
        line = line + BendValueString(bend, mdy, 8, bend.Y)
        line = line + BendValueString(bend, mdz, 8, bend.Z)
        If unitSystem.UnitModeController.unitMode = umUS Then line = line + " "
        tmpnumber = unitSystem.DisplayUnitMgr.ConvertToDisplayUnits(bend.T1, "Angle")
        If val(tmpnumber) = 0 Then tmpnumber = ""
        line = line + FixLength("", Len(tmpnumber), 3) + Trim(tmpnumber) + " "
        tmpnumber = unitSystem.DisplayUnitMgr.ConvertToDisplayUnits(bend.T2, "Angle")
        If (val(tmpnumber) = 0) And (unitSystem.UnitModeController.unitMode = umSI) Then tmpnumber = ""
        If (val(tmpnumber) = 0) And (unitSystem.UnitModeController.unitMode = umUS) Then
            line = line + "     "
        Else
            line = line + FixLength("", Len(tmpnumber), 3) + Trim(tmpnumber) + " "
            If unitSystem.UnitModeController.unitMode = umUS Then line = line + " "
        End If
    End If

    ReportBend = line

End Function

Private Function BendValueString(bend As IBend, ByVal SupportThisData As Integer, _
                                    spaces As Integer, data As Double) As String
    Dim tmpNumberString As String
    Dim count As Integer
    Dim tmpString

    If bend.SupportsDimension(SupportThisData) And data <> 0 Then
        tmpNumberString = FormatUnitValueWOTag(data, "Dim")
        BendValueString = FixDim((tmpNumberString), Len(tmpNumberString), spaces)
    Else
        For count = 1 To spaces
            tmpString = tmpString + " "
        Next count
        BendValueString = tmpString
    End If
    
End Function


Private Function FixLength(text As String, truelength As Integer, wantlength As Integer) As String
    Dim count As Integer
    Dim amountToAdd As Integer
    Dim amountToSubtract
    
    amountToAdd = wantlength - truelength
    amountToSubtract = truelength - wantlength
    If amountToAdd > amountToSubtract Then
        For count = 1 To amountToAdd
            text = text + " "
        Next count
    Else
    
    End If
    
    FixLength = text

End Function

Private Function ConvertUse(Use As UseType) As String
    Dim UseString As String
    Select Case Use
        Case utLongitudinal
            UseString = "  "
        Case utTransverse
            UseString = "T "
        Case utSeismic
            UseString = "S " '"E" in old format, "S" in new format
    End Select
    ConvertUse = UseString

End Function

Private Function ConvertBool(ToConvert As Boolean, datatype As String) As String
    Dim convertedString As String
    Select Case ToConvert
        Case "True"
            convertedString = datatype + " "
        Case "False"
            convertedString = "  "
    End Select
    ConvertBool = convertedString

End Function

Private Sub ReportErrors(statusMsgs As BARSLib.StatusMessageCollection)
    Dim Message As String
    
    Dim statusMsg As BARSLib.StatusMessage
    For Each statusMsg In statusMsgs
       Message = FormatStatusMessage(statusMsg)
       If Not Mid$(Message, 1, 7) = "WARNING" Then
             AddLine Message
             Message = ""
       End If
    Next
    
End Sub

Private Function FixSize(size As String) As String
    Select Case (Len(size) - 1)
    'Reason for the Mid$ is because the data is in format: "#2", etc...
        Case 0
            FixSize = "   "
        Case 1
            FixSize = Mid$(size, 2, 1) + "  "
        Case 2
            FixSize = Mid$(size, 2, 2) + " "
    End Select

End Function

Private Function FixNumReqd(NumReqd As Long) As String
    Dim NumString As String
    Dim length As Integer
    
    NumString = Str(NumReqd)
    'All numstrings that have one digit begin with " "
    If Len(NumString) = 2 Then
        If NumString = " 0" Then
            NumString = "   "
        Else: NumString = NumString + " "
        End If
    Else
        length = Len(NumString) - 1
        NumString = Mid$(NumString, length, length) + " "
    End If
    
    FixNumReqd = NumString

End Function

Private Function FixDim(text As String, truelength As Integer, wantlength As Integer) As String
    Dim tmpString As String
    If IsNumeric(text) Then
        Dim count As Integer
        Dim amountToAdd As Integer
        Dim amountToSubtract As Integer
        Dim firsthalf As String
        Dim secondhalf As String
        Dim NewWantLength As Integer
        
        amountToAdd = wantlength - truelength
        amountToSubtract = truelength - wantlength
        If amountToSubtract > amountToAdd Then
            text = Mid$(text, 1, wantlength)
        End If
        
        firsthalf = StringBeforeDecimal(text)   'get string before decimal point
        secondhalf = StringAfterDecimal(text)   'get string after decimal point
        
        firsthalf = FormatFirstHalf(firsthalf)      'formats to field length and adds "."
        
        If unitSystem.UnitModeController.unitMode = umSI Then
            NewWantLength = 3
            truelength = Len(secondhalf)
            amountToSubtract = truelength - NewWantLength
            If amountToSubtract > 0 Then
                secondhalf = Mid$(secondhalf, 1, NewWantLength)
            End If
        End If
        
        If unitSystem.UnitModeController.unitMode = umSI Then
            secondhalf = FormatSecondHalf(secondhalf)   'formats to field length
        Else
            secondhalf = FixLength(secondhalf, Len(secondhalf), 2)
        End If
        
        text = firsthalf + secondhalf
        
        For count = 1 To (wantlength - 7)
            text = text + " "
        Next count
        tmpString = text
'        If val(tmpString) = 0 Then
'            tmpString = ""
'            For count = 1 To wantlength
'                tmpString = tmpString + " "
'            Next count
'        End If
        FixDim = tmpString
    Else
        tmpString = FormatUS(text)
        tmpString = Mid$(tmpString, 1, (Len(tmpString) - 1))
        If val(text) = 0 Then
            tmpString = ""
            For count = 1 To wantlength
                tmpString = tmpString + " "
            Next count
        End If
        FixDim = tmpString
    End If
    
End Function

Private Function StringBeforeDecimal(text As String) As String
    Dim firsthalf As String
    Dim count As Integer
    
    For count = 0 To Len(text)
        If Mid$(text, (count + 1), 1) = "." Then
            StringBeforeDecimal = Mid$(text, 1, count)
            Exit Function
        Else    'No "." present, so just return string without the " " prefix
            If count = Len(text) Then
                StringBeforeDecimal = Mid$(text, 2, count - 1)
                Exit Function
            End If
        End If
    
    Next count
    
End Function

Private Function StringAfterDecimal(text As String) As String
    Dim secondhalf As String
    Dim count As Integer
    
    For count = 1 To Len(text)
        If Mid$(text, (count), 1) = "." Then
            StringAfterDecimal = Mid$(text, (count + 1), (Len(text) - (count)))
            Exit Function
        Else    'No "." present, so just return a "0"
            If count = Len(text) Then
                StringAfterDecimal = "0"
                Exit Function
            End If
        End If
    Next count
    
End Function

Private Function FormatFirstHalf(text) As String
    FormatFirstHalf = FixLength("", Len(text), 3) & text & "."

End Function

Private Function FormatSecondHalf(text) As String
    Dim tmptext As String
    tmptext = text
    FormatSecondHalf = FixLength(tmptext, Len(text), 3)

End Function


Private Sub ReportVaries(bend As IBend, mass As Double, blumpsum As Boolean)
    Dim line As String
    line = ReportBend(bend, line, True, blumpsum)
    Dim length As String    'The " " is to keep this format (which is good) in sync
                            ' with the format of the "str" function (used earlier)
                            ' which leaves an xtra space at start of string
    length = FormatUnitValueWOTag(bend.length, "Dim")
    If blumpsum = True Then
        If unitSystem.UnitModeController.unitMode = umUS Then
            line = line + "  $  $"
        Else
            line = line + "    $   "
        End If
    Else
        If unitSystem.UnitModeController.unitMode = umUS Then
            length = ConvertUSLengthField(length)
            line = line + length
        Else
            line = line + FixDim(length, Len(length), 7)
        End If
    End If
    
'    line = line + FixDim(length, Len(length), 9)
    Dim weight As String
    weight = FormatUnitValueWOTag(mass, "Mass")
    If val(weight) = 0 Then
        line = line + FixLength(" ", Len(weight), 7) + " "
    Else
        line = line + FixLength(" ", Len(weight), 7) + weight
    End If
        
    AddLine line

End Sub

Private Sub ReportAllErrors(barrecord As barrecord)
   'Report bar record level errors
   ReportErrors barrecord.StatusMessages
   
   'Report Primary Bend Errors
    ReportErrors barrecord.PrimaryBend.StatusMessages
    
    'Report Varies Bend Errors
    If (barrecord.varies) Then ReportErrors barrecord.VariesBend.StatusMessages

End Sub

Private Function FormatUS(text As String) As String
    Dim wholenumber As String
    Dim fraction As String
    Dim count As Integer
    
    For count = 1 To Len(text)
    If Mid$(text, count, 1) = " " Then
        wholenumber = Mid$(text, 1, count - 1)
        fraction = Mid$(text, count + 1, (Len(text) - count))
    End If
    Next count
    'format first half
    wholenumber = FixLength("", Len(wholenumber), 3) + wholenumber
    'format second half
    fraction = FixDim(fraction, Len(fraction), 5)
    
    FormatUS = wholenumber + fraction
End Function


Private Sub AddLine(line As String)
    strBXFReport = strBXFReport + line + Chr$(13) + Chr$(10)

End Sub

Private Function GetReport() As String
    GetReport = strBXFReport
    
End Function

Private Sub SaveBXF(blist As Barlist)
    If Not blist Is Nothing Then
        Dim sfile As String

        With bxffrmUnits.dlgCommonDialog
            .DialogTitle = "Save"
            .CancelError = True
            .Flags = cdlOFNHideReadOnly
            On Error GoTo cnlError
            'ToDo: set the flags and attributes of the common dialog control
            .Filter = "Barlist Files (*.BXF)|*.BXF"
            .ShowSave
            If Len(.filename) = 0 Then
                Exit Sub
            End If
            sfile = .filename
        End With

        If sfile = "" Then Exit Sub
       
        If DoesFileExist(sfile) Then
            Dim answer As Integer
            answer = MsgBox(sfile & " already exists. " + Chr$(13) + Chr$(10) & _
                        "Do you want to replace it?", vbYesNo, "Save As")
            If answer = 7 Then
'                Unload bxffrmUnits
                Exit Sub
            Else
                'continue
            End If
        End If
        
        WriteBXFile sfile
        MsgBox "Barlist Exchange File Saved", vbInformation
    End If
'    Unload bxffrmUnits
    Exit Sub
    
cnlError:
    'Cancel was pushed
End Sub


Private Sub WriteBXFile(sfile As String)
    Dim intfilenumber As Integer
'    Dim count As Integer
'    Dim tmpString As String
    intfilenumber = FreeFile
    Open (sfile) For Output As intfilenumber
    
    Print #intfilenumber, strBXFReport
   
    Close #intfilenumber
    
End Sub





'Configures the display units manager by associating it with our
'units server and creating indirect measures using DisplayUnits objects
Private Sub ConfigureDisplayUnits()
   'Configure the display units manager
   
   'Dim
   Dim formatter As New WBFLUnitServer.DisplayUnitFormatter
   formatter.FormatSpecifiers 7, 3, tjRight, nftFixed, 0.0001
   Dim group As WBFLUnitServer.IDisplayUnitGroup
   Set group = dispUnitsMgr.Add("Dim", "Length")
   group.Add "m", unitsSI, formatter, True
   
   Dim ftformat As New WBFLUnitServer.AnnotatedDisplayUnitFormatter
   ftformat.Annotation = "'-,"""
   ftformat.Multiplier = 12#
   ftformat.OffsetDigits = 0
   ftformat.FormatSpecifiers 7, 1, tjRight, nftFixed, 0.0001
   group.Add "ft", unitsUS, ftformat, True
   
   'Length
   Set formatter = Nothing
   Set formatter = New WBFLUnitServer.DisplayUnitFormatter
   formatter.FormatSpecifiers 7, 3, tjRight, nftFixed, 0.0001
   Set group = dispUnitsMgr.Add("Length", "Length")
   group.Add "m", unitsSI, formatter, True
   
   Set ftformat = Nothing
   Set ftformat = New WBFLUnitServer.AnnotatedDisplayUnitFormatter
   ftformat.Annotation = "'-,"""
   ftformat.Multiplier = 12#
   ftformat.OffsetDigits = 0
   ftformat.FormatSpecifiers 7, 0, tjRight, nftFixed, 0.0001
   group.Add "ft", unitsUS, ftformat, True
   
   'Mass
   Set formatter = Nothing
   Set formatter = New WBFLUnitServer.DisplayUnitFormatter
   formatter.FormatSpecifiers 6, 0, tjRight, nftFixed, 0.0001
   Set group = dispUnitsMgr.Add("Mass", "Mass")
   group.Add "kg", unitsSI, formatter, True
   group.Add "lb", unitsUS, formatter, True
   
   'Angle
   Set group = dispUnitsMgr.Add("Angle", "Angle")
   Set formatter = Nothing
   Set formatter = New WBFLUnitServer.DisplayUnitFormatter
   formatter.FormatSpecifiers 4, 0, tjRight, nftFixed, 0.0001
   group.Add "deg", unitsAll, formatter, True
   
End Sub

'Takes a value and a DisplayUnits object and creates an output string.
'The value is in base units and is converted to display units.
'The unit tag is appended to the string
Private Function FormatUnitValue(val As Double, group As String) As String
   FormatUnitValue = dispUnitsMgr.Format(val, group, True)
End Function
Private Function FormatUnitValueWOTag(val As Double, group As String) As String
   FormatUnitValueWOTag = dispUnitsMgr.Format(val, group, False)
End Function

'Configures the display units manage for a particual set of system units
Public Sub SetDisplayUnitsMode(um As UnitSystemType)
   Debug.Assert False 'should never get here
End Sub

'Takes a StatusMessage object and formats it into a text string
Private Function FormatStatusMessage(msg As StatusMessage) As String
   Dim text As String
   text = msg.text
   
   Dim v1 As String
   Dim v2 As String
   
   If (VarType(msg.Val1) = vbDouble) Then
      v1 = FormatUnitValue(msg.Val1, "Length")
   Else
      v1 = msg.Val1
   End If
   
   If (VarType(msg.Val2) = vbDouble) Then
      v2 = FormatUnitValue(msg.Val2, "Length")
   Else
      v2 = msg.Val2
   End If
   
   text = Replace(text, "%1", v1, , , vbTextCompare)
   text = Replace(text, "%2", v2, , , vbTextCompare)
     
   FormatStatusMessage = text
End Function

Private Function FromBendGetStatusMessage(bend As IBend, count As Long) As String
    Dim Message As StatusMessage
    
    Set Message = bend.StatusMessages.Item(count)
    FromBendGetStatusMessage = FormatStatusMessage(Message)
    Set Message = Nothing
End Function

Private Function ConvertUSLengthField(text As String) As String
        Dim tmpString As String
        Dim count As Integer
        
        tmpString = FormatUS2(text)
        tmpString = Mid$(tmpString, 1, (Len(tmpString) - 1))
        If val(text) = 0 Or text = "" Then
            tmpString = ""
            For count = 1 To 6
                tmpString = tmpString + " "
            Next count
        End If
        ConvertUSLengthField = tmpString

End Function
Private Function FormatUS2(text As String) As String
    Dim wholenumber As String
    Dim fraction As String
    Dim intFraction As Integer
    Dim count As Integer
    
    For count = 1 To Len(text)
    If Mid$(text, count, 1) = " " Then
        wholenumber = Mid$(text, 1, count - 1)
        fraction = Mid$(text, count + 1, (Len(text) - count))
    End If
    Next count
    'format first half
    wholenumber = FixLength("", Len(wholenumber), 3) + wholenumber
    'format second half
    intFraction = Round(val(fraction), 0)
    fraction = Str(intFraction)
    fraction = FixDim(fraction, Len(fraction), 5)
    fraction = Mid$(fraction, 1, Len(fraction) - 3) + " "
    
    FormatUS2 = wholenumber + fraction
End Function


Private Function FillDollarSigns() As String
    Dim line As String
    Dim count As Integer
    
    If (unitSystem.UnitModeController.unitMode = unitsUS) Then
        line = "  $   $    $   $   $   $   $   $   $   $    $   $  "
    Else
        line = "   $        $        $       $       $      $   $"
    End If
    
    FillDollarSigns = line

End Function


Public Function GetRidOfSpaces(text As String) As String
    Dim count As Integer
    Dim length As Integer
    Dim startOfChar As Integer
    Dim endOfChar As Integer
    Dim char As String
    Dim parsedString As String
    
    length = Len(text)
    
    'find start of characters within string:
    For count = 1 To length
        char = Mid$(text, count, 1)
        If Not char = " " Then
            startOfChar = count
            Exit For
        End If
    Next
    
    'find end of characters within string:
    For count = length To 1 Step -1
        char = Mid$(text, count, 1)
        If Not char = " " Then
            endOfChar = count
            Exit For
        End If
    Next
    
    Dim count2 As Integer
    Dim truelength As Integer
    truelength = ((endOfChar - startOfChar) + 1)
    count2 = 1
    parsedString = ""
    For count = startOfChar To endOfChar
        If startOfChar = 0 And endOfChar = 0 Then Exit For
        parsedString = parsedString + Mid$(text, count, 1)
        
    Next
    
    GetRidOfSpaces = parsedString
    
End Function

Private Function DoesFileExist(filename As String) As Boolean
    'check if file exists:
    Dim FileExists As String
    FileExists = Dir$(filename)
    If FileExists > "" Then
        'The File Exists
        DoesFileExist = True
    Else
        DoesFileExist = False
    End If
    
End Function
