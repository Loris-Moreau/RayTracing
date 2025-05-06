#include <iostream>

#include "Box.h"
#include "BVH.h"
#include "Camera.h"
#include "CheckerTexture.h"
#include "Common.h"
#include "ConstantMedium.h"
#include "Dielectric.h"
#include "DiffuseLight.h"
#include "FlipFace.h"
#include "HittableList.h"
#include "ImageTexture.h"
#include "Lambertian.h"
#include "Metal.h"
#include "MovingSphere.h"
#include "NoiseTexture.h"
#include "RotateY.h"
#include "SolidColor.h"
#include "Sphere.h"
#include "Translate.h"
#include "XYRect.h"
#include "XZRect.h"
#include "YZRect.h"

Vector3 ray_color(const Ray& r, const color& background, const Hittable& world, int depth)
{
    HitRecord rec;

    // If we've exceeded the Ray bounce limit, no more light is gathered.
    if (depth <= 0)
    {
        return color{0, 0, 0};
    }

    // If the Ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
    {
        return background;
    }

    Ray scattered;
    color attenuation;
    const color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    {
        return emitted;
    }

    return emitted +
           attenuation * ray_color(scattered, background, world, depth - 1);
}

HittableList random_scene()
{
    HittableList world;

    std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(
        std::make_shared<SolidColor>(0.2, 0.3, 0.1),
        std::make_shared<SolidColor>(0.9, 0.9, 0.9));
    
    world.add(std::make_shared<Sphere>(point3{0, -1000, 0}, 1000, std::make_shared<Lambertian>(checker)));

    for (int a = -10; a < 10; ++a)
    {
        for (int b = -10; b < 10; ++b)
        {
            const double choose_mat = random_double();
            Vector3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Vector3{4, .2, 0}).length() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    // diffuse
                    Vector3 albedo = Vector3::random() * Vector3::random();
                    world.add(std::make_shared<MovingSphere>(
                        center, center + Vector3{0, random_double(0, .5), 0}, 0.0,
                        1.0, 0.2,
                        std::make_shared<Lambertian>(std::make_shared<SolidColor>(albedo))));
                }
                else if (choose_mat < 0.95)
                {
                    // Metal
                    Vector3 albedo = Vector3::random(.5, 1);
                    double fuzz = random_double(0, .5);
                    world.add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
                }
                else
                {
                    // glass
                    world.add(std::make_shared<Sphere>(
                        center, 0.2, std::make_shared<Dielectric>(1.5)));
                }
            }
        }
    }

    world.add(std::make_shared<Sphere>(Vector3{0, 1, 0}, 1.0, std::make_shared<Dielectric>(1.5)));
    world.add(std::make_shared<Sphere>(Vector3{-4, 1, 0}, 1.0, std::make_shared<Lambertian>(std::make_shared<SolidColor>(0.4, 0.2, 0.1))));
    world.add(std::make_shared<Sphere>(Vector3{4, 1, 0}, 1.0, std::make_shared<Metal>(Vector3{0.7, 0.6, 0.5}, 0.0)));

    return world;
}

