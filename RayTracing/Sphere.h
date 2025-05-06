#pragma once

#include "Hittable.h"
#include "Vector3.h"

#include <utility>

void get_sphere_uv(const Vector3& p, double& u, double& v);

class Sphere final : public Hittable
{
 public:
    Sphere() = default;
    Sphere(Vector3 cen, double r, std::shared_ptr<Material> m)
        : center(cen), radius(r), mat_ptr(std::move(m)) {}

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& output_box) const override;

    Vector3 center;
    double radius{0.0};
    std::shared_ptr<Material> mat_ptr;
};

inline bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    const Vector3 oc = r.origin() - center;
    const double a = r.direction().length_squared();
    const double half_b = dot(oc, r.direction());
    const double c = oc.length_squared() - radius * radius;
    const double discriminant = half_b * half_b - a * c;

    if (discriminant > 0)
    {
        const double root = sqrt(discriminant);
        double temp = (-half_b - root) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.at(rec.t);

            const Vector3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;

            return true;
        }

        temp = (-half_b + root) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.at(rec.t);

            const Vector3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;

            return true;
        }
    }

    return false;
}

inline bool Sphere::bounding_box([[maybe_unused]] double t0, [[maybe_unused]] double t1, AABB& output_box) const
{
    output_box = AABB(center - Vector3(radius, radius, radius),
                      center + Vector3(radius, radius, radius));

    return true;
}

inline void get_sphere_uv(const Vector3& p, double& u, double& v)
{
    const double phi = atan2(p.z(), p.x());
    const double theta = asin(p.y());
    
    u = 1 - (phi + pi) / (2 * pi);
    v = (theta + pi / 2) / pi;
}
