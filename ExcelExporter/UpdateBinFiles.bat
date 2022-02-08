REM - Script to prepare for Release

SET BINTARGET=..\bin
SET REGFREECOM=%ARPDIR%\BridgeLink\RegFreeCOM


xcopy /Y /d %REGFREECOM%\x64\Release\BarlistExcelExporter.dll		%BINTARGET%\x64\
xcopy /Y /d *.xltx		                                        %BINTARGET%\


