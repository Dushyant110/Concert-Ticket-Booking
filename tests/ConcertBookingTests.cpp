#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <cassert>
#include <cmath>
#include <string>
#include <memory>
#include "BookingSystem.hpp"
#include "BookingPolicy.hpp"
#include "BookingState.hpp"

using namespace std;

static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            cerr << "  [FAIL] " << msg << " (" << __FILE__ << ":" << __LINE__ << ")" << endl; \
            g_tests_failed++; \
        } else { \
            g_tests_passed++; \
        } \
    } while(0)

#define RUN_TEST(fn) \
    do { \
        cout << "Running " << #fn << "..." << endl; \
        fn(); \
        cout << "  Completed " << #fn << endl; \
    } while(0)

void testConcertCreation() {
    Concert concert("C100", "Summer Symphony", "Grand Hall", "2026-12-01", 120.0, 50);
    TEST_ASSERT(concert.getConcertId() == "C100", "Concert ID matches");
    TEST_ASSERT(concert.getName() == "Summer Symphony", "Concert name matches");
    TEST_ASSERT(concert.getVenue() == "Grand Hall", "Venue matches");
    TEST_ASSERT(concert.getDate() == "2026-12-01", "Date matches");
    TEST_ASSERT(abs(concert.getBasePrice() - 120.0) < 1e-5, "Base price matches");
    TEST_ASSERT(concert.getTotalSeats() == 50, "Total seats is 50");
    TEST_ASSERT(concert.getAvailableSeats() == 50, "All 50 seats initially available");
}

void testValidSeatBooking() {
    BookingSystem system(false);
    Concert concert("C101", "Rock Night", "Arena 1", "2026-12-05", 100.0, 20);
    system.addConcert(&concert);

    string bId = system.createBooking("Alice", &concert, 5);
    TEST_ASSERT(!bId.empty(), "Booking ID should be non-empty");
    TEST_ASSERT(!concert.isSeatAvailable(5), "Seat 5 should now be unavailable");
    TEST_ASSERT(concert.getAvailableSeats() == 19, "Available seats should decrease by 1");

    Booking* b = system.findBooking(bId);
    TEST_ASSERT(b != nullptr, "Booking must be retrievable by ID");
    TEST_ASSERT(b->getSeatNumber() == 5, "Booking seat number must be 5");
    TEST_ASSERT(b->getStatus() == "CONFIRMED", "Initial active booking status must be CONFIRMED");
    TEST_ASSERT(abs(b->getTotalPrice() - 100.0) < 1e-5, "Regular booking price matches base price");
}

void testBookAlreadyBookedSeat() {
    BookingSystem system(false);
    Concert concert("C102", "Pop Gala", "Arena 2", "2026-12-06", 80.0, 20);
    system.addConcert(&concert);

    string b1 = system.createBooking("Alice", &concert, 3);
    TEST_ASSERT(!b1.empty(), "First booking on seat 3 succeeds");

    string b2 = system.createBooking("Bob", &concert, 3);
    TEST_ASSERT(b2.empty(), "Second booking on seat 3 must fail");
    TEST_ASSERT(concert.getAvailableSeats() == 19, "Seat count remains 19");
}

void testInvalidSeatSelection() {
    BookingSystem system(false);
    Concert concert("C103", "Acoustic Night", "Club A", "2026-12-07", 50.0, 10);
    system.addConcert(&concert);

    string bZero = system.createBooking("Charlie", &concert, 0);
    TEST_ASSERT(bZero.empty(), "Booking seat 0 must fail");

    string bNeg = system.createBooking("Charlie", &concert, -1);
    TEST_ASSERT(bNeg.empty(), "Booking seat -1 must fail");

    string bOver = system.createBooking("Charlie", &concert, 11);
    TEST_ASSERT(bOver.empty(), "Booking seat 11 on a 10-seat concert must fail");
    TEST_ASSERT(concert.getAvailableSeats() == 10, "All 10 seats remain available");
}

void testSuccessfulCancellation() {
    BookingSystem system(false);
    Concert concert("C104", "Opera Gala", "Opera House", "2026-12-10", 150.0, 10);
    system.addConcert(&concert);

    string bId = system.createBooking("Diana", &concert, 4);
    TEST_ASSERT(!bId.empty(), "Booking seat 4 succeeded");
    TEST_ASSERT(!concert.isSeatAvailable(4), "Seat 4 is booked");

    bool cancelled = system.cancelBooking(bId);
    TEST_ASSERT(cancelled == true, "Cancellation must return true");
    TEST_ASSERT(concert.isSeatAvailable(4), "Seat 4 must be available after cancellation");
    TEST_ASSERT(concert.getAvailableSeats() == 10, "Available seats restored to 10");

    Booking* b = system.findBooking(bId);
    TEST_ASSERT(b != nullptr, "Booking record persists");
    TEST_ASSERT(b->getStatus() == "CANCELLED", "Booking state updated to CANCELLED");
}

