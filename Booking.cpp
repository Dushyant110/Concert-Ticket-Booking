#include "Booking.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

Booking::Booking(string bookingId, string customerName, Concert* concert, 
                 int seatNumber, double totalPrice)
    : bookingId(move(bookingId)), customerName(move(customerName)), 
      concert(concert), seatNumber(seatNumber), totalPrice(totalPrice),
      policy(make_shared<RegularBookingPolicy>()),
      state(make_unique<ConfirmedState>()) {}

Booking::Booking(string bookingId, string customerName, Concert* concert, 
                 int seatNumber, double totalPrice, shared_ptr<BookingPolicy> policy,
                 unique_ptr<BookingState> initialState)
    : bookingId(move(bookingId)), customerName(move(customerName)), 
      concert(concert), seatNumber(seatNumber), totalPrice(totalPrice),
      policy(policy ? move(policy) : make_shared<RegularBookingPolicy>()),
      state(initialState ? move(initialState) : make_unique<ConfirmedState>()) {}

Booking::Booking(Booking&& other) noexcept {
    lock_guard<mutex> lock(other.bookingMutex);
    bookingId = move(other.bookingId);
    customerName = move(other.customerName);
    concert = other.concert;
    seatNumber = other.seatNumber;
    totalPrice = other.totalPrice;
    policy = move(other.policy);
    state = move(other.state);
}

Booking& Booking::operator=(Booking&& other) noexcept {
    if (this != &other) {
        scoped_lock lock(bookingMutex, other.bookingMutex);
        bookingId = move(other.bookingId);
        customerName = move(other.customerName);
        concert = other.concert;
        seatNumber = other.seatNumber;
        totalPrice = other.totalPrice;
        policy = move(other.policy);
        state = move(other.state);
    }
    return *this;
}

string Booking::getBookingId() const {
    lock_guard<mutex> lock(bookingMutex);
    return bookingId;
}

string Booking::getCustomerName() const {
    lock_guard<mutex> lock(bookingMutex);
    return customerName;
}

Concert* Booking::getConcert() const {
    lock_guard<mutex> lock(bookingMutex);
    return concert;
}

int Booking::getSeatNumber() const {
    lock_guard<mutex> lock(bookingMutex);
    return seatNumber;
}

double Booking::getTotalPrice() const {
    lock_guard<mutex> lock(bookingMutex);
    return totalPrice;
}

string Booking::getStatus() const {
    lock_guard<mutex> lock(bookingMutex);
    return state ? state->getStateName() : "UNKNOWN";
}

shared_ptr<BookingPolicy> Booking::getPolicy() const {
    lock_guard<mutex> lock(bookingMutex);
    return policy;
}

void Booking::setState(unique_ptr<BookingState> newState) {
    state = move(newState);
}

bool Booking::confirm() {
    lock_guard<mutex> lock(bookingMutex);
    if (!state) return false;
    return state->confirm(*this);
}

bool Booking::cancel() {
    lock_guard<mutex> lock(bookingMutex);
    if (!state) return false;
    return state->cancel(*this);
}

void Booking::displayInfo() const {
    lock_guard<mutex> lock(bookingMutex);
    cout << "\nBooking Details:" << "\n"
         << "Booking ID: " << bookingId << "\n"
         << "Customer Name: " << customerName << "\n"
         << "Seat Number: " << seatNumber << "\n"
         << "Status: " << (state ? state->getStateName() : "UNKNOWN") << "\n"
         << "Pricing Policy: " << (policy ? policy->getPolicyName() : "None") << "\n"
         << "Total Price: $" << fixed << setprecision(2) << totalPrice << "\n"
         << "\nConcert Information:" << endl;
    if (concert) {
        concert->displayInfo();
    }
}
