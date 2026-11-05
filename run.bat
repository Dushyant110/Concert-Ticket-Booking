@echo off
setlocal
cd /d "%~dp0"

REM Prepend MinGW-w64 path if installed via chocolatey
if exist "C:\ProgramData\mingw64\mingw64\bin\g++.exe" (
    set "PATH=C:\ProgramData\mingw64\mingw64\bin;%PATH%"
)

echo ========================================================
echo  Building Concert Booking System (C++17)
echo ========================================================

g++ -static -std=c++17 -I. -o ConcertBookingDemo.exe ^
    Seat.cpp Concert.cpp BookingPolicy.cpp BookingState.cpp Booking.cpp BookingService.cpp BookingSystem.cpp ConcertBookingDemo.cpp

if errorlevel 1 (
    echo.
    echo [ERROR] Demo build failed.
    pause
    exit /b 1
)
echo [OK] Demo compiled successfully: ConcertBookingDemo.exe

g++ -static -std=c++17 -I. -o tests\ConcertBookingTests.exe ^
    Seat.cpp Concert.cpp BookingPolicy.cpp BookingState.cpp Booking.cpp BookingService.cpp BookingSystem.cpp tests\ConcertBookingTests.cpp

if errorlevel 1 (
    echo.
    echo [ERROR] Test suite build failed.
    pause
    exit /b 1
)
echo [OK] Test suite compiled successfully: tests\ConcertBookingTests.exe

echo.
echo ========================================================
echo  Running Automated Test Suite
echo ========================================================
.\tests\ConcertBookingTests.exe

if errorlevel 1 (
    echo.
    echo [ERROR] Automated tests failed!
    pause
    exit /b 1
)

echo.
echo ========================================================
echo  Running Live Demo
echo ========================================================
.\ConcertBookingDemo.exe

echo.
pause
