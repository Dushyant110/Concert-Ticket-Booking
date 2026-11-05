#include "Booking.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

Booking::Booking(string bookingId, string customerName, Concert* concert, 
                int seatNumber, double totalPrice)
    : bookingId(bookingId), customerName(customerName), concert(concert), 
      seatNumber(seatNumber), totalPrice(totalPrice) {}

string Booking::getBookingId() const { return bookingId; }
string Booking::getCustomerName() const { return customerName; }
Concert* Booking::getConcert() const { return concert; }
int Booking::getSeatNumber() const { return seatNumber; }
double Booking::getTotalPrice() const { return totalPrice; }

void Booking::displayInfo() const {
    cout << "\nBooking Details:" << endl;
    cout << "Booking ID: " << bookingId << endl;
    cout << "Customer Name: " << customerName << endl;
    cout << "Seat Number: " << seatNumber << endl;
    cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
    cout << "\nConcert Information:" << endl;
    concert->displayInfo();
}
