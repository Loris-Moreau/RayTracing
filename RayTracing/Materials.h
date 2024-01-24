#pragma once

#include "Color.h"
#include "Hittable.h"
#include "Texture.h"

class HitInfo;

class Materials
{
public:
    virtual ~Materials() = default;

    virtual Color Emitted(double u, double v, const Position& p) const;

    virtual bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const = 0;
};
