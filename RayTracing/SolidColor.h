#pragma once

#include "Texture.h"

class SolidColor final : public Texture
{
 public:
    SolidColor() = default;
    SolidColor(color c) : color_value(c) {}
    SolidColor(double red, double green, double blue) : SolidColor(color(red, green, blue)) {}

    color value([[maybe_unused]] double u, [[maybe_unused]] double v, [[maybe_unused]] const point3& p) const override
    {
        return color_value;
    }

 private:
    color color_value;
};
