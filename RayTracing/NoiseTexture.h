#pragma once

#include "Perlin.h"
#include "Texture.h"

class NoiseTexture final : public Texture
{
 public:
    NoiseTexture() = default;
    NoiseTexture(double sc) : scale(sc) {}

    color value([[maybe_unused]] double u, [[maybe_unused]] double v, const point3& p) const override
    {
        return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
    }

    Perlin noise;
    double scale = 0.0;
};
