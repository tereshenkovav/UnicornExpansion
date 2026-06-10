@echo off
for /F %%i in ('git tag --list --sort=committerdate') do set BUILDTAG=%%i
for /F %%i in ('git rev-parse HEAD') do set BUILDCOMMIT=%%i
set BUILDCOMMIT=%BUILDCOMMIT:~0,8%
for /F %%i in ('git branch --show-current') do set BUILDBRANCH=%%i

SET VERSION=%BUILDTAG:~1%

echo #pragma once > ..\..\UnicornExpansion\UnicornExpansion\version.h
echo const char * VERSION = "%VERSION%" ; >> ..\..\UnicornExpansion\UnicornExpansion\version.h

SET PATH=%PATH%;C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin
msbuild.exe ..\..\UnicornExpansion\UnicornExpansion.sln /p:Configuration=Release

SET BINDIR=..\..\UnicornExpansion\x64\Release

del ..\..\data\deflang

"C:\Program Files (x86)\NSIS\makensis.exe" /DBINDIR=%BINDIR% /DVERSION=%VERSION% /DGAMELANG=ru /DUPPERLANG=RU UnicornExpansion.nsi
"C:\Program Files (x86)\NSIS\makensis.exe" /DBINDIR=%BINDIR% /DVERSION=%VERSION% /DGAMELANG=en /DUPPERLANG=EN UnicornExpansion.nsi
SmartZipBuilder.exe script.szb /LANGL=ru /LANGH=RU
SmartZipBuilder.exe script.szb /LANGL=en /LANGH=EN
