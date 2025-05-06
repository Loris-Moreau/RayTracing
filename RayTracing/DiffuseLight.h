#pragma once

#include <utility>

#include "Material.h"
#include "Texture.h"

class DiffuseLight final : public Material
{
 public:
    DiffuseLight(std::shared_ptr<Texture> a) : emit(std::move(a)) {}

    bool scatter([[maybe_unused]] const Ray& r_in, [[maybe_unused]] const HitRecord& rec, [[maybe_unused]] Vector3& attenuation, [[maybe_unused]] Ray& scattered) const override
    {
        return false;
    }

    color emitted(double u, double v, const point3& p) const override
    {
        return emit->value(u, v, p);
    }

    std::shared_ptr<Texture> emit;
};
