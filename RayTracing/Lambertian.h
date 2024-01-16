#pragma once
#include "Materials.h"

class Lambertian : public Materials
{
public:
    Lambertian(const Color& a) : albedo(a) {}

    bool scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const override
    {
        Vector3 scatterDirection = hitInfo.normal + RandomUnitVector();
        scattered = Ray(hitInfo.coordinates, scatterDirection);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};