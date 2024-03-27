#pragma once

#include "Interval.h"

// New Vector3 alias for color
using Color = Vector3;

inline double LinearToGamma(const double linearComponent)
{
    return sqrt(linearComponent);
}

inline void WriteColor(std::ostream& out, const Color& pixel, const int sampleCount)
{
    const double scale = 1.0 / sampleCount;
    double r = pixel.x * scale;
    double g = pixel.y * scale;
    double b = pixel.z * scale;

    // Apply the linear to gamma transform.
    r = LinearToGamma(r);
    g = LinearToGamma(g);
    b = LinearToGamma(b);

    // Write the translated [0,255] value of each color component.
    static const Interval intensity(0.000, 0.999);
    out << static_cast<int>(255.999 * intensity.Clamp(r)) << ' '
        << static_cast<int>(255.999 * intensity.Clamp(g)) << ' '
        << static_cast<int>(255.999 * intensity.Clamp(b)) << '\n';
}
