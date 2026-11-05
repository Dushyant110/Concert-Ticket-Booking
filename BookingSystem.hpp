#ifndef BOOKING_SYSTEM_HPP
#define BOOKING_SYSTEM_HPP

#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include "Concert.hpp"
#include "Booking.hpp"
#include "BookingPolicy.hpp"
#include "BookingService.hpp"

using namespace std;

class BookingSystem {
private:
    vector<Concert*> concerts;
    vector<shared_ptr<Booking>> bookings;
    int bookingIdCounter;
    mutable mutex systemMutex;
    BookingService bookingService;
    bool ownsConcerts;

public:
    explicit BookingSystem(bool ownsConcerts = true);
    ~BookingSystem();

    BookingSystem(const BookingSystem&) = delete;
    BookingSystem& operator=(const BookingSystem&) = delete;

    void addConcert(Concert* concert);
    Concert* findConcert(const string& concertId) const;
    void displayAllConcerts() const;
    void displayAvailableSeats(Concert* concert) const;

    string createBooking(const string& customerName, Concert* concert, int seatNumber);
    string createBooking(const string& customerName, Concert* concert, int seatNumber, 
                         shared_ptr<BookingPolicy> policy);
    bool cancelBooking(const string& bookingId);

    void displayAllBookings() const;
    Booking* findBooking(const string& bookingId) const;
    shared_ptr<Booking> getBooking(const string& bookingId) const;
};

#endif
