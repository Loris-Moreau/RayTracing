#pragma once

#include <iostream>

#include "Color.h"
#include "Ray.h"
#include "HittableCollection.h"
#include "Sphere.h"
#include "Utility.h"

class Camera
{
public:
	void render(const Hittable& world);

    //Resolution
    double resolution;
    int width;;
    int height;

    //Viewport
    double viewportHeight;
    double viewportWidth;
    double focalLength;
    Position cameraCenter;

    Vector3 viewportX;
    Vector3 viewportY;

    //Delta vector between pixels
    Vector3 pixelDeltaX;
    Vector3 pixelDeltaY;

    //Position of the top left pixel
    Vector3 viewportOrigin;

    Vector3 originPixelLocation;


private:
	void Initialise();

	Color rayColor(const Ray& ray, const Hittable& world)const;
};
