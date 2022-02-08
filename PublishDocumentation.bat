REM - Script to build and publish documentation sets to the WSDOT http server

SET DOC_TARGET=\\wsdot\Resources\Topics\Publish\Web\ProdCF\EESC\Bridge\software\Documentation

REM - use only the major and minor build number
REM - this assumes documentation for version 1.1.0 is the same as for version 1.1.1
REM - which will generally be true. This way we don't have to publish new documentation
REM - for every bug-fix release. Just use 1.1 for the version number

SET BARLIST_VERSION=7.0

REM - Build the documentation sets
call %ARPDIR%\Barlist\Documentation\BuildDocumentation.bat          %BARLIST_VERSION%
call %ARPDIR%\Barlist\Barlist\Documentation\BuildDocumentation.bat          %BARLIST_VERSION%

cd %ARPDIR%\Barlist

REM the /b option is for build only, no publish
if "%1" == "/b" goto END

REM - Copy the documentation sets to the WSDOT server
REM - Documenation for the Barlist driver application
rmdir /S /Q %DOC_TARGET%\Barlist\%BARLIST_VERSION%\
mkdir %DOC_TARGET%\Barlist\%BARLIST_VERSION%\
copy %ARPDIR%\Barlist\Documentation\Barlist.dm %DOC_TARGET%\Barlist\%BARLIST_VERSION%\Barlist.dm.html
copy %ARPDIR%\Barlist\Documentation\doc\html\* %DOC_TARGET%\Barlist\%BARLIST_VERSION%\

REM - Documenation for the Barlist plugin to the driver application
mkdir %DOC_TARGET%\Barlist\Barlist\%BARLIST_VERSION%\
copy %ARPDIR%\Barlist\Barlist\Documentation\Barlist.dm %DOC_TARGET%\Barlist\Barlist\%BARLIST_VERSION%\BarlistDocs.dm.html
copy %ARPDIR%\Barlist\Barlist\Documentation\doc\html\* %DOC_TARGET%\Barlist\Barlist\%BARLIST_VERSION%\

:END

cd %ARPDIR%\Barlist
