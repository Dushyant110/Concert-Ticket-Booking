#ifndef CONCERT_HPP
#define CONCERT_HPP

#include <string>
#include <vector>
#include "Seat.hpp"

using namespace std;

class Concert {
private:
    string concertId;
    string name;
    string venue;
    string date;
    double basePrice;
    vector<Seat> seats;
    int totalSeats;

public:
    Concert(string concertId, string name, string venue, 
            string date, double basePrice, int totalSeats);
    
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
