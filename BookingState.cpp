#include "BookingState.hpp"
#include "Booking.hpp"

using namespace std;

string PendingState::getStateName() const {
    return "PENDING";
}

bool PendingState::confirm(Booking& booking) {
    booking.setState(make_unique<ConfirmedState>());
    return true;
}

bool PendingState::cancel(Booking& booking) {
    booking.setState(make_unique<CancelledState>());
    return true;
}

string ConfirmedState::getStateName() const {
    return "CONFIRMED";
}

bool ConfirmedState::confirm(Booking&) {
    return false;
}

bool ConfirmedState::cancel(Booking& booking) {
    booking.setState(make_unique<CancelledState>());
    return true;
}

string CancelledState::getStateName() const {
    return "CANCELLED";
}

bool CancelledState::confirm(Booking&) {
    return false;
}

bool CancelledState::cancel(Booking&) {
    return false;
}
