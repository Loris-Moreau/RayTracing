#include "Camera.h"

using namespace std;

void Camera::render(const Hittable& world)
{
	Initialise();
}

void Camera::Initialise()
{
	//Resolution
	int height = static_cast<int>(width / resolution);
	if (height < 1) 
	{ 
		height = 1; 
	}

	//Viewport
	viewportHeight = 2;
	viewportWidth = viewportHeight * (static_cast<double>(width) / height);

	focalLength = 1;
	cameraCenter = Position(0, 0, 0);

	viewportX = Vector3(viewportWidth, 0, 0);
	viewportY = Vector3(0, -viewportHeight, 0); //We invert Y

	//Delta vector between pixels
	pixelDeltaX = viewportX / width;
	Vector3 pixelDeltaY = viewportY / height;

	//Position of the top left pixel
	viewportOrigin = cameraCenter - Vector3(0, 0, focalLength) - viewportX / 2 - viewportY / 2;

	originPixelLocation = viewportOrigin + 0.5 * (pixelDeltaX + pixelDeltaY);

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

			HittableCollection world;
			Color pixel = rayColor(ray, world);
			WriteColor(cout, pixel);
		}
	}

	clog << "It has been done \n";
}

Color Camera::rayColor(const Ray& ray, const Hittable& world) const
{
	HitInfo hitInfo;
	if (world.Hit(ray, Interval(0, infinity), hitInfo))
	{
		return 0.5 * (hitInfo.normal + Color(1, 1, 1));
	}

	Vector3 unitDirection = Unit(ray.GetDirection());
	double blue = 0.5 * (unitDirection.y + 1.0);
	return (1.0 - blue) * Color(1.0, 1.0, 1.0) + blue * Color(0.5, 0.7, 1.0);
}
