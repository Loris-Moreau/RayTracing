#pragma once

#include "Hittable.h"
#include "Material.h"

class Dielectric final : public Material
{
 public:
    Dielectric(double ri) : ref_idx(ri) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override
    {
        attenuation = Vector3{1.0, 1.0, 1.0};
        const double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;

        const Vector3 unit_direction = unit_vector(r_in.direction());
        const double cos_theta = ffmin(dot(-unit_direction, rec.normal), 1.0);
        const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        if (etai_over_etat * sin_theta > 1.0)
        {
            const Vector3 reflected = reflect(unit_direction, rec.normal);
            scattered = Ray{rec.p, reflected};

            return true;
        }

        const double reflect_prob = schlick(cos_theta, etai_over_etat);
        if (random_double() < reflect_prob)
        {
            const Vector3 reflected = reflect(unit_direction, rec.normal);
            scattered = Ray{rec.p, reflected};

            return true;
        }

        const Vector3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
        scattered = Ray{rec.p, refracted};

        return true;
    }

    double ref_idx;
};

