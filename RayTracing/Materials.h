#pragma once

#include "Color.h"

class HitInfo;

class Materials
{
public:
    virtual ~Materials() = default;

    virtual bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const = 0;
};