void testRepeatedCancellation() {
    BookingSystem system(false);
    Concert concert("C105", "Metal Fest", "Open Ground", "2026-12-12", 90.0, 10);
    system.addConcert(&concert);

    string bId = system.createBooking("Evan", &concert, 2);
    TEST_ASSERT(!bId.empty(), "Booking created");

    bool firstCancel = system.cancelBooking(bId);
    TEST_ASSERT(firstCancel == true, "First cancellation succeeds");

    bool secondCancel = system.cancelBooking(bId);
    TEST_ASSERT(secondCancel == false, "Second cancellation attempt must return false");

    Booking* b = system.findBooking(bId);
    TEST_ASSERT(b->getStatus() == "CANCELLED", "Status remains CANCELLED");
}

void testRegularPricing() {
    RegularBookingPolicy policy;
    TEST_ASSERT(policy.getPolicyName() == "Regular", "Policy name is Regular");
    double price = policy.calculatePrice(100.0);
    TEST_ASSERT(abs(price - 100.0) < 1e-5, "Regular price equals base price $100.00");
}

void testStudentPricing() {
    StudentBookingPolicy defaultStudentPolicy;
    double price20 = defaultStudentPolicy.calculatePrice(100.0);
    TEST_ASSERT(abs(price20 - 80.0) < 1e-5, "Student 20% discount yields $80.00 from $100.00");

    StudentBookingPolicy customStudentPolicy(0.35);
    double price35 = customStudentPolicy.calculatePrice(100.0);
    TEST_ASSERT(abs(price35 - 65.0) < 1e-5, "Student 35% discount yields $65.00 from $100.00");
}

void testPremiumPricing() {
    PremiumBookingPolicy defaultPremiumPolicy;
    double price20 = defaultPremiumPolicy.calculatePrice(100.0);
    TEST_ASSERT(abs(price20 - 120.0) < 1e-5, "Premium 20% surcharge yields $120.00 from $100.00");

    PremiumBookingPolicy customPremiumPolicy(0.50);
    double price50 = customPremiumPolicy.calculatePrice(100.0);
    TEST_ASSERT(abs(price50 - 150.0) < 1e-5, "Premium 50% surcharge yields $150.00 from $100.00");
}

void testValidStateTransitions() {
    Concert concert("C106", "Jazz Cafe", "Club B", "2026-12-15", 60.0, 5);
    
    Booking booking("B-TEST-1", "Frank", &concert, 1, 60.0,
                    make_shared<RegularBookingPolicy>(),
                    make_unique<PendingState>());

    TEST_ASSERT(booking.getStatus() == "PENDING", "State starts as PENDING");

    bool confirmed = booking.confirm();
    TEST_ASSERT(confirmed == true, "Pending -> Confirmed transition succeeds");
    TEST_ASSERT(booking.getStatus() == "CONFIRMED", "State is now CONFIRMED");

    bool cancelled = booking.cancel();
    TEST_ASSERT(cancelled == true, "Confirmed -> Cancelled transition succeeds");
    TEST_ASSERT(booking.getStatus() == "CANCELLED", "State is now CANCELLED");
}

void testInvalidStateTransitions() {
    Concert concert("C107", "Classical Eve", "Hall C", "2026-12-16", 70.0, 5);

    Booking confirmedBooking("B-TEST-2", "Grace", &concert, 1, 70.0,
                             make_shared<RegularBookingPolicy>(),
                             make_unique<ConfirmedState>());
    TEST_ASSERT(confirmedBooking.getStatus() == "CONFIRMED", "State is CONFIRMED");
    bool reconfirm = confirmedBooking.confirm();
    TEST_ASSERT(reconfirm == false, "Re-confirming ConfirmedState must be rejected");
    TEST_ASSERT(confirmedBooking.getStatus() == "CONFIRMED", "State remains CONFIRMED");

    Booking cancelledBooking("B-TEST-3", "Heidi", &concert, 2, 70.0,
                             make_shared<RegularBookingPolicy>(),
                             make_unique<CancelledState>());
    TEST_ASSERT(cancelledBooking.getStatus() == "CANCELLED", "State is CANCELLED");
    bool resurrect = cancelledBooking.confirm();
    TEST_ASSERT(resurrect == false, "Confirming CancelledState must be rejected");
    TEST_ASSERT(cancelledBooking.getStatus() == "CANCELLED", "State remains CANCELLED");

    bool recancel = cancelledBooking.cancel();
    TEST_ASSERT(recancel == false, "Cancelling CancelledState must be rejected");
    TEST_ASSERT(cancelledBooking.getStatus() == "CANCELLED", "State remains CANCELLED");
}

