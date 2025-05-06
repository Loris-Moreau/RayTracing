#pragma once

#include "Common.h"

struct HitRecord;

class Material
{
 public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const = 0;

    virtual color emitted([[maybe_unused]] double u, [[maybe_unused]] double v, [[maybe_unused]] const point3& p) const
    {
        return color(0, 0, 0);
    }
};

inline double schlick(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
