cd \ARP\Barlist\Documentation

rmdir /S /Q doc
doxygen Doxygen.dox

\ARP\BridgeLink\RegFreeCOM\x64\Release\MakeDocMap Barlist
rmdir /S /Q \ARP\Barlist\Docs\Barlist\%1\
mkdir \ARP\Barlist\Docs\Barlist\%1\
copy \ARP\Barlist\Documentation\doc\html\* \ARP\Barlist\Docs\Barlist\%1\
copy \ARP\Barlist\Documentation\Barlist.dm \ARP\Barlist\Docs\Barlist\%1\
