#pragma once

#include "Common.h"

inline double trilinear_interp(double c[2][2][2], double u, double v, double w)
{
    double accum = 0.0;

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                accum += (i * u + (1 - i) * (1 - u)) *
                         (j * v + (1 - j) * (1 - v)) *
                         (k * w + (1 - k) * (1 - w)) * c[i][j][k];
            }
        }
    }

    return accum;
}

class Perlin
{
 public:
    Perlin();
    ~Perlin();

    double noise(const point3& p) const;
    double turb(const point3& p, int depth = 7) const;

 private:
    static int* perlin_generate_perm();
    static void permute(int* p, int n);

    static double perlin_interp(Vector3 c[2][2][2], double u, double v, double w);

    static const int point_count = 256;
    Vector3* ranvec;
    int* perm_x;
    int* perm_y;
    int* perm_z;
};

