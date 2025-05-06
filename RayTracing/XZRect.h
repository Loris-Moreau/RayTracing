#pragma once

#include <utility>

#include "Hittable.h"

class XZRect final : public Hittable
{
 public:
    XZRect() = default;
    XZRect(double _x0, double _x1, double _z0, double _z1, double _k, std::shared_ptr<Material> mat)
        : mp(std::move(mat)), x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k) {}
    
    bool hit(const Ray& r, double t0, double t1, HitRecord& rec) const override;
    bool bounding_box([[maybe_unused]] double t0, [[maybe_unused]] double t1, AABB& output_box) const override
    {
        // The bounding Box must have non-zero width in each dimension, so pad the Y dimension a very small amount.
        output_box = AABB(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
        
        return true;
    }

    std::shared_ptr<Material> mp;
    double x0 = 0.0, x1 = 0.0, z0 = 0.0, z1 = 0.0, k = 0.0;
};

inline bool XZRect::hit(const Ray& r, double t0, double t1, HitRecord& rec) const
{
    const double t = (k - r.origin().y()) / r.direction().y();
    if (t < t0 || t > t1)
    {
        return false;
    }

    const double x = r.origin().x() + t * r.direction().x();
    const double z = r.origin().z() + t * r.direction().z();

    if (x < x0 || x > x1 || z < z0 || z > z1)
    {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;

    const Vector3 outward_normal = Vector3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}
