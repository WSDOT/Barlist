REM - Script to prepare for Release

SET BINTARGET=..\bin
SET REGFREECOM=\ARP\Barlist\RegFreeCOM


xcopy /Y /d %REGFREECOM%\Win32\Release\Bar*.dll		%BINTARGET%\Win32\
xcopy /Y /d Schema\Barlist.xsd %BINTARGET%

