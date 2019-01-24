REM - Script to prepare for Release

SET BINTARGET=..\bin
SET REGFREECOM=\ARP\Barlist\RegFreeCOM


xcopy /Y /d %REGFREECOM%\Win32\Release\BXF.dll		%BINTARGET%\Win32\


