// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "Color.h"
#include "Ray.h"
#include "HittableCollection.h"
#include "Sphere.h"
#include "Utility.h"
#include "Camera.h"

using namespace std;

int main(int argc, char* argv[])
{
    //World
    HittableCollection world;
    world.Add(make_shared<Sphere>(Position(0, 0, -1), 0.5));
    world.Add(make_shared<Sphere>(Position(0, -100.5, 0), 100));

    //Rendering
    Camera cam;
    cam.resolution = 16.0 / 9.0;
    cam.width = 400;
    cam.render(world);

    return 0;
}
