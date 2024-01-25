#include "DiffuseLight.h"

bool DiffuseLight::Scatter(const Ray& r_in, const HitInfo& hitInfo, Color& attenuation, Ray& scattered, double& pdf) const
{
    return false;
}

Color DiffuseLight::Emitted(double U, double V, const Position& position) const
{
    return emit->Value(U, V, position);
}

Color DiffuseLight::Emitted(const Ray& rayIn, const HitInfo hitInfo, double U, double V, const Position& position)
{
    if (!hitInfo.frontFace)
    {
        return Color(0, 0, 0);
    }
    return emit->Value(U, V, position);
}
