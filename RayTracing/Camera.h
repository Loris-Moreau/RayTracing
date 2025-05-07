#pragma once

#include "Common.h"
#include "Ray.h"
#include "Vector3.h"

class Camera
{
 public:
    // vfov: top to bottom, in degrees
    Camera(Vector3 lookfrom, Vector3 lookat, Vector3 vup, double vfov, double aspect, double aperture, double focus_dist, double t0 = 0.0, double t1 = 0.0);

    Ray get_ray(double s, double t) const;

    Vector3 origin;
    Vector3 lower_left_corner;
    
    Vector3 horizontal;
    Vector3 vertical;
    
    Vector3 u, v, w;
    
    double lens_radius;
    
    // shutter open/close times
    double time0, time1;
};

