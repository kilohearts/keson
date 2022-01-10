@echo off
mkdir vs2019 2> nul
cd vs2019
..\genie.exe vs2019
if ERRORLEVEL 1 pause
cd ..
