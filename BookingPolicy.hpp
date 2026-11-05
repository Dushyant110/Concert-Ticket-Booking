#ifndef BOOKING_POLICY_HPP
#define BOOKING_POLICY_HPP

#include <string>
#include <memory>

using namespace std;

class BookingPolicy {
public:
    virtual ~BookingPolicy() = default;
    virtual double calculatePrice(double basePrice) const = 0;
    virtual string getPolicyName() const = 0;
};

class RegularBookingPolicy : public BookingPolicy {
public:
    double calculatePrice(double basePrice) const override;
    string getPolicyName() const override;
};

class StudentBookingPolicy : public BookingPolicy {
private:
    double discountPercentage;

public:
    explicit StudentBookingPolicy(double discountPercentage = 0.20);
    double calculatePrice(double basePrice) const override;
    string getPolicyName() const override;
};

class PremiumBookingPolicy : public BookingPolicy {
private:
    double surchargePercentage;

public:
    explicit PremiumBookingPolicy(double surchargePercentage = 0.20);
    double calculatePrice(double basePrice) const override;
    string getPolicyName() const override;
};

#endif
