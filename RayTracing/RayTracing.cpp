// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

///Command for .ppm file creation :
///.\x64\Debug\RayTracing.exe > Render.ppm


#include "Camera.h"
#include "HittableCollection.h"
#include "Sphere.h"
//Mat
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

using namespace std;

int main(int argc, char* argv[])
{
	//World
	HittableCollection world;

	/*
	shared_ptr<Materials> groundMat = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	//shared_ptr<Materials> centerMat = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	//shared_ptr<Materials> leftMat = make_shared<Metal>(Color(0.8, 0.8, 0.8),0.085);
	shared_ptr<Materials> rightMat = make_shared<Metal>(Color(0.8, 0.6, 0.2),0.7);

	shared_ptr<Materials> centerMat = make_shared<Dielectric>(1.5);
	shared_ptr<Materials> leftMat = make_shared<Dielectric>(1.5);
	*/

	shared_ptr<Materials> groundMat = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	shared_ptr<Materials> centerMat = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	shared_ptr<Materials> leftMat = make_shared<Dielectric>(1.5);
	shared_ptr<Materials> rightMat = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

	world.Add(make_shared<Sphere>(Position(0, -100.5, -1), 100, groundMat));
	world.Add(make_shared<Sphere>(Position(0, 0, -1), 0.5, centerMat));
	//world.Add(make_shared<Sphere>(Position(-1.0, 0, -1), 0.5, leftMat)); //Reflective glass
	world.Add(make_shared<Sphere>(Position(1.0, 0, -1), 0.5, rightMat)); //shiny metal ball

	world.Add(make_shared<Sphere>(Position(-1.0, 0.0, -1.0), -0.4, leftMat)); //Transparent Glass

	//Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector)
	Camera camera(400, 16.0 / 9.0, 100, 50, 90, Position(-2, 2, 1), Position(0, 0, -1), Vector3(0, 1, 0));
	camera.Render(world);

	return 0;
}
