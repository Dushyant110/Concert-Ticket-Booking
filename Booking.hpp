#ifndef BOOKING_HPP
#define BOOKING_HPP

#include <string>
#include "Concert.hpp"

using namespace std;

class Booking {
private:
    string bookingId;
    string customerName;
    Concert* concert;
    int seatNumber;
    double totalPrice;

public:
    Booking(string bookingId, string customerName, Concert* concert, 
            int seatNumber, double totalPrice);
    
    string getBookingId() const;
    string getCustomerName() const;
    Concert* getConcert() const;
    int getSeatNumber() const;
    double getTotalPrice() const;
    void displayInfo() const;
};

#endif
