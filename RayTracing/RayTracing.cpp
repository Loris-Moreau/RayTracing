// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "Color.h"
#include "Ray.h"
#include "HittableCollection.h"
#include "Sphere.h"
#include "Utility.h"

using namespace std;

Color RayColor(const Ray& rRay, const Hittable& rWorld)
{
    HitInfo hitInfo;
    if (rWorld.Hit(rRay, Interval(0, infinity), hitInfo)) {
        return 0.5 * (hitInfo.normal + Color(1, 1, 1));
    }
    Vector3 unitDirection = Unit(rRay.GetDirection());
    double blue = 0.5 * (unitDirection.y + 1.0);
    return (1.0 - blue) * Color(1.0, 1.0, 1.0) + blue * Color(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[])
{
    //Resolution
    double resolution = 16.0 / 9.0;
    int width = 400, height = static_cast<int>(width / resolution);
    if (height < 1) height = 1;

    //Viewport
    double viewportHeight = 2;
    double viewportWidth = viewportHeight * (static_cast<double>(width) / height);
    double focalLength = 1;
    Position cameraCenter = Position(0, 0, 0);

    Vector3 viewportX = Vector3(viewportWidth, 0, 0);
    Vector3 viewportY = Vector3(0, -viewportHeight, 0); //We invert Y

    //Delta vector between pixels
    Vector3 pixelDeltaX = viewportX / width;
    Vector3 pixelDeltaY = viewportY / height;

    //Position of the top left pixel
    Vector3 viewportOrigin = cameraCenter - Vector3(0, 0, focalLength) - viewportX / 2 - viewportY / 2;

    Vector3 originPixelLocation = viewportOrigin + 0.5 * (pixelDeltaX + pixelDeltaY);

    //World
    HittableCollection world;
    world.Add(make_shared<Sphere>(Position(0, 0, -1), 0.5));
    world.Add(make_shared<Sphere>(Position(0, -100.5, 0), 100));

    //Image
    cout << "P3\n" << width << ' ' << height << "\n255\n";

    for (int y = 0; y < height; y++)
    {
        clog << "Progress : " << (y * 100 / height) << " %\n" << flush;

        for (int x = 0; x < width; x++)
        {
            Position pixelCenter = originPixelLocation + (x * pixelDeltaX) + (y * pixelDeltaY);
            Vector3 direction = pixelCenter - cameraCenter;
            Ray ray(cameraCenter, direction);
            Color pixel = RayColor(ray, world);
            WriteColor(cout, pixel);
        }
    }

    clog << "It has been done \n";

    return 0;
}
