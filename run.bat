@echo off
cd /d "%~dp0"
g++ -std=c++17 -o ConcertBookingDemo.exe Seat.cpp Concert.cpp Booking.cpp BookingSystem.cpp ConcertBookingDemo.cpp
if errorlevel 1 (
  echo Build failed.
  pause
  exit /b 1
)
echo.
echo === Running Concert Booking Demo ===
echo.
ConcertBookingDemo.exe
echo.
pause
