#pragma once

#include <functional>
#include <limits>
#include <random>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// Utility functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180;
}

inline double ffmin(double a, double b)
{
    return a <= b ? a : b;
}

inline double ffmax(double a, double b)
{
    return a >= b ? a : b;
}

inline double random_double()
{
    // Returns a random real in [0, 1).
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);

    return rand_generator();
}

inline double random_double(double min, double max)
{
    // Returns a random real in [min, max).
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max + 1));
}

// Common headers
#include "Ray.h"
#include "Vector3.h"
