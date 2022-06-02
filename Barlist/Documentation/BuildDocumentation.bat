cd %ARPDIR%\Barlist\Barlist\Documentation

rmdir /S /Q doc
doxygen Doxygen.dox

%ARPDIR%\BridgeLink\RegFreeCOM\x64\Release\MakeDocMap Barlist
rmdir /S /Q %ARPDIR%\BridgeLink\Docs\Barlist\Barlist\%1\
mkdir %ARPDIR%\BridgeLink\Docs\Barlist\Barlist\%1\
copy %ARPDIR%\Barlist\Barlist\Documentation\doc\html\* %ARPDIR%\BridgeLink\Docs\Barlist\Barlist\%1\
copy %ARPDIR%\Barlist\Barlist\Documentation\Barlist.dm %ARPDIR%\BridgeLink\Docs\Barlist\Barlist\%1\BarlistDocs.dm
