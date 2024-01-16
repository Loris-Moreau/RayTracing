#pragma once
#include "Materials.h"

class Dielectric : public Materials
{
public:
    Dielectric(double refractionIndex) : indexOfRefraction(refractionIndex) {}

    bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const override;

private:
    double indexOfRefraction; // Index of Refraction
};
