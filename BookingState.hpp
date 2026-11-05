#ifndef BOOKING_STATE_HPP
#define BOOKING_STATE_HPP

#include <string>
#include <memory>

using namespace std;

class Booking;

class BookingState {
public:
    virtual ~BookingState() = default;
    virtual string getStateName() const = 0;
    virtual bool confirm(Booking& booking) = 0;
    virtual bool cancel(Booking& booking) = 0;
};

class PendingState : public BookingState {
public:
    string getStateName() const override;
    bool confirm(Booking& booking) override;
    bool cancel(Booking& booking) override;
};

class ConfirmedState : public BookingState {
public:
    string getStateName() const override;
    bool confirm(Booking& booking) override;
    bool cancel(Booking& booking) override;
};

class CancelledState : public BookingState {
public:
    string getStateName() const override;
    bool confirm(Booking& booking) override;
    bool cancel(Booking& booking) override;
};

#endif
