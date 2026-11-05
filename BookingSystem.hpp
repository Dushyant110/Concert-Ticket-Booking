#ifndef BOOKING_SYSTEM_HPP
#define BOOKING_SYSTEM_HPP

#include <vector>
#include <string>
#include "Concert.hpp"
#include "Booking.hpp"

using namespace std;

class BookingSystem {
private:
    vector<Concert*> concerts;
    vector<Booking*> bookings;
    int bookingIdCounter;

public:
    BookingSystem();
    ~BookingSystem();
    
    void addConcert(Concert* concert);
    string createBooking(string customerName, Concert* concert, int seatNumber);
    bool cancelBooking(string bookingId);
    
    void displayAllConcerts() const;
    void displayAllBookings() const;
    void displayAvailableSeats(Concert* concert) const;
    
    Concert* findConcert(string concertId) const;
    Booking* findBooking(string bookingId) const;
};

#endif
