@echo off
for /F %%i in ('git tag --list --sort=committerdate') do set VERSION=%%i
for /F %%i in ('git rev-parse HEAD') do set BUILDCOMMIT=%%i
set BUILDCOMMIT=%BUILDCOMMIT:~0,8%
for /F %%i in ('git branch --show-current') do set BUILDBRANCH=%%i

echo #pragma once > ..\..\UnicornExpansion\UnicornExpansion\version.h
echo const char * VERSION = "%VERSION%" ; >> ..\..\UnicornExpansion\UnicornExpansion\version.h

SET PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin
msbuild.exe ..\..\UnicornExpansion\UnicornExpansion.sln /p:Configuration=Release

SET BINDIR=..\..\UnicornExpansion\x64\Release
SET DATADIR=..\..\data

SmartZipBuilder.exe script.szb
