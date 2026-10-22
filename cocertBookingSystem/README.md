# Concert Ticket Booking System (C++)

Console C++ project for booking concert seats: list concerts, book a seat, show bookings, and cancel a booking.

## How to run

1. Open this folder.
2. Double-click `run.bat`.

Or in a terminal:

```bat
g++ -std=c++17 -o ConcertBookingDemo.exe Seat.cpp Concert.cpp Booking.cpp BookingSystem.cpp ConcertBookingDemo.cpp
ConcertBookingDemo.exe
```

You need MinGW `g++` installed (`C:\MinGW\bin` on this machine).

## Files

- `ConcertBookingDemo.cpp` — demo that creates concerts and bookings
- `BookingSystem`, `Concert`, `Seat`, `Booking` — source and headers
- [Problem statement](docs/concert-ticket-booking-system.md)
- [Class diagram](docs/concertticketbookingsystem-class-diagram.png)
