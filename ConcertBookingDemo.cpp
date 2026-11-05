#include "BookingSystem.hpp"
#include "BookingPolicy.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

int main() {
    cout << "============================================================" << endl;
    cout << "       CONCERT TICKET BOOKING SYSTEM - LIVE DEMO" << endl;
    cout << "============================================================" << endl;

    BookingSystem bookingSystem;
    
    cout << "\n[1] Creating Concerts..." << endl;
    Concert* concert1 = new Concert("C001", "Rock Festival", "Stadium A", "2024-07-15", 100.0, 50);
    Concert* concert2 = new Concert("C002", "Jazz Night", "Hall B", "2024-07-20", 75.0, 30);
    
    bookingSystem.addConcert(concert1);
    bookingSystem.addConcert(concert2);
    
    bookingSystem.displayAllConcerts();
    
    cout << "\n[2] Making Standard Bookings..." << endl;
    string booking1 = bookingSystem.createBooking("John Doe", concert1, 1);
    string booking2 = bookingSystem.createBooking("Jane Smith", concert1, 2);
    string booking3 = bookingSystem.createBooking("Alice Johnson", concert2, 1);
    
    if (!booking1.empty()) {
        cout << "Booking created successfully: " << booking1 << endl;
    }
    if (!booking2.empty()) {
        cout << "Booking created successfully: " << booking2 << endl;
    }
    if (!booking3.empty()) {
        cout << "Booking created successfully: " << booking3 << endl;
    }
    
    bookingSystem.displayAllBookings();
    
    bookingSystem.displayAvailableSeats(concert1);
    
    cout << "\n[3] Cancelling Booking " << booking1 << "..." << endl;
    if (bookingSystem.cancelBooking(booking1)) {
        cout << "Booking " << booking1 << " cancelled successfully" << endl;
    }
    
    bookingSystem.displayAvailableSeats(concert1);

    cout << "\n============================================================" << endl;
    cout << " [DEMO] Strategy Pattern: Flexible Pricing Policies" << endl;
    cout << "============================================================" << endl;
    
    auto studentPolicy = make_shared<StudentBookingPolicy>(0.20);
    auto premiumPolicy = make_shared<PremiumBookingPolicy>(0.25);

    cout << "\nBooking seat 3 with Student Policy (20% off base $100)..." << endl;
    string studentBookingId = bookingSystem.createBooking("Bob Student", concert1, 3, studentPolicy);
    Booking* studentBooking = bookingSystem.findBooking(studentBookingId);
    if (studentBooking) {
        cout << "Created " << studentBookingId << " with Price: $" << studentBooking->getTotalPrice()
             << " (" << studentBooking->getPolicy()->getPolicyName() << ")" << endl;
    }

    cout << "\nBooking seat 4 with Premium Policy (25% VIP surcharge on base $100)..." << endl;
    string premiumBookingId = bookingSystem.createBooking("Charlie VIP", concert1, 4, premiumPolicy);
    Booking* premiumBooking = bookingSystem.findBooking(premiumBookingId);
    if (premiumBooking) {
        cout << "Created " << premiumBookingId << " with Price: $" << premiumBooking->getTotalPrice()
             << " (" << premiumBooking->getPolicy()->getPolicyName() << ")" << endl;
    }

    cout << "\n============================================================" << endl;
    cout << " [DEMO] State Pattern: Booking Lifecycle Validation" << endl;
    cout << "============================================================" << endl;

    if (studentBooking) {
        cout << "Current state of " << studentBookingId << ": " << studentBooking->getStatus() << endl;
        
        cout << "Cancelling " << studentBookingId << "..." << endl;
        bool cancelSuccess = bookingSystem.cancelBooking(studentBookingId);
        cout << "First cancellation result: " << (cancelSuccess ? "SUCCESS (Transitioned to CANCELLED)" : "FAILED") << endl;
        cout << "Current state of " << studentBookingId << ": " << studentBooking->getStatus() << endl;

        cout << "Attempting repeated cancellation on " << studentBookingId << "..." << endl;
        bool doubleCancel = bookingSystem.cancelBooking(studentBookingId);
        cout << "Repeated cancellation result: " << (doubleCancel ? "SUCCESS" : "REJECTED (State machine prevented double-cancel)") << endl;
    }

    cout << "\n============================================================" << endl;
    cout << " [DEMO] Mutex Synchronization: Concurrent Same-Seat Booking" << endl;
    cout << "============================================================" << endl;
    cout << "Spawning 10 threads simultaneously competing to book Seat 10 on " 
         << concert1->getName() << "..." << endl;

    const int targetSeat = 10;
    const int numThreads = 10;
    vector<thread> threads;
    atomic<bool> startSignal{false};
    atomic<int> successCount{0};
    atomic<int> failCount{0};
    string winningBookingId;
    mutex winMutex;

    for (int i = 1; i <= numThreads; i++) {
        threads.emplace_back([&, i]() {
            while (!startSignal.load(memory_order_acquire)) {
                this_thread::yield();
            }

            string user = "Competitor_" + to_string(i);
            string bId = bookingSystem.createBooking(user, concert1, targetSeat);
            if (!bId.empty()) {
                successCount.fetch_add(1, memory_order_relaxed);
                lock_guard<mutex> lock(winMutex);
                winningBookingId = bId;
            } else {
                failCount.fetch_add(1, memory_order_relaxed);
            }
        });
    }

    startSignal.store(true, memory_order_release);

    for (auto& t : threads) {
        t.join();
    }

    cout << "\nConcurrency Test Results for Seat " << targetSeat << ":" << endl;
    cout << "  Total Threads: " << numThreads << endl;
    cout << "  Successful Bookings: " << successCount.load() << " (Winner: " << winningBookingId << ")" << endl;
    cout << "  Failed Requests: " << failCount.load() << " (Safely rejected)" << endl;
    cout << "  Double Booking Occurred? " << (successCount.load() > 1 ? "YES (BUG!)" : "NO (Thread Safe!)") << endl;

    cout << "\n============================================================" << endl;
    cout << " Live Demo Completed Successfully!" << endl;
    cout << "============================================================" << endl;

    return 0;
}
