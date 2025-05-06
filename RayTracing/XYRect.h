#pragma once

#include <utility>

#include "Hittable.h"

class XYRect final : public Hittable
{
 public:
    XYRect() = default;
    XYRect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<Material> mat)
        : mp(std::move(mat)), x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k) {}

    bool hit(const Ray& r, double t0, double t1, HitRecord& rec) const override;
    bool bounding_box([[maybe_unused]] double t0, [[maybe_unused]] double t1, AABB& output_box) const override
    {
        // The bounding Box must have non-zero width in each dimension, so pad the Z dimension a very small amount.
        output_box = AABB(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
        
        return true;
    }

    std::shared_ptr<Material> mp;
    double x0 = 0.0, x1 = 0.0, y0 = 0.0, y1 = 0.0, k = 0.0;
};

inline bool XYRect::hit(const Ray& r, double t0, double t1, HitRecord& rec) const
{
    const double t = (k - r.origin().z()) / r.direction().z();
    if (t < t0 || t > t1)
    {
        return false;
    }

    const double x = r.origin().x() + t * r.direction().x();
    const double y = r.origin().y() + t * r.direction().y();

    if (x < x0 || x > x1 || y < y0 || y > y1)
    {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;

    const Vector3 outward_normal = Vector3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}
