#pragma once
#include "Materials.h"

class Dielectric : public Materials
{
public:
    Dielectric(double refractionIndex) : indexOfRefraction(refractionIndex) {}

    bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const override;

    static double Reflectance(double cosine, double reflectanceIndex);

private:
    double indexOfRefraction; // Index of Refraction
};
