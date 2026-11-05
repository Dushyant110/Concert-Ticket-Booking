#include "Concert.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

Concert::Concert(string concertId, string name, string venue, 
                string date, double basePrice, int totalSeats)
    : concertId(concertId), name(name), venue(venue), date(date), 
      basePrice(basePrice), totalSeats(totalSeats) {
    
    // Initialize seats
    for (int i = 1; i <= totalSeats; i++) {
        seats.push_back(Seat(i));
    }
}

string Concert::getConcertId() const { return concertId; }
string Concert::getName() const { return name; }
string Concert::getVenue() const { return venue; }
string Concert::getDate() const { return date; }
double Concert::getBasePrice() const { return basePrice; }
int Concert::getTotalSeats() const { return totalSeats; }

int Concert::getAvailableSeats() const {
    int available = 0;
    for (const auto& seat : seats) {
        if (!seat.isBooked()) available++;
    }
    return available;
}

bool Concert::bookSeat(int seatNumber) {
    if (seatNumber < 1 || seatNumber > totalSeats) return false;
    if (seats[seatNumber - 1].isBooked()) return false;
    
    seats[seatNumber - 1].book();
    return true;
}

bool Concert::cancelSeat(int seatNumber) {
    if (seatNumber < 1 || seatNumber > totalSeats) return false;
    if (!seats[seatNumber - 1].isBooked()) return false;
    
    seats[seatNumber - 1].cancel();
    return true;
}

bool Concert::isSeatAvailable(int seatNumber) const {
    if (seatNumber < 1 || seatNumber > totalSeats) return false;
    return !seats[seatNumber - 1].isBooked();
}

void Concert::displayInfo() const {
    cout << "Concert: " << name << endl;
    cout << "ID: " << concertId << endl;
    cout << "Venue: " << venue << endl;
    cout << "Date: " << date << endl;
    cout << "Price: $" << fixed << setprecision(2) << basePrice << endl;
    cout << "Available Seats: " << getAvailableSeats() << "/" << totalSeats << endl;
}
