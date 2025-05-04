@echo off

rem ## Daisuke Engine Visual Studio project setup script
rem ## Copyright 2025 Daisuke Games, Inc. All Rights Reserved.
rem ##
rem ## This script is expecting to exist in the root directory.  It will not work correctly
rem ## if you copy it to a different location and run it.

setlocal
echo Setting up DaisukeEngine project files...

cmake -B Builds -G "Visual Studio 17 2022"

set /p DUMMY=Hit ENTER to continue...