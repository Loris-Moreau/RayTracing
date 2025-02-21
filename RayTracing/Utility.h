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

// AVX Dot Product
#include <immintrin.h>
#include <cmath>

// Dot product of two __m256d vectors (each storing x,y,z,0)
inline double dotAVX(const __m256d &a, const __m256d &b)
{
	// Multiply element-wise
	__m256d product = _mm256_mul_pd(a, b);

	// Store to a temporary array
	alignas(32) double temp[4];
	_mm256_store_pd(temp, product);

	// Sum up x + y + z (the 4th is 0)
	return temp[0] + temp[1] + temp[2];
}

// Cross product of two __m256d vectors (each storing x,y,z,0)
// Returns __m256d with (x,y,z,0) in that order
inline __m256d crossAVX(const __m256d &a, const __m256d &b)
{
	alignas(32) double A[4], B[4];
	_mm256_store_pd(A, a);  // A[0]=ax, A[1]=ay, A[2]=az, A[3]=0
	_mm256_store_pd(B, b);  // B[0]=bx, B[1]=by, B[2]=bz, B[3]=0

	// Cross(a,b) = (ay*bz - az*by, az*bx - ax*bz, ax*by - ay*bx)
	double cx = A[1]*B[2] - A[2]*B[1];
	double cy = A[2]*B[0] - A[0]*B[2];
	double cz = A[0]*B[1] - A[1]*B[0];

	return _mm256_set_pd(0.0, cz, cy, cx);
}


// End of AVX Dot Product