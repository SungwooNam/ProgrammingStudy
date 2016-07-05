call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

@echo off
set SRC_ROOT=%1
if "x%1"=="x" goto USAGE

set path=%path%;C:\Program Files (x86)\Java\jre7\bin;
SET JSTESTDRIVER_HOME=%SRC_ROOT%\3rdParty\JsTestDriver
rem start "JsTestDriver Server Process" "java.exe" "-jar %JSTESTDRIVER_HOME%/JsTestDriver-1.3.5.jar --port 4224"
rem cmd "java.exe -jar %JSTESTDRIVER_HOME%/JsTestDriver-1.3.5.jar --port 4224"
rem start "" "notepad.exe" "%SRC_ROOT%\Build\BuildAll.bat"

cd /d %SRC_ROOT%\Build
rem set path=%WIX%bin;%path%


goto EOF

:USAGE
echo Usage of StartBuild
echo   StartBuild.bat path_to_src_root

:EOF
@echo on
