#pragma once

#include "Hittable.h"
#include "Materials.h"

using namespace std;

class Camera
{
public:
	Camera() = default;
	Camera(double imageWidth, double ratio, int samplePerPixel = 10, int bounces = 10, double fov = 90,
		Position lookfrom = Position(0, 0, 0), Position lookat = Position(0, 0, 0), Vector3 upVector = Vector3(0, 1, 0), 
		double defocus_Angle = 1, double focusDistance = 4, 
		Color bg = Color(0.70,0.80,1.00))
		: verticalFov(fov), lookFrom(lookfrom), lookAt(lookat), vecUp(upVector), defocusAngle(defocus_Angle),
		  focusDist(focusDistance), background(bg), sqrt_spp(0), viewportHeight(0), viewportWidth(0), theta(0), h(0),
		  height(0),
		  width(imageWidth),
		  aspectRatio(ratio), sampleCount(samplePerPixel), maxBounces(bounces)
	{
	}

	void Render(const Hittable& rWorld, const Hittable& lights);

	double verticalFov; //Vertical view angle (field of view)
	Position lookFrom; //Point camera is looking from
	Position lookAt;  //Point camera is looking at
	Vector3 vecUp;	 //Camera-relative "up" direction

	double defocusAngle = 0; // Variation angle of rays through each pixel
	double focusDist = 10; // Distance from camera lookfrom point to plane of perfect focus

	Color background; //Scene Background color

private:
	int sqrt_spp;

	double viewportHeight;
	double viewportWidth;

	double theta;
	double h;

	int height;
	double width;
	double aspectRatio;
	int sampleCount;

	int maxBounces;

	Position center, originPixelLocation;
	Vector3 pixelDeltaX, pixelDeltaY;
	Vector3 x, y, W; //Camera frame basis vectors (X,Y,Z)

	double rayTime = RandomDouble();

	Vector3 defocusDiskX; //Defocus disk horizontal radius
	Vector3 defocusDiskY; //Defocus disk vertical radius

	void Initialize();
	Color RayColor(const Ray& ray, int bounceLeft, const Hittable& rWorld, const Hittable& lights) const;
	Ray GetRay(int x, int y, int sampleI, int sampleJ) const;
	Vector3 PixelSampleSquared(int sampleI, int sampleJ) const;

	Position DefocusDiskSample() const;
};
