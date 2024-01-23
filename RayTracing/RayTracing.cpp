// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

///Command for .ppm file creation :
///.\x64\Debug\RayTracing.exe > Render.ppm

#include "Camera.h"
#include "HittableCollection.h"
#include "Sphere.h"
#include "BVHNode.h"
//Mat
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

#include "Texture.h"

using namespace std;

void BaseBalls(int set, int glass)
{
    //World
    HittableCollection world;

    //First Set Materials
    //shared_ptr<Materials> groundMat = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));

    shared_ptr<Materials> rightMat = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.7);
    shared_ptr<Materials> centerMat = make_shared<Dielectric>(1.5);
    shared_ptr<Materials> leftMat = make_shared<Dielectric>(1.5);

    if (set == 1)
    {
        //Second Set Materials
        shared_ptr<Materials> centerMat = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
        shared_ptr<Materials> leftMat = make_shared<Dielectric>(1.5);
        shared_ptr<Materials> rightMat = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);
    }

    //world.Add(make_shared<Sphere>(Position(0, -100.5, -1), 100, groundMat));
    world.Add(make_shared<Sphere>(Position(0, 0, -1), 0.5, centerMat));
    world.Add(make_shared<Sphere>(Position(1.0, 0, -1), 0.5, rightMat)); //shiny metal ball

    if (glass == 1)
    {
        world.Add(make_shared<Sphere>(Position(-1.0, 0, -1), 0.5, leftMat)); //Reflective glass
    }
    else
    {
        world.Add(make_shared<Sphere>(Position(-1.0, 0.0, -1.0), -0.4, leftMat)); //Transparent Glass
    }

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance)
    Camera camera(400, 16.0 / 9.0, 75, 50, 25, Position(2, 2, 6), Position(0, 0, 0), Vector3(0, 1, 0), 0.65, 10);
    camera.Render(world);
}

void RandomSpheres(int nb)
{
    //World
    HittableCollection world;

    shared_ptr<Materials> groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Position(0, -1000, 0), 1000, groundMaterial));

    for (int a = -nb; a < nb; a++)
    {
        for (int b = -nb; b < nb; b++)
        {
            double choose_mat = RandomDouble();
            Position Center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

            if ((Center - Position(4, 0.2, 0)).Length() > 0.9)
            {
                shared_ptr<Materials> sphereMaterial;

                if (choose_mat < 0.8)
                {
                    //Diffuse
                    Vector3 albedo = Color::Random() * Color::Random();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    auto center2 = Center + Vector3(0, RandomDouble(0, 0.5), 0);
                    world.Add(make_shared<Sphere>(Center, center2, 0.2, sphereMaterial));
                }
                else if (choose_mat < 0.95)
                {
                    //Metal
                    Vector3 albedo = Color::Random(0.5, 1);
                    double fuzz = RandomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(Center, 0.2, sphereMaterial));
                }
                else
                {
                    //Glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.Add(make_shared<Sphere>(Center, 0.2, sphereMaterial));
                }
            }
        }
    }

    shared_ptr<Materials> material1 = make_shared<Dielectric>(1.5);
    world.Add(make_shared<Sphere>(Position(0, 1, 0), 1.0, material1));

    shared_ptr<Materials> material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.Add(make_shared<Sphere>(Position(-4, 1, 0), 1.0, material2));

    shared_ptr<Materials> material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(make_shared<Sphere>(Position(4, 1, 0), 1.0, material3));

    world = HittableCollection(make_shared<BVHNode>(world));

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance)
    Camera camera(400, 16.0 / 9.0, 75, 50, 25, Position(13, 2, 6), Position(0, 0, 0), Vector3(0, 1, 0), 0.6, 10);
    camera.Render(world);
}

void Checkers()
{
    //World
    HittableCollection world;

    shared_ptr<CheckerTexture> checker = make_shared<CheckerTexture>(0.8, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    world.Add(make_shared<Sphere>(Position(0, -10, 0), 10, make_shared<Lambertian>(checker)));
    world.Add(make_shared<Sphere>(Position(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance)
    Camera camera(400, 16.0 / 9.0, 75, 50, 25, Position(13, 2, 6), Position(0, 0, 0), Vector3(0, 1, 0), 0);
    camera.Render(world);
}

int main(int argc, char* argv[])
{
    /*switch (3)
    {
    case 1: BaseBalls(0, 0);
        break;
    case 2: Checkers();
        break;
    case 3: RandomSpheres(7);
        break;
    }*/
    Checkers();

    return 0;
}
