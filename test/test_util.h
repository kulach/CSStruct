#pragma once

#include <cmath>

const double FEQ_EPS = .001;

inline bool feq(double a, double b, double eps) {
    return std::abs(a - b) <= eps;
}

inline bool feq(double a, double b) {
    return feq(a, b, FEQ_EPS);
}

