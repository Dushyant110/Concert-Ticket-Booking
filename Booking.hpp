#ifndef BOOKING_HPP
#define BOOKING_HPP

#include <string>
#include <memory>
#include <mutex>
#include "Concert.hpp"
#include "BookingPolicy.hpp"
#include "BookingState.hpp"

using namespace std;

class Booking {
private:
    string bookingId;
    string customerName;
    Concert* concert;
    int seatNumber;
    double totalPrice;
    shared_ptr<BookingPolicy> policy;
    unique_ptr<BookingState> state;
    mutable mutex bookingMutex;

public:
    Booking(string bookingId, string customerName, Concert* concert, 
            int seatNumber, double totalPrice);

    Booking(string bookingId, string customerName, Concert* concert, 
            int seatNumber, double totalPrice, shared_ptr<BookingPolicy> policy,
            unique_ptr<BookingState> initialState = nullptr);

    ~Booking() = default;

    Booking(const Booking&) = delete;
    Booking& operator=(const Booking&) = delete;

    Booking(Booking&& other) noexcept;
    Booking& operator=(Booking&& other) noexcept;

    string getBookingId() const;
    string getCustomerName() const;
    Concert* getConcert() const;
    int getSeatNumber() const;
    double getTotalPrice() const;
    string getStatus() const;
    shared_ptr<BookingPolicy> getPolicy() const;

    void setState(unique_ptr<BookingState> newState);
    bool confirm();
    bool cancel();
    void displayInfo() const;
};

#endif
