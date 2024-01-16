#pragma once

#include "Materials.h"

class Lambertian : public Materials
{
public:
    Lambertian(const Color& a) : albedo(a) {}

    bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const override;

private:
    Color albedo;
};
