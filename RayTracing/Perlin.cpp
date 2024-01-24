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
    double u = position.x - floor(position.x);
    double v = position.y - floor(position.y);
    double w = position.z - floor(position.z);

    /*
    //Hermitian Smoothing
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);
    */

    int i = static_cast<int>(floor(position.x));
    int j = static_cast<int>(floor(position.y));
    int k = static_cast<int>(floor(position.z));
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

    return PerlinInterp(c, u, v, w);
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

void Perlin::Permute(int* point, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int target = RandomInt(0, i);
        int tmp = point[i];
        point[i] = point[target];
        point[target] = tmp;
    }
}

double Perlin::TrilinearInterp(double c[2][2][2], double u, double v, double w)
{
    double accum = 0.0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accum += (i * u + (1 - i) * (1 - u)) *
                (j * v + (1 - j) * (1 - v)) *
                (k * w + (1 - k) * (1 - w)) * c[i][j][k];

    return accum;
}

double Perlin::PerlinInterp(Vector3 c[2][2][2], double u, double v, double w)
{
    double uu = u * u * (3 - 2 * u);
    double vv = v * v * (3 - 2 * v);
    double ww = w * w * (3 - 2 * w);
    double accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                Vector3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu))
                    * (j * vv + (1 - j) * (1 - vv))
                    * (k * ww + (1 - k) * (1 - ww))
                    * Dot(c[i][j][k], weight_v);
            }

    return accum;
}
