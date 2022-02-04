REM - Script to prepare for Release

REM - Update Barlist Files
cd \ARP\Barlist\Barlist
call UpdateBinFiles.bat

cd \ARP\Barlist\BXF
call UpdateBinFiles.bat

cd \ARP\Barlist\ExcelExporter
call UpdateBinFiles.bat

REM - Update Barlist Files

cd \ARP\Barlist
SET BINTARGET=bin
SET REGFREECOM=\ARP\BridgeLink\RegFreeCOM

REM - Application files
xcopy /Y /d %REGFREECOM%\x64\Release\Barlist.dll 	%BINTARGET%\x64\
xcopy /Y /d %REGFREECOM%\x64\Release\Barlist.exe 	%BINTARGET%\x64\
