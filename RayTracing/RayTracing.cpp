// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

//#include <iostream>
#include "Camera.h"
#include "HittableCollection.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"

using namespace std;

int main(int argc, char* argv[])
{
	//World
	HittableCollection world;

	shared_ptr<Materials> groundMat = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	shared_ptr<Materials> centerMat = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	shared_ptr<Materials> leftMat = make_shared<Metal>(Color(0.8, 0.8, 0.8));
	shared_ptr<Materials> rightMat = make_shared<Metal>(Color(0.8, 0.6, 0.2));

	world.Add(make_shared<Sphere>(Position(0, -100.5, -1), 100, groundMat));
	world.Add(make_shared<Sphere>(Position(0, 0, -1), 0.5, centerMat));
	world.Add(make_shared<Sphere>(Position(-1.0, 0, -1), 0.5, leftMat));
	world.Add(make_shared<Sphere>(Position(1.0, 0, -1), 0.5, rightMat));


	Camera camera(400, 16.0 / 9.0, 100, 50);
	camera.Render(world);

	return 0;
}
