@echo off
set BUILD_DIR=%~dp0..\build

cd /d %BUILD_DIR%

if exist %BUILD_DIR% (
    echo Cleaning up the build directory: %BUILD_DIR%

    del /S /Q *.* 2>nul
    for /d %%i in (*) do rmdir /S /Q "%%i"
    
    echo Cleanup complete.
) else (
    echo Build directory not found: %BUILD_DIR%
)

pause
