#pragma once

#include "Color.h"
#include "Ray.h"

class HitInfo;

class Materials
{
public:
    virtual ~Materials() = default;

    virtual bool scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const = 0;
};