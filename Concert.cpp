#include "Concert.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

Concert::Concert(string concertId, string name, string venue, 
                 string date, double basePrice, int totalSeats)
    : concertId(move(concertId)), name(move(name)), 
      venue(move(venue)), date(move(date)), 
      basePrice(basePrice), totalSeats(totalSeats) {
    seats.reserve(totalSeats);
    for (int i = 1; i <= totalSeats; i++) {
        seats.push_back(make_unique<Seat>(i));
    }
}

string Concert::getConcertId() const {
    return concertId;
}

string Concert::getName() const {
    return name;
}

string Concert::getVenue() const {
    return venue;
}

string Concert::getDate() const {
    return date;
}

double Concert::getBasePrice() const {
    return basePrice;
}

int Concert::getTotalSeats() const {
    return totalSeats;
}

int Concert::getAvailableSeats() const {
    lock_guard<mutex> lock(concertMutex);
    int available = 0;
    for (const auto& seat : seats) {
        if (!seat->isBooked()) {
            available++;
        }
    }
    return available;
}

bool Concert::bookSeat(int seatNumber) {
    if (seatNumber < 1 || seatNumber > totalSeats) {
        return false;
    }
    return seats[seatNumber - 1]->book();
}

bool Concert::cancelSeat(int seatNumber) {
    if (seatNumber < 1 || seatNumber > totalSeats) {
        return false;
    }
    return seats[seatNumber - 1]->cancel();
}

bool Concert::isSeatAvailable(int seatNumber) const {
    if (seatNumber < 1 || seatNumber > totalSeats) {
        return false;
    }
    return !seats[seatNumber - 1]->isBooked();
}

void Concert::displayInfo() const {
    cout << "Concert: " << name << "\n"
         << "ID: " << concertId << "\n"
         << "Venue: " << venue << "\n"
         << "Date: " << date << "\n"
         << "Price: $" << fixed << setprecision(2) << basePrice << "\n"
         << "Available Seats: " << getAvailableSeats() << "/" << totalSeats << endl;
}
