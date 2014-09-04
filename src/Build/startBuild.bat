call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

@echo off
set SRC_BUILD=%1
if "x%1"=="x" goto USAGE

cd /d %SRC_BUILD%
rem set path=%WIX%bin;%path%

goto EOF

:USAGE
echo Usage of StartScanner
echo   StartBuild.bat path_to_build

:EOF
@echo on
