@echo off
:: Create build directory if it doesn't exist
if not exist "%~dp0..\build" (
    mkdir "%~dp0..\build"
)

:: Navigate to build directory
cd "%~dp0..\build"

:: Define paths to external libraries (update these paths according to your system)
set OpenCV_DIR=C:\path\to\opencv                          
set EIGEN3_INCLUDE_DIR=C:\path\to\eigen
set CERES_DIR=C:\path\to\ceres

:: Run CMake to generate Visual Studio solution and specify libraries' directories
cmake -G "Visual Studio 17 2022" -A x64 ^
    -DOpenCV_DIR=%OpenCV_DIR% ^
    -DEIGEN3_INCLUDE_DIR=%EIGEN3_INCLUDE_DIR% ^
    -DCeres_DIR=%CERES_DIR% ^
    ..

:: Check if CMake succeeded
if %errorlevel% neq 0 (
    echo CMake failed to generate solution.
    exit /b %errorlevel%
)

echo Solution generated successfully.
pause