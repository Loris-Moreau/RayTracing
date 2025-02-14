#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

// Random
#include <complex.h>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

inline double DegToRad(double degrees)
{
	return degrees * pi / 180.0;
}

/*inline unsigned int XOrShift32(unsigned int *state)
{
	unsigned int x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*state = x;
	return x;
}

//#include <limits.h>

//unsigned int my_random_state = 1234;

inline unsigned int ShiftRandomNumber(void)
{
	unsigned int r = 1234;//my_random_state;
	r ^= r << 13;
	r ^= r >> 17;
	r ^= r << 5;
	//my_random_state = r;
	return r;
}*/

inline double RandomDouble()
{
	// Thread safe Rand
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	
	std::uniform_real_distribution<> distrDouble(0, RAND_MAX); // No Range
	double randDouble = distrDouble(gen); // Generate random Double in range
	return randDouble / (RAND_MAX + 1.0);
	
	// old Rand
	// Returns a random real in [0,1).
	//return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max)
{
	// Returns a random real in [min,max).
	return min + (max - min) * RandomDouble();
}
