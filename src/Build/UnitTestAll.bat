@echo off

@SET TARGET=%1

if "x%1"=="x" SET TARGET=FullBuild

msbuild UnitTestAll.proj /t:%TARGET%
