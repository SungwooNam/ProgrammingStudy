@echo off
set path=%path%;C:\Program Files (x86)\Java\jre7\bin;
set SRC_BUILD=%~dp0
SET JSTESTDRIVER_HOME=%SRC_BUILD%\..\3rdParty\JsTestDriver
rem start "JsTestDriver Server Process" /B "java.exe" " -jar %JSTESTDRIVER_HOME%\JsTestDriver-1.3.5.jar --port 4224 --browser ""C:\Program Files (x86)\Legpat\Application\chrome.exe"""
rem cmd /k java.exe -jar %JSTESTDRIVER_HOME%/JsTestDriver-1.3.5.jar --port 4224 --browser "C:\Program Files (x86)\Legpat\Application\chrome.exe","C:\Program Files\Internet Explorer\iexplore.exe"
cmd /k java.exe -jar %JSTESTDRIVER_HOME%/JsTestDriver-1.3.5.jar --port 4224 --browser "C:\Program Files (x86)\Legpat\Application\chrome.exe"
pause