REM - Script to prepare for Release

REM - Update Barlist Files
cd %ARPDIR%\Barlist\Barlist
call UpdateBinFiles.bat

cd %ARPDIR%\Barlist\BXF
call UpdateBinFiles.bat

cd %ARPDIR%\Barlist\ExcelExporter
call UpdateBinFiles.bat

REM - Update Barlist Files

cd %ARPDIR%\Barlist
SET BINTARGET=bin
SET REGFREECOM=%ARPDIR%\BridgeLink\RegFreeCOM

REM - Application files
xcopy /Y /d %REGFREECOM%\x64\Release\Barlist.dll 	%BINTARGET%\x64\
xcopy /Y /d %REGFREECOM%\x64\Release\Barlist.exe 	%BINTARGET%\x64\
