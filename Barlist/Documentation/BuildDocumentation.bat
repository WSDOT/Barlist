cd \ARP\Barlist\Barlist\Documentation

rmdir /S /Q doc
doxygen Doxygen.dox

\ARP\BridgeLink\RegFreeCOM\x64\Release\MakeDocMap Barlist
rmdir /S /Q \ARP\BridgeLink\Docs\Barlist\Barlist\%1\
mkdir \ARP\BridgeLink\Docs\Barlist\Barlist\%1\
copy \ARP\Barlist\Barlist\Documentation\doc\html\* \ARP\BridgeLink\Docs\Barlist\Barlist\%1\
copy \ARP\Barlist\Barlist\Documentation\Barlist.dm \ARP\BridgeLink\Docs\Barlist\Barlist\%1\BarlistDocs.dm
