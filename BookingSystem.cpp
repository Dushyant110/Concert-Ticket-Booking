#include "BookingSystem.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

BookingSystem::BookingSystem(bool ownsConcerts) 
    : bookingIdCounter(1), ownsConcerts(ownsConcerts) {}

BookingSystem::~BookingSystem() {
    if (ownsConcerts) {
        for (auto concert : concerts) {
            delete concert;
        }
    }
}

void BookingSystem::addConcert(Concert* concert) {
    if (!concert) return;
    lock_guard<mutex> lock(systemMutex);
    concerts.push_back(concert);
}

string BookingSystem::createBooking(const string& customerName, Concert* concert, int seatNumber) {
    return createBooking(customerName, concert, seatNumber, make_shared<RegularBookingPolicy>());
}

string BookingSystem::createBooking(const string& customerName, Concert* concert, 
                                   int seatNumber, shared_ptr<BookingPolicy> policy) {
    string bookingId;
    {
        lock_guard<mutex> lock(systemMutex);
        bookingId = "B" + to_string(bookingIdCounter++);
    }

    auto booking = bookingService.createBooking(bookingId, customerName, concert, seatNumber, policy);
    if (!booking) {
        cout << "Failed to book seat: Seat " << seatNumber << " is not available" << endl;
        return "";
    }

    {
        lock_guard<mutex> lock(systemMutex);
        bookings.push_back(booking);
    }

    return bookingId;
}

bool BookingSystem::cancelBooking(const string& bookingId) {
    shared_ptr<Booking> booking = getBooking(bookingId);
    if (!booking) {
        return false;
    }

    return bookingService.cancelBooking(booking);
}

void BookingSystem::displayAllConcerts() const {
    lock_guard<mutex> lock(systemMutex);
    cout << "\nAvailable Concerts:" << endl;
    for (const auto& concert : concerts) {
        concert->displayInfo();
        cout << "------------------------" << endl;
    }
}

void BookingSystem::displayAllBookings() const {
    lock_guard<mutex> lock(systemMutex);
    cout << "\nCurrent Bookings:" << endl;
    for (const auto& booking : bookings) {
        booking->displayInfo();
        cout << "------------------------" << endl;
    }
}

void BookingSystem::displayAvailableSeats(Concert* concert) const {
    if (!concert) return;
    cout << "\nAvailable seats for concert " << concert->getName() << ":" << endl;
    for (int i = 1; i <= concert->getTotalSeats(); i++) {
        if (concert->isSeatAvailable(i)) {
            cout << i << " ";
        }
    }
    cout << endl;
}

Concert* BookingSystem::findConcert(const string& concertId) const {
    lock_guard<mutex> lock(systemMutex);
    for (auto concert : concerts) {
        if (concert->getConcertId() == concertId) {
            return concert;
        }
    }
    return nullptr;
}

Booking* BookingSystem::findBooking(const string& bookingId) const {
    auto sp = getBooking(bookingId);
    return sp.get();
}

shared_ptr<Booking> BookingSystem::getBooking(const string& bookingId) const {
    lock_guard<mutex> lock(systemMutex);
    for (const auto& booking : bookings) {
        if (booking->getBookingId() == bookingId) {
            return booking;
        }
    }
    return nullptr;
}