HittableList two_spheres()
{
    HittableList objects;

    std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(
        std::make_shared<SolidColor>(0.2, 0.3, 0.1),
        std::make_shared<SolidColor>(0.9, 0.9, 0.9));

    objects.add(std::make_shared<Sphere>(point3(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
    objects.add(std::make_shared<Sphere>(point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

    return objects;
}

HittableList two_perlin_spheres()
{
    HittableList objects;

    const std::shared_ptr<NoiseTexture> pertext = std::make_shared<NoiseTexture>(4.0);

    objects.add(std::make_shared<Sphere>(
        point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    objects.add(std::make_shared<Sphere>(
        point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    return objects;
}

HittableList earth()
{
    std::shared_ptr<ImageTexture> earth_texture = std::make_shared<ImageTexture>("earthmap.jpg");
    std::shared_ptr<Lambertian> earth_surface = std::make_shared<Lambertian>(earth_texture);
    const std::shared_ptr<Sphere> globe = std::make_shared<Sphere>(point3(0, 0, 0), 2, earth_surface);

    return HittableList(globe);
}

HittableList simple_light()
{
    HittableList objects;

    std::shared_ptr<NoiseTexture> pertext = std::make_shared<NoiseTexture>(4);
    objects.add(std::make_shared<Sphere>(point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    objects.add(std::make_shared<Sphere>(point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    std::shared_ptr<DiffuseLight> difflight = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(4, 4, 4));
    objects.add(std::make_shared<Sphere>(point3(0, 7, 0), 2, difflight));
    objects.add(std::make_shared<XYRect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

HittableList cornell_box()
{
    HittableList objects;

    std::shared_ptr<Lambertian> red = std::make_shared<Lambertian>(
        std::make_shared<SolidColor>(.65, .05, .05));
    std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(
        std::make_shared<SolidColor>(.73, .73, .73));
    std::shared_ptr<Lambertian> green = std::make_shared<Lambertian>(
        std::make_shared<SolidColor>(.12, .45, .15));
    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(
        std::make_shared<SolidColor>(15, 15, 15));

    objects.add(std::make_shared<FlipFace>(
        std::make_shared<YZRect>(0, 555, 0, 555, 555, green)));
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<XZRect>(213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<FlipFace>(
        std::make_shared<XZRect>(0, 555, 0, 555, 0, white)));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<FlipFace>(
        std::make_shared<XYRect>(0, 555, 0, 555, 555, white)));

    std::shared_ptr<Hittable> box1 =
        std::make_shared<Box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vector3(265, 0, 295));
    objects.add(std::move(box1));

    std::shared_ptr<Hittable> box2 =
        std::make_shared<Box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vector3(130, 0, 65));
    objects.add(std::move(box2));

    return objects;
}

HittableList cornell_smoke()
{
    HittableList objects;

    std::shared_ptr<Lambertian> red = std::make_shared<Lambertian>(
        std::make_shared<SolidColor>(.65, .05, .05));
    std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(
        std::make_shared<SolidColor>(.73, .73, .73));
    std::shared_ptr<Lambertian> green = std::make_shared<Lambertian>(
        std::make_shared<SolidColor>(.12, .45, .15));
    std::shared_ptr<DiffuseLight> light =
        std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(7, 7, 7));

    objects.add(std::make_shared<FlipFace>(
        std::make_shared<YZRect>(0, 555, 0, 555, 555, green)));
    objects.add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<XZRect>(113, 443, 127, 432, 554, light));
    objects.add(std::make_shared<FlipFace>(
        std::make_shared<XZRect>(0, 555, 0, 555, 555, white)));
    objects.add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<FlipFace>(
        std::make_shared<XYRect>(0, 555, 0, 555, 555, white)));

    std::shared_ptr<Hittable> box1 =
        std::make_shared<Box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vector3(265, 0, 295));

    std::shared_ptr<Hittable> box2 =
        std::make_shared<Box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vector3(130, 0, 65));

    objects.add(std::make_shared<ConstantMedium>(
        box1, 0.01, std::make_shared<SolidColor>(0, 0, 0)));
    objects.add(std::make_shared<ConstantMedium>(
        box2, 0.01, std::make_shared<SolidColor>(1, 1, 1)));

    return objects;
}

HittableList final_scene()
{
    HittableList boxes1;
    std::shared_ptr<Lambertian> ground = std::make_shared<Lambertian>(
        std::make_shared<SolidColor>(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++)
    {
        for (int j = 0; j < boxes_per_side; j++)
        {
            const double w = 100.0;
            const double x0 = -1000.0 + i * w;
            const double z0 = -1000.0 + j * w;
            const double y0 = 0.0;
            const double x1 = x0 + w;
            const double y1 = random_double(1, 101);
            const double z1 = z0 + w;

            boxes1.add(std::make_shared<Box>(point3(x0, y0, z0),
                                             point3(x1, y1, z1), ground));
        }
    }

    HittableList objects;

    objects.add(std::make_shared<BVHNode>(boxes1, 0, 1));

    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(7, 7, 7));
    objects.add(std::make_shared<XZRect>(123, 423, 147, 412, 554, light));

    point3 center1 = point3(400, 400, 200);
    Vector3 center2 = center1 + Vector3(30, 0, 0);
    std::shared_ptr<Lambertian> moving_sphere_material = std::make_shared<Lambertian>(std::make_shared<SolidColor>(0.7, 0.3, 0.1));
    objects.add(std::make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(std::make_shared<Sphere>(point3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
    objects.add(std::make_shared<Sphere>(point3(0, 150, 145), 50, std::make_shared<Metal>(color(0.8, 0.8, 0.9), 10.0)));

    std::shared_ptr<Sphere> boundary = std::make_shared<Sphere>(point3(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(std::make_shared<ConstantMedium>(boundary, 0.2, std::make_shared<SolidColor>(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Sphere>(point3(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
    objects.add(std::make_shared<ConstantMedium>(boundary, .0001, std::make_shared<SolidColor>(1, 1, 1)));

    std::shared_ptr<Lambertian> emat = std::make_shared<Lambertian>(
        std::make_shared<ImageTexture>("earthmap.jpg"));
    objects.add(std::make_shared<Sphere>(point3(400, 200, 400), 100, emat));
    std::shared_ptr<NoiseTexture> pertext = std::make_shared<NoiseTexture>(0.1);
    objects.add(std::make_shared<Sphere>(
        point3(220, 280, 300), 80, std::make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(
        std::make_shared<SolidColor>(.73, .73, .73));
    const int ns = 1000;
    for (int j = 0; j < ns; j++)
    {
        boxes2.add(std::make_shared<Sphere>(point3::random(0, 165), 10, white));
    }

    objects.add(std::make_shared<Translate>(
        std::make_shared<RotateY>(std::make_shared<BVHNode>(boxes2, 0.0, 1.0),
                                   15),
        Vector3(-100, 270, 395)));

    return objects;
}

int main(int argc, char* argv[])
{
    const int image_width = 600;
    const int image_height = 600;
    const int samples_per_pixel = 10000;
    const int max_depth = 50;
    const double aspect_ratio = static_cast<double>(image_width) / image_height;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    const HittableList world = final_scene();

    const Vector3 lookfrom{478, 278, -800};
    const Vector3 lookat{278, 278, 0};
    const Vector3 vup{0, 1, 0};
    const double dist_to_focus = 10.0;
    const double aperture = 0.0;
    const double vfov = 40.0;
    const color background{0, 0, 0};

    const Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture,
                     dist_to_focus, 0.0, 1.0);

    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i)
        {
            Vector3 color{0, 0, 0};

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                const double u = (i + random_double()) / image_width;
                const double v = (j + random_double()) / image_height;
                Ray r = cam.get_ray(u, v);
                color += ray_color(r, background, world, max_depth);
            }

            color.write_color(std::cout, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}