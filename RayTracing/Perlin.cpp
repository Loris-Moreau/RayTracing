#include "Perlin.h"

Perlin::Perlin()
{
    ranvec = new Vector3[pointCount];
    for (int i = 0; i < pointCount; ++i) 
    {
        ranvec[i] = Unit(Vector3::Random(-1, 1));
    }

    permX = PerlinGeneratePerm();
    permY = PerlinGeneratePerm();
    permZ = PerlinGeneratePerm();
}

Perlin::~Perlin()
{
    delete[] ranvec;
    delete[] permX;
    delete[] permY;
    delete[] permZ;
}

double Perlin::noise(const Position& position) const
{
    double U = position.x - floor(position.x);
    double V = position.y - floor(position.y);
    double W = position.z - floor(position.z);

    /*
    //Hermitian Smoothing
    U = U * U * (3 - 2 * U);
    V = V* V * (3 - 2 * V);
    W = W * W * (3 - 2 * W);
    */

    const int i = static_cast<int>(floor(position.x));
    const int j = static_cast<int>(floor(position.y));
    const int k = static_cast<int>(floor(position.z));
    Vector3 c[2][2][2];

    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] = ranvec
                [
                    permX[(i + di) & 255] ^
                    permY[(j + dj) & 255] ^
                    permZ[(k + dk) & 255]
                ];

    return PerlinInterp(c, U, V, W);
}

double Perlin::Turbulence(const Position& position, const int depth) const
{
    double accum = 0.0;
    double weight = 1.0; 
    Position tempP = position;

    for (int i = 0; i < depth; i++) 
    {
        accum += weight * noise(tempP);
        weight *= 0.5;
        tempP *= 2;
    }

    return fabs(accum);
}

int* Perlin::PerlinGeneratePerm()
{
    int* point = new int[pointCount];

    for (int i = 0; i < Perlin::pointCount; i++)
    {
        point[i] = i;
    }
    Permute(point, pointCount);

    return point;
}

void Perlin::Permute(int* point, const int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        const int target = RandomInt(0, i);
        const int tmp = point[i];
        point[i] = point[target];
        point[target] = tmp;
    }
}

double Perlin::TrilinearInterp(double c[2][2][2], const double U, const double V, const double W)
{
    double accum = 0.0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accum += (i * U + (1 - i) * (1 - U)) *
                (j * V + (1 - j) * (1 - V)) *
                (k * W + (1 - k) * (1 - W)) * c[i][j][k];

    return accum;
}

double Perlin::PerlinInterp(Vector3 c[2][2][2], const double U, const double V, const double W)
{
    const double uu = U * U * (3 - 2 * U);
    const double vv = V * V * (3 - 2 * V);
    const double ww = W * W * (3 - 2 * W);
    double accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                const Vector3 weight_v(U - i, V - j, W - k);
                accum += (i * uu + (1 - i) * (1 - uu))
                    * (j * vv + (1 - j) * (1 - vv))
                    * (k * ww + (1 - k) * (1 - ww))
                    * Dot(c[i][j][k], weight_v);
            }

    return accum;
}
