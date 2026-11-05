#ifndef CONCERT_HPP
#define CONCERT_HPP

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include "Seat.hpp"

using namespace std;

class Concert {
private:
    string concertId;
    string name;
    string venue;
    string date;
    double basePrice;
    int totalSeats;
    vector<unique_ptr<Seat>> seats;
    mutable mutex concertMutex;

public:
    Concert(string concertId, string name, string venue, 
            string date, double basePrice, int totalSeats);
    ~Concert() = default;

    Concert(const Concert&) = delete;
    Concert& operator=(const Concert&) = delete;

    string getConcertId() const;
    string getName() const;
    string getVenue() const;
    string getDate() const;
    double getBasePrice() const;
    int getTotalSeats() const;
    int getAvailableSeats() const;
    
    bool bookSeat(int seatNumber);
    bool cancelSeat(int seatNumber);
    bool isSeatAvailable(int seatNumber) const;
    void displayInfo() const;
};

#endif
