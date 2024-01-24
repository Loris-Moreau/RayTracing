#include "DiffuseLight.h"

bool DiffuseLight::Scatter(const Ray& r_in, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const
{
    return false;
}

Color DiffuseLight::Emitted(double u, double v, const Position& position) const
{
    return emit->Value(u, v, position);
}
