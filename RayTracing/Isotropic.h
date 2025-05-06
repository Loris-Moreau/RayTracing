#pragma once

#include <utility>

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class Isotropic final : public Material
{
 public:
    Isotropic(std::shared_ptr<Texture> a) : albedo(std::move(a)) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override
    {
        scattered = Ray{rec.p, random_in_unit_sphere(), r_in.time()};
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        
        return true;
    }

    std::shared_ptr<Texture> albedo;
};
