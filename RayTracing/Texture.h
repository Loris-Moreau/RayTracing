#pragma once

#include "Common.h"

class Texture
{
 public:
    virtual ~Texture() = default;

    virtual color value(double u, double v, const point3& p) const = 0;
};

