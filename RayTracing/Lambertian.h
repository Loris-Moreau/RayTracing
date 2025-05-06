#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class Lambertian final : public Material
{
 public:
    Lambertian(std::shared_ptr<Texture> a) : albedo(a) {}

    bool scatter([[maybe_unused]] const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override
    {
        const Vector3 scatter_direction = rec.normal + random_unit_vector();
        scattered = Ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);

        return true;
    }

    std::shared_ptr<Texture> albedo;
};
