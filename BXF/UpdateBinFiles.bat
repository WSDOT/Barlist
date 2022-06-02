REM - Script to prepare for Release

SET BINTARGET=..\bin
SET REGFREECOM=%ARPDIR%\BridgeLink\RegFreeCOM


xcopy /Y /d %REGFREECOM%\x64\Release\BXF.dll		%BINTARGET%\x64\


