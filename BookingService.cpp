#include "BookingService.hpp"
#include <iostream>

using namespace std;

shared_ptr<Booking> BookingService::createBooking(
    const string& bookingId,
    const string& customerName,
    Concert* concert,
    int seatNumber,
    shared_ptr<BookingPolicy> policy) {

    if (!concert) {
        cerr << "Booking failed: Invalid concert reference" << endl;
        return nullptr;
    }

    if (!policy) {
        policy = make_shared<RegularBookingPolicy>();
    }

    if (!concert->bookSeat(seatNumber)) {
        return nullptr;
    }

    double totalPrice = policy->calculatePrice(concert->getBasePrice());

    auto booking = make_shared<Booking>(
        bookingId,
        customerName,
        concert,
        seatNumber,
        totalPrice,
        policy,
        make_unique<PendingState>()
    );

    if (!booking->confirm()) {
        concert->cancelSeat(seatNumber);
        return nullptr;
    }

    return booking;
}

bool BookingService::cancelBooking(shared_ptr<Booking> booking) {
    if (!booking) {
        return false;
    }

    if (!booking->cancel()) {
        return false;
    }

    if (booking->getConcert()) {
        booking->getConcert()->cancelSeat(booking->getSeatNumber());
    }

    return true;
}
