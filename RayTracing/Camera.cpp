#include "Camera.h"

void Camera::Render(const Hittable& rWorld)
{
    Initialize();
    
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";
    
    std::clog << "Game Start !" << '\n';
    
    for (int y = 0; y < height; y++)
    {
        std::clog << "Progress : " << (y * 100 / height) << " %\n" << std::flush;
        
        for (int x = 0; x < width; x++)
        {
            Color pixel(0, 0, 0);
            for (int sample = 0; sample < sampleCount; sample++)
            {
                Ray ray = GetRay(x, y);
                pixel += RayColor(ray, maxBounces, rWorld);
            }

            WriteColor(std::cout, pixel, sampleCount);
        }
    }
    
    std::clog << "It has been done\n";
}

void Camera::Initialize()
{
    height = static_cast<int>(width / aspectRatio);

    if (height < 1) height = 1;

    center = lookFrom;

    theta = DegToRad(verticalFov);
    h = tan(theta / 2);
    viewportHeight = 2 * h * focusDist;
    viewportWidth = viewportHeight * (static_cast<double>(width) / height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w = Unit(lookFrom - lookAt); //(Z)
    x = Unit(Cross(vecUp, w)); //(X)
    y = Cross(w, x); //(Y)

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    Vector3 viewportX = viewportWidth * x; // Vector across viewport horizontal edge
    Vector3 viewportY = viewportHeight * -y; // Vector down viewport vertical edge

    //Delta vector between pixels
    pixelDeltaX = viewportX / width;
    pixelDeltaY = viewportY / height;

    //Position of the top left pixel
    Vector3 viewportUpperLeft = center - (focusDist * w) - viewportX / 2 - viewportY / 2;
    //Vector3 viewportOrigin = center - Vector3(0, 0, focalLength) - viewportX / 2 - viewportY / 2;

    originPixelLocation = viewportUpperLeft + 0.5 * (pixelDeltaX + pixelDeltaY);

    //Calculate the camera defocus disk basis vectors.
    double defocusRadius = focusDist * tan(DegToRad(defocusAngle / 2));
    defocusDiskX = x * defocusRadius;
    defocusDiskY = y * defocusRadius;
}

Color Camera::RayColor(const Ray& ray, int bounceLeft, const Hittable& rWorld) const
{
    HitInfo hitInfo;

    /*
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
    */

    //If we've exceeded the ray bounce limit, no more light is gathered.
    if (bounceLeft <= 0)
        return Color(0, 0, 0);

    // If the ray hits nothing, return the background color.
    if (!rWorld.Hit(ray, Interval(0.001, infinity), hitInfo))
        return background;

    Ray scattered;
    Color attenuation;
    Color color_from_emission = hitInfo.mat->Emitted(hitInfo.x, hitInfo.y, hitInfo.coordinates);

    if (!hitInfo.mat->Scatter(ray, hitInfo, attenuation, scattered))
        return color_from_emission;

    Color color_from_scatter = attenuation * RayColor(scattered, bounceLeft - 1, rWorld);

    return color_from_emission + color_from_scatter;
}

Ray Camera::GetRay(int x, int y) const
{
    //Get a randomly-sampled camera ray for the pixel at location i,j, originating from the camera defocus disk.
    Vector3 pixelCenter = originPixelLocation + (x * pixelDeltaX) + (y * pixelDeltaY);
    Vector3 pixelSample = pixelCenter + PixelSampleSquared();

    Position rayOrigin = (defocusAngle <= 0) ? center : DefocusDiskSample();
    Vector3 rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection, rayTime);
}

Vector3 Camera::PixelSampleSquared() const
{
    //Returns a random point in the square around a pixel at the origin
    double pX = -0.5 + RandomDouble();
    double pY = -0.5 + RandomDouble();

    return (pX * pixelDeltaX) + (pY * pixelDeltaY);
}

Position Camera::DefocusDiskSample() const
{
    //Returns a random point in the camera defocus disk.
    Vector3 position = RandomInUnitDisc();
    return center + (position[0] * defocusDiskX) + (position[1] * defocusDiskY);
}
