#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

inline double DegToRad(const double degrees)
{
	return degrees * pi / 180.0;
}

inline double RandomDouble()
{
	//Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(const double min, const double max)
{
	//Returns a random real in [min,max).
	return min + (max - min) * RandomDouble();
}
