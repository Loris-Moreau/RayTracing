#pragma once

#include "Hittable.h"
#include "Material.h"

class Metal final : public Material
{
 public:
    Metal(const Vector3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override
    {
        const Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray{rec.p, reflected + fuzz * random_in_unit_sphere()};
        attenuation = albedo;

        return dot(scattered.direction(), rec.normal) > 0;
    }

    Vector3 albedo;
    double fuzz;
};

