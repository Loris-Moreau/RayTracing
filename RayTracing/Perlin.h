#pragma once

#include "Utility.h"
#include "Vector3.h"

class Perlin
{
public:
    Perlin();

    ~Perlin();

    double noise(const Position& position) const;
    double Turbulence(const Position& position, int depth = 7) const;

private:
    static const int pointCount = 256;
    Vector3* ranvec;
    int* permX;
    int* permY;
    int* permZ;

    static int* PerlinGeneratePerm();

    static void Permute(int* point, int n);

    static double TrilinearInterp(double c[2][2][2], double u, double v, double w);
    static double PerlinInterp(Vector3 c[2][2][2], double u, double v, double w);
};