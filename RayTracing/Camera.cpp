#include "Camera.h"

Camera::Camera(Vector3 lookfrom, Vector3 lookat, Vector3 vup, double vfov, double aspect, double aperture, double focus_dist, double t0, double t1)
{
    origin = lookfrom;
    lens_radius = aperture / 2;
    time0 = t0;
    time1 = t1;

    const double theta = degrees_to_radians(vfov);
    const double half_height = tan(theta / 2);
    const double half_width = aspect * half_height;

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);
    
    lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;

    horizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;
}

Ray Camera::get_ray(double s, double t) const
{
    const Vector3 rd = lens_radius * random_in_unit_disk();
    const Vector3 offset = u * rd.x() + v * rd.y();

    return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random_double(time0, time1));
}