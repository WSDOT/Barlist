cd %ARPDIR%\Barlist\Documentation

rmdir /S /Q doc
doxygen Doxygen.dox

%ARPDIR%\BridgeLink\RegFreeCOM\x64\Release\MakeDocMap Barlist
rmdir /S /Q %ARPDIR%\BridgeLink\Docs\Barlist\%1\
mkdir %ARPDIR%\BridgeLink\Docs\Barlist\%1\
copy %ARPDIR%\Barlist\Documentation\doc\html\* %ARPDIR%\BridgeLink\Docs\Barlist\%1\
copy %ARPDIR%\Barlist\Documentation\Barlist.dm %ARPDIR%\BridgeLink\Docs\Barlist\%1\