void testConcurrentSameSeatBooking() {
    BookingSystem system(false);
    Concert concert("C108", "High Demand Gig", "Mega Stadium", "2026-12-20", 200.0, 50);
    system.addConcert(&concert);

    const int targetSeat = 15;
    const int numThreads = 20;

    vector<thread> threads;
    threads.reserve(numThreads);

    atomic<bool> ready{false};
    atomic<int> successCount{0};
    atomic<int> failCount{0};
    vector<string> bookedIds(numThreads);

    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back([&, i]() {
            while (!ready.load(memory_order_acquire)) {
                this_thread::yield();
            }

            string user = "User_" + to_string(i);
            string bId = system.createBooking(user, &concert, targetSeat);
            if (!bId.empty()) {
                successCount.fetch_add(1, memory_order_relaxed);
                bookedIds[i] = bId;
            } else {
                failCount.fetch_add(1, memory_order_relaxed);
            }
        });
    }

    ready.store(true, memory_order_release);

    for (auto& th : threads) {
        th.join();
    }

    TEST_ASSERT(successCount.load() == 1, "Exactly ONE thread successfully booked seat 15");
    TEST_ASSERT(failCount.load() == (numThreads - 1), "All other threads failed to double-book");
    TEST_ASSERT(!concert.isSeatAvailable(targetSeat), "Target seat 15 is marked booked");
    TEST_ASSERT(concert.getAvailableSeats() == 49, "Exactly 1 seat was booked out of 50");
}

void testConcurrentDifferentSeatsBooking() {
    BookingSystem system(false);
    const int totalSeats = 40;
    const int numThreads = 30;
    Concert concert("C109", "Multi-Seat Gala", "Grand Arena", "2026-12-25", 150.0, totalSeats);
    system.addConcert(&concert);

    vector<thread> threads;
    threads.reserve(numThreads);

    atomic<bool> ready{false};
    atomic<int> successCount{0};
    atomic<int> failCount{0};

    for (int i = 0; i < numThreads; i++) {
        int seatNum = i + 1;
        threads.emplace_back([&, seatNum, i]() {
            while (!ready.load(memory_order_acquire)) {
                this_thread::yield();
            }

            string user = "Customer_" + to_string(i);
            string bId = system.createBooking(user, &concert, seatNum);
            if (!bId.empty()) {
                successCount.fetch_add(1, memory_order_relaxed);
            } else {
                failCount.fetch_add(1, memory_order_relaxed);
            }
        });
    }

    ready.store(true, memory_order_release);

    for (auto& th : threads) {
        th.join();
    }

    TEST_ASSERT(successCount.load() == numThreads, "All 30 concurrent bookings for distinct seats succeeded");
    TEST_ASSERT(failCount.load() == 0, "Zero failures when booking non-conflicting seats");
    TEST_ASSERT(concert.getAvailableSeats() == (totalSeats - numThreads), "Remaining seats is exactly 10 (40 - 30)");
}

int main() {
    cout << "==================================================" << endl;
    cout << "  CONCERT BOOKING SYSTEM - COMPREHENSIVE TEST SUITE" << endl;
    cout << "==================================================" << endl;

    RUN_TEST(testConcertCreation);
    RUN_TEST(testValidSeatBooking);
    RUN_TEST(testBookAlreadyBookedSeat);
    RUN_TEST(testInvalidSeatSelection);
    RUN_TEST(testSuccessfulCancellation);
    RUN_TEST(testRepeatedCancellation);
    RUN_TEST(testRegularPricing);
    RUN_TEST(testStudentPricing);
    RUN_TEST(testPremiumPricing);
    RUN_TEST(testValidStateTransitions);
    RUN_TEST(testInvalidStateTransitions);
    RUN_TEST(testConcurrentSameSeatBooking);
    RUN_TEST(testConcurrentDifferentSeatsBooking);

    cout << "==================================================" << endl;
    cout << "Test Summary:" << endl;
    cout << "  Assertions Passed: " << g_tests_passed << endl;
    cout << "  Assertions Failed: " << g_tests_failed << endl;
    cout << "==================================================" << endl;

    if (g_tests_failed == 0) {
        cout << ">>> ALL TESTS PASSED SUCCESSFULLY! <<<" << endl;
        return 0;
    } else {
        cerr << ">>> SOME TESTS FAILED! <<<" << endl;
        return 1;
    }
}
