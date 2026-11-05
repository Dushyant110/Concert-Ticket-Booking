#ifndef BOOKING_SERVICE_HPP
#define BOOKING_SERVICE_HPP

#include <string>
#include <memory>
#include "Concert.hpp"
#include "Booking.hpp"
#include "BookingPolicy.hpp"

using namespace std;

class BookingService {
public:
    BookingService() = default;
    ~BookingService() = default;

    shared_ptr<Booking> createBooking(
        const string& bookingId,
        const string& customerName,
        Concert* concert,
        int seatNumber,
        shared_ptr<BookingPolicy> policy
    );

    bool cancelBooking(shared_ptr<Booking> booking);
};

#endif
