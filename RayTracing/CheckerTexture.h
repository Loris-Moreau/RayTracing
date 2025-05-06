#pragma once

#include <utility>

#include "Texture.h"

class CheckerTexture final : public Texture
{
 public:
    CheckerTexture() = default;
    CheckerTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1)
        : odd(std::move(t1)), even(std::move(t0)) {}

    color value(double u, double v, const point3& p) const override
    {
        const double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());

        if (sines < 0)
        {
            return odd->value(u, v, p);
        }
        else
        {
            return even->value(u, v, p);
        }
    }

    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

