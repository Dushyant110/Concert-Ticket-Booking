#include "BookingSystem.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

BookingSystem::BookingSystem() : bookingIdCounter(1) {}

BookingSystem::~BookingSystem() {
    for (auto concert : concerts) delete concert;
    for (auto booking : bookings) delete booking;
}

void BookingSystem::addConcert(Concert* concert) {
    concerts.push_back(concert);
}

string BookingSystem::createBooking(string customerName, Concert* concert, int seatNumber) {
    if (!concert->bookSeat(seatNumber)) {
        cout << "Failed to book seat: Seat " << seatNumber << " is not available" << endl;
        return "";
    }
    
    string bookingId = "B" + to_string(bookingIdCounter++);
    double totalPrice = concert->getBasePrice();  // Could add premium pricing logic here
    
    Booking* booking = new Booking(bookingId, customerName, concert, seatNumber, totalPrice);
    bookings.push_back(booking);
    return bookingId;
}

bool BookingSystem::cancelBooking(string bookingId) {
    Booking* booking = findBooking(bookingId);
    if (!booking) return false;
    
    booking->getConcert()->cancelSeat(booking->getSeatNumber());
    
    auto it = find(bookings.begin(), bookings.end(), booking);
    if (it != bookings.end()) {
        bookings.erase(it);
        delete booking;
        return true;
    }
    return false;
}

void BookingSystem::displayAllConcerts() const {
    cout << "\nAvailable Concerts:" << endl;
    for (const auto& concert : concerts) {
        concert->displayInfo();
        cout << "------------------------" << endl;
    }
}

void BookingSystem::displayAllBookings() const {
    cout << "\nCurrent Bookings:" << endl;
    for (const auto& booking : bookings) {
        booking->displayInfo();
        cout << "------------------------" << endl;
    }
}

void BookingSystem::displayAvailableSeats(Concert* concert) const {
    cout << "\nAvailable seats for concert " << concert->getName() << ":" << endl;
    for (int i = 1; i <= concert->getTotalSeats(); i++) {
        if (concert->isSeatAvailable(i)) {
            cout << i << " ";
        }
    }
    cout << endl;
}

Concert* BookingSystem::findConcert(string concertId) const {
    for (auto concert : concerts) {
        if (concert->getConcertId() == concertId) return concert;
    }
    return nullptr;
}

Booking* BookingSystem::findBooking(string bookingId) const {
    for (auto booking : bookings) {
        if (booking->getBookingId() == bookingId) return booking;
    }
    return nullptr;
}
