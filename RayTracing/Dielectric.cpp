#include "Dielectric.h"

bool Dielectric::Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const
{
    attenuation = Color(1.0, 1.0, 1.0);

    double refraction_ratio = hitInfo.frontFace ? (1.0 / indexOfRefraction) : indexOfRefraction;

    Vector3 unit_direction = Unit(rayIn.GetDirection());

    double cos_theta = fmin(Dot(-unit_direction, hitInfo.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    Vector3 direction;

    if (cannot_refract)
    {
        direction = Reflect(unit_direction, hitInfo.normal);
    }
    else
    {
        direction = Refract(unit_direction, hitInfo.normal, refraction_ratio);
    }

    scattered = Ray(hitInfo.coordinates, direction);

    return true;
}
