#include "Camera.h"

#include "PDF.h"
#include "HittableCollection.h"

void Camera::Render(const Hittable& rWorld, const Hittable& lights)
{
    Initialize();

    cout << "P3\n" << width << ' ' << height << "\n255\n";

    // Calculate the number of samples per pixel in each dimension
    sqrt_spp = int(sqrt(sampleCount));

    // Loop through each pixel in the image
    for (int y = 0; y < height; y++)
    {
        // Print the progress
        clog << (y * 100 / height) << " % \n" << flush;

        for (int x = 0; x < width; x++)
        {
            Color pixel(0, 0, 0);
            /*
            //non Stratified rendering
            for (int sample = 0; sample < sampleCount; sample++)
            {
                Ray ray = GetRay(x, y);
                pixel += RayColor(ray, maxBounces, rWorld);
            }
            */

            // Stratified sampling: loop through each sub-pixel sample
            for (int s_j = 0; s_j < sqrt_spp; ++s_j)
            {
                for (int s_i = 0; s_i < sqrt_spp; ++s_i)
                {
                    // Get a ray for the current sub-pixel sample
                    Ray ray = GetRay(x, y, s_i, s_j);
                    // Accumulate color for the current sub-pixel sample
                    pixel += RayColor(ray, maxBounces, rWorld, lights);
                }
            }

            WriteColor(cout, pixel, sampleCount);
        }
    }
    clog << "It has been done \n";
}

void Camera::Initialize()
{
    // Calculate camera parameters based on user input and aspect ratio
    height = /*static_cast<int>*/(int)(width / aspectRatio);

    if (height < 1) height = 1;

    center = lookFrom;

    theta = DegToRad(verticalFov);
    h = tan(theta / 2);
    viewportHeight = 2 * h * focusDist;
    viewportWidth = viewportHeight * (/*static_cast<double>*/(double)(width) / height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.(Calculate camera coordinate frame vectors)
    W = Unit(lookFrom - lookAt); //(Z)
    x = Unit(Cross(vecUp, W)); //(X)
    y = Cross(W, x); //(Y)

    // Calculate the vectors across the horizontal and down the vertical viewport edges.(Calculate viewport vectors)
    Vector3 viewportX = viewportWidth * x; // Vector across viewport horizontal edge
    Vector3 viewportY = viewportHeight * -y; // Vector down viewport vertical edge

    //Calculate pixel delta vectors and the top-left pixel position
    pixelDeltaX = viewportX / width;
    pixelDeltaY = viewportY / height;

    //Position of the top left pixel
    Vector3 viewportUpperLeft = center - (focusDist * W) - viewportX / 2 - viewportY / 2;
    //Vector3 viewportOrigin = center - Vector3(0, 0, focalLength) - viewportX / 2 - viewportY / 2;

    originPixelLocation = viewportUpperLeft + 0.5 * (pixelDeltaX + pixelDeltaY);

    //Calculate the camera defocus disk basis vectors.
    double defocusRadius = focusDist * tan(DegToRad(defocusAngle / 2));
    defocusDiskX = x * defocusRadius;
    defocusDiskY = y * defocusRadius;
}

Color Camera::RayColor(const Ray& ray, int bounceLeft, const Hittable& rWorld, const Hittable& lights) const
{
    HittableCollection info;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (bounceLeft <= 0)
    {
        return Color(0, 0, 0);
    }

    // If the ray hits nothing, return the background color.
    if (!rWorld.Hit(ray, Interval(0.001, infinity), info))
    {
        return background;
    }
    
    ScaterInfo scatterInfo;
    Color color_from_emission = info.mat->Emitted(ray, info, info.u, info.v, info.p);

    if (!info.mat->scatter(ray, info, scatterInfo))
    {
        return color_from_emission;
    }

    auto light_ptr = make_shared<HittablePDF>(lights, info.p);
    MixturePDF p(light_ptr, scatterInfo.pdf_ptr);

    Ray scattered = Ray(info.p, p.Generate(), ray.time());
    auto pdf_val = p.Value(scattered.GetDirection());

    double scattering_pdf = info.mat->scattering_pdf(ray, info, scattered);

    Color sample_color = RayColor(scattered, bounceLeft - 1, rWorld, lights);
    Color color_from_scatter = (scatterInfo.attenuation * scattering_pdf * sample_color) / pdf_val;

    return color_from_emission + color_from_scatter;
}

Ray Camera::GetRay(int x, int y, int sampleI, int sampleJ) const
{
    /*
    //Get a randomly-sampled camera ray for the pixel at location i,j, originating from the camera defocus disk.
    Vector3 pixelCenter = originPixelLocation + (x * pixelDeltaX) + (y * pixelDeltaY);
    Vector3 pixelSample = pixelCenter + PixelSampleSquared();

    Position rayOrigin = (defocusAngle <= 0) ? center : DefocusDiskSample();
    Vector3 rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection, rayTime);
    */

    // Get a randomly-sampled camera ray for the pixel at location i,j, 
    //originating from the camera defocus disk, and randomly sampled around the pixel location.
    Vector3 pixelCenter = originPixelLocation + (x * pixelDeltaX) + (y * pixelDeltaY);
    Vector3 pixelSample = pixelCenter + PixelSampleSquared(sampleI, sampleJ);

    Vector3 rayOrigin = (defocusAngle <= 0) ? center : DefocusDiskSample();
    Vector3 rayDirection = pixelSample - rayOrigin;
    double rayTime = RandomDouble();

    return Ray(rayOrigin, rayDirection, rayTime);
}

Vector3 Camera::PixelSampleSquared(int sampleI, int sampleJ) const
{
    //Returns a random point in the square around a pixel at the origin
    double pX = -0.5 + sqrt_spp * (sampleI + RandomDouble());
    double pY = -0.5 + sqrt_spp * (sampleJ + RandomDouble());

    return (pX * pixelDeltaX) + (pY * pixelDeltaY);
}

Position Camera::DefocusDiskSample() const
{
    //Returns a random point in the camera defocus disk.
    Vector3 position = RandomInUnitDisc();
    return center + (position[0] * defocusDiskX) + (position[1] * defocusDiskY);
}
