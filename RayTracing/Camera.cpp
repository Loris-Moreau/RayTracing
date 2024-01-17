#include "Camera.h"

void Camera::Render(const Hittable& rWorld)
{
    Initialize();

    cout << "P3\n" << width << ' ' << height << "\n255\n";
    for (int y = 0; y < height; y++)
    {
        clog << "Progress : " << (y * 100 / height) << " %\n" << flush;
        for (int x = 0; x < width; x++)
        {
            Color pixel(0, 0, 0);
            for (int sample = 0; sample < sampleCount; sample++)
            {
                Ray ray = GetRay(x, y);
                pixel += RayColor(ray, maxBounces, rWorld);
            }

            WriteColor(cout, pixel, sampleCount);
        }
    }
    clog << "It has been done\n";
}

void Camera::Initialize()
{
    height = static_cast<int>(width / aspectRatio);

    if (height < 1) height = 1;

    center = lookFrom;
    focalLength = (lookFrom - lookAt).Length();

    theta = DegToRad(verticalFov);
    h = tan(theta / 2);
    viewportHeight = 2 * h * focalLength;
    viewportWidth = viewportHeight * (static_cast<double>(width) / height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w = Unit(lookFrom - lookAt); //(Z)
    u = Unit(Cross(vecUp, w)); //(X)
    v = Cross(w, u); //(Y)

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    Vector3 viewportX = viewportWidth * u; // Vector across viewport horizontal edge
    Vector3 viewportY = viewportHeight * -v; // Vector down viewport vertical edge

    //Delta vector between pixels
    pixelDeltaX = viewportX / width;
    pixelDeltaY = viewportY / height;

    //Position of the top left pixel
    auto viewportUpperLeft = center - (focalLength * w) - viewportX / 2 - viewportY / 2;
    //Vector3 viewportOrigin = center - Vector3(0, 0, focalLength) - viewportX / 2 - viewportY / 2;

    originPixelLocation = viewportUpperLeft + 0.5 * (pixelDeltaX + pixelDeltaY);
}

Color Camera::RayColor(const Ray& ray, int bounceLeft, const Hittable& rWorld) const
{
    HitInfo hitInfo;

    if (bounceLeft <= 0) return Color(0, 0, 0);

    if (rWorld.Hit(ray, Interval(0.001, infinity), hitInfo))
    {
        Ray scattered;
        Color attenuation;

        if (hitInfo.mat->Scatter(ray, hitInfo, attenuation, scattered))
        {
            return attenuation * RayColor(scattered, bounceLeft - 1, rWorld);
        }

        return Color(0, 0, 0);
    }

    Vector3 unitDirection = Unit(ray.GetDirection());
    double blue = 0.5 * (unitDirection.y + 1.0);

    return (1.0 - blue) * Color(1.0, 1.0, 1.0) + blue * Color(0.5, 0.7, 1.0);
}

Ray Camera::GetRay(int x, int y) const
{
    //Get a randomly sampled camera ray for the pixel at location i,j.
    Vector3 pixelCenter = originPixelLocation + (x * pixelDeltaX) + (y * pixelDeltaY);
    Vector3 pixelSample = pixelCenter + PixelSampleSquared();

    Position rayOrigin = center;
    Vector3 rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection);
}

Vector3 Camera::PixelSampleSquared() const
{
    //Returns a random point in the square around a pixel at the origin
    double pX = -0.5 + RandomDouble();
    double pY = -0.5 + RandomDouble();

    return (pX * pixelDeltaX) + (pY * pixelDeltaY);
}
