REM - Script to prepare for Release

REM - Update WBFL Files
cd \ARP\WBFL
call UpdateBinFiles.bat

REM - Update Barlist Files
cd \ARP\Barlist\Barlist
call UpdateBinFiles.bat

cd \ARP\Barlist\BXF
call UpdateBinFiles.bat

REM - Update Barlist Files

cd \ARP\Barlist
SET BINTARGET=bin

REM - Application files
xcopy /Y /d RegFreeCOM\Win32\Release\Barlist.exe 	%BINTARGET%\Win32\
xcopy /Y /d License.txt					%BINTARGET%\
xcopy /Y /d res\Barlist.ico                          %BINTARGET%\
