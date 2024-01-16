// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

//#include <iostream>
#include "Camera.h"
#include "HittableCollection.h"
#include "Sphere.h"

using namespace std;

int main(int argc, char* argv[])
{
	//World
	HittableCollection world;
	world.Add(make_shared<Sphere>(Position(0, 0, -1), 0.5));
	world.Add(make_shared<Sphere>(Position(0, -100.5, -1), 100));

	Camera camera(400, 16.0 / 9.0, 100);
	camera.Render(world);

	return 0;
}
