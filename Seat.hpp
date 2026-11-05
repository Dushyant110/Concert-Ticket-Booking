#ifndef SEAT_HPP
#define SEAT_HPP

#include <mutex>

using namespace std;

class Seat {
private:
    int seatNumber;
    bool booked;
    mutable mutex seatMutex;

public:
    explicit Seat(int number);
    ~Seat() = default;

    Seat(const Seat&) = delete;
    Seat& operator=(const Seat&) = delete;

    Seat(Seat&& other) noexcept;
    Seat& operator=(Seat&& other) noexcept;

    int getSeatNumber() const;
    bool isBooked() const;
    bool book();
    bool cancel();
};

#endif
