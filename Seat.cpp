#include "Seat.hpp"

using namespace std;

Seat::Seat(int number) : seatNumber(number), booked(false) {}

Seat::Seat(Seat&& other) noexcept {
    lock_guard<mutex> lock(other.seatMutex);
    seatNumber = other.seatNumber;
    booked = other.booked;
}

Seat& Seat::operator=(Seat&& other) noexcept {
    if (this != &other) {
        scoped_lock lock(seatMutex, other.seatMutex);
        seatNumber = other.seatNumber;
        booked = other.booked;
    }
    return *this;
}

int Seat::getSeatNumber() const {
    return seatNumber;
}

bool Seat::isBooked() const {
    lock_guard<mutex> lock(seatMutex);
    return booked;
}

bool Seat::book() {
    lock_guard<mutex> lock(seatMutex);
    if (booked) {
        return false;
    }
    booked = true;
    return true;
}

bool Seat::cancel() {
    lock_guard<mutex> lock(seatMutex);
    if (!booked) {
        return false;
    }
    booked = false;
    return true;
}
