#include "BookingPolicy.hpp"

using namespace std;

double RegularBookingPolicy::calculatePrice(double basePrice) const {
    return basePrice;
}

string RegularBookingPolicy::getPolicyName() const {
    return "Regular";
}

StudentBookingPolicy::StudentBookingPolicy(double discountPercentage)
    : discountPercentage(discountPercentage) {}

double StudentBookingPolicy::calculatePrice(double basePrice) const {
    return basePrice * (1.0 - discountPercentage);
}

string StudentBookingPolicy::getPolicyName() const {
    return "Student (" + to_string(static_cast<int>(discountPercentage * 100)) + "% discount)";
}

PremiumBookingPolicy::PremiumBookingPolicy(double surchargePercentage)
    : surchargePercentage(surchargePercentage) {}

double PremiumBookingPolicy::calculatePrice(double basePrice) const {
    return basePrice * (1.0 + surchargePercentage);
}

string PremiumBookingPolicy::getPolicyName() const {
    return "Premium (" + to_string(static_cast<int>(surchargePercentage * 100)) + "% surcharge)";
}
