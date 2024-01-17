#pragma once

#include "Hittable.h"
#include "Materials.h"

using namespace std;

class Camera
{
public:
	Camera() = default;
	Camera(double imageWidth, double ratio, int samplePerPixel = 10, int bounces = 10, double fov = 90, 
			Position lookfrom = Position(0, 0, 0), Position lookat = Position(0, 0, 0), Vector3 upVector = Vector3(0, 1, 0))
		: aspectRatio(ratio), width(imageWidth), sampleCount(samplePerPixel), maxBounces(bounces), verticalFov(fov), lookFrom(lookfrom), lookAt(lookat), vecUp(upVector) {}

	void Render(const Hittable& rWorld);

	double verticalFov;						 //Vertical view angle (field of view)
	Position lookFrom = Position(0, 0, -1); //Point camera is looking from
	Position lookAt = Position(0, 0, 0);   //Point camera is looking at
	Vector3 vecUp = Vector3(0, 1, 0);	  //Camera-relative "up" direction

private:
	double focalLength;
	double viewportHeight;
	double viewportWidth;

	double theta;
	double h;

	int height;
	double aspectRatio, width;
	int sampleCount;

	int maxBounces;

	Position center, originPixelLocation;
	Vector3 pixelDeltaX, pixelDeltaY;
	Vector3 u, v, w; //Camera frame basis vectors (X,Y,Z)

	void Initialize();
	Color RayColor(const Ray& rRay, int bounceLeft, const Hittable& rWorld) const;
	Ray GetRay(int x, int y) const;
	Vector3 PixelSampleSquared() const;
};
