//RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

///Command for .ppm file creation :
///.\x64\Debug\RayTracing.exe > Render.ppm

#include "Camera.h"
#include "BVHNode.h"

#include "HittableCollection.h"
#include "Sphere.h"
#include "Quadrilaterals.h"
//Mat
#include "Materials.h"
#include "ConstantDensityMedium.h"

#include "Texture.h"

using namespace std;

void BaseBalls(int set, int glass)
{
    //World
    HittableCollection world;

    //First Set Materials
    shared_ptr<Materials> groundMat = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));

    shared_ptr<Materials> rightMat = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.7);
    shared_ptr<Materials> centerMat = make_shared<Dielectric>(1.5);
    shared_ptr<Materials> leftMat = make_shared<Dielectric>(1.5);

    if (set == 1)
    {
        //Second Set Materials
        centerMat = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
        leftMat = make_shared<Dielectric>(1.5);
        rightMat = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);
    }

    world.Add(make_shared<Sphere>(Position(0, -100.5, -1), 100, groundMat));
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
    camera.Render(world, world);
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
                    Vector3 center2 = Center + Vector3(0, RandomDouble(0, 0.5), 0);
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
    Camera camera(400, 16.0 / 9.0, 75, 50, 25, Position(13, 2, 6), Position(0, 0, 0), Vector3(0, 1, 0), 0.6, 10, Color(0.70, 0.80, 1.00));
    camera.Render(world, world);
}

void Checkers()
{
    //World
    HittableCollection world;

    shared_ptr<Texture> checker = make_shared<CheckerTexture>(0.8, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    world.Add(make_shared<Sphere>(Position(0, -10, 0), 10, make_shared<Lambertian>(checker)));
    world.Add(make_shared<Sphere>(Position(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance)
    Camera camera(400, 16.0 / 9.0, 75, 50, 25, Position(13, 2, 6), Position(0, 0, 0), Vector3(0, 1, 0), 0.6, 10, Color(0.70, 0.80, 1.00));
    camera.Render(world, world);
}

//Image Texture
/*
void Earth()
{
    shared_ptr<Texture> imageTexture = make_shared<ImageTexture>("..\\Images\\earthmap.jpg");
    shared_ptr<Materials> imageSurface = make_shared<Lambertian>(imageTexture);
    shared_ptr<Sphere> renderedObj = make_shared<Sphere>(Position(0, 0, 0), 2, imageSurface);


    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance)
    Camera camera(400, 16.0 / 9.0, 75, 50, 25, Position(0, 0, 12), Position(0, 0, 0), Vector3(0, 1, 0), 0, 0, Color(0.70, 0.80, 1.00));
    camera.Render(HittableCollection(renderedObj));
}
*/

void PerlinSphere()
{
    //World
    HittableCollection world;

    shared_ptr<Texture> pertext = make_shared<NoiseTexture>(4);
    world.Add(make_shared<Sphere>(Position(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    world.Add(make_shared<Sphere>(Position(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance)
    Camera camera(400, 16.0 / 9.0, 75, 50, 25, Position(13, 2, 6), Position(0, 0, 0), Vector3(0, 1, 0), 0, 10, Color(0.70, 0.80, 1.00));
    camera.Render(world, world);
}

void Quads()
{
    HittableCollection world;

    //Materials
    shared_ptr<Materials> leftRed = make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    shared_ptr<Materials> backGreen = make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    shared_ptr<Materials> rightBlue = make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    shared_ptr<Materials> upperOrange = make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    shared_ptr<Materials> lowerMiku = make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    //Quads
    world.Add(make_shared<Quadrilaterals>(Position(-3, -2, 5), Vector3(0, 0, -4), Vector3(0, 4, 0), leftRed));
    world.Add(make_shared<Quadrilaterals>(Position(-2, -2, 0), Vector3(4, 0, 0), Vector3(0, 4, 0), backGreen));
    world.Add(make_shared<Quadrilaterals>(Position(3, -2, 1), Vector3(0, 0, 4), Vector3(0, 4, 0), rightBlue));
    world.Add(make_shared<Quadrilaterals>(Position(-2, 3, 1), Vector3(4, 0, 0), Vector3(0, 0, 4), upperOrange));
    world.Add(make_shared<Quadrilaterals>(Position(-2, -3, 5), Vector3(4, 0, 0), Vector3(0, 0, -4), lowerMiku));

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance)
    Camera camera(1200, 1.0, 70, 50, 80, Position(0, 0, 9), Position(0, 0, 0), Vector3(0, 1, 0), 0, 10, Color(0.70, 0.80, 1.00));
    camera.Render(world, world);
}

void SimpleLight() 
{
    HittableCollection world;

    shared_ptr<Texture> pertext = make_shared<NoiseTexture>(4);
    world.Add(make_shared<Sphere>(Position(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    world.Add(make_shared<Sphere>(Position(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    shared_ptr<Materials> difflight = make_shared<DiffuseLight>(Color(4, 4, 4));
    world.Add(make_shared<Quadrilaterals>(Position(3, 1, -2), Vector3(2, 0, 0), Vector3(0, 2, 0), difflight));
    //world.Add(make_shared<Sphere>(Position(0, 6, 0), 1.5, difflight));

    //light source
    HittableCollection lights;
    shared_ptr<Materials> lightMat = shared_ptr<Materials>();
    lights.Add(make_shared<Quadrilaterals>(Position(343, 554, 332), Vector3(-2, 0, 0), Vector3(0, 2, 0), lightMat));

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance, bg)
    Camera camera(600, 16.0/9.0, 45, 50, 25, Position(20, 3, 6), Position(0, 1, 0), Vector3(0, 1, 0), 0, 10, Color(0, 0, 0));
    camera.Render(world, lights);
}

void CornellBox()
{
    HittableCollection world;


    shared_ptr<Materials> red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    shared_ptr<Materials> white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    shared_ptr<Materials> green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));

    shared_ptr<Materials> light = make_shared<DiffuseLight>(Color(4, 4, 4));

    //Room
    world.Add(make_shared<Quadrilaterals>(Position(555, 0, 0), Vector3(0, 555, 0), Vector3(0, 0, 555), green));
    world.Add(make_shared<Quadrilaterals>(Position(0, 0, 0), Vector3(0, 555, 0), Vector3(0, 0, 555), red));

    world.Add(make_shared<Quadrilaterals>(Position(0, 0, 0), Vector3(555, 0, 0), Vector3(0, 0, 555), white));
    world.Add(make_shared<Quadrilaterals>(Position(555, 555, 555), Vector3(-555, 0, 0), Vector3(0, 0, -555), white));
    world.Add(make_shared<Quadrilaterals>(Position(0, 0, 555), Vector3(555, 0, 0), Vector3(0, 555, 0), white));

    //Light  
    world.Add(make_shared<Quadrilaterals>(Position(213, 500, 227), Vector3(-130, 0, 0), Vector3(0, 0, 105), light));

    //Box 1
    shared_ptr<Materials> aluminum = make_shared<Metal>(Color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<Hittable> box1 = Box(Position(0, 0, 0), Position(165, 330, 165), aluminum);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector3(265, 0, 295));
    world.Add(box1);

    //Box 2
    shared_ptr<Hittable> box2 = Box(Position(0, 0, 0), Position(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector3(130, 0, 65));
    world.Add(box2);

    //light source
    HittableCollection lights;
    shared_ptr<Materials> lightMat = shared_ptr<Materials>();
    lights.Add(make_shared<Quadrilaterals>(Position(213, 500, 227), Vector3(-130, 0, 0), Vector3(0, 0, 105), lightMat));

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance, bg)
    Camera camera(600, 1.0, 25, 40, 25, Position(278, 278, -1250), Position(278, 278, 0), Vector3(0, 1, 0), 0, 100, Color(0.70, 0.80, 1.00));
    camera.Render(world, lights);
}

void CornellSmoke() 
{
    HittableCollection world;

    shared_ptr<Materials> red = make_shared<Lambertian>(Color(.65, .05, .05));
    shared_ptr<Materials> white = make_shared<Lambertian>(Color(.73, .73, .73));
    shared_ptr<Materials> green = make_shared<Lambertian>(Color(.12, .45, .15));

    shared_ptr<Materials> light = make_shared<DiffuseLight>(Color(7, 7, 7));

    world.Add(make_shared<Quadrilaterals>(Position(555, 0, 0), Vector3(0, 555, 0), Vector3(0, 0, 555), green));
    world.Add(make_shared<Quadrilaterals>(Position(0, 0, 0), Vector3(0, 555, 0), Vector3(0, 0, 555), red));

    world.Add(make_shared<Quadrilaterals>(Position(113, 554, 127), Vector3(330, 0, 0), Vector3(0, 0, 305), light));

    world.Add(make_shared<Quadrilaterals>(Position(0, 555, 0), Vector3(555, 0, 0), Vector3(0, 0, 555), white));
    world.Add(make_shared<Quadrilaterals>(Position(0, 0, 0), Vector3(555, 0, 0), Vector3(0, 0, 555), white));
    world.Add(make_shared<Quadrilaterals>(Position(0, 0, 555), Vector3(555, 0, 0), Vector3(0, 555, 0), white));

    shared_ptr<Hittable> box1 = Box(Position(0, 0, 0), Position(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector3(265, 0, 295));

    shared_ptr<Hittable> box2 = Box(Position(0, 0, 0), Position(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector3(130, 0, 65));

    world.Add(make_shared<ConstantDensityMedium>(box1, 0.01, Color(0, 0, 0)));
    world.Add(make_shared<ConstantDensityMedium>(box2, 0.01, Color(1, 1, 1)));

    //light source
    HittableCollection lights;
    shared_ptr<Materials> lightMat = shared_ptr<Materials>();
    lights.Add(make_shared<Quadrilaterals>(Position(343, 554, 332), Vector3(-130, 0, 0), Vector3(0, 0, 0), lightMat));

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance, bg)
    Camera camera(600, 1.0, 50, 50, 25, Position(278, 278, -1250), Position(278, 278, 0), Vector3(0, 1, 0), 0, 100, Color(0, 0, 0));
    camera.Render(world, lights);
}

void FinalSceneB2(int imageWidth, int samplePerPixel, int bounces, int floorAmount, int clusterAmount)
{
    HittableCollection boxes1;
    shared_ptr<Lambertian> ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    for (int i = 0; i < floorAmount; i++)
    {
        for (int j = 0; j < floorAmount; j++)
        {
            double W = 100.0;
            double x0 = -1000.0 + i * W;
            double z0 = -1000.0 + j * W;
            double y0 = 0.0;
            double x1 = x0 + W;
            double y1 = RandomDouble(1, 101);
            double z1 = z0 + W;

            boxes1.Add(Box(Position(x0, y0, z0), Position(x1, y1, z1), ground));
        }
    }

    HittableCollection world;

    world.Add(make_shared<BVHNode>(boxes1));

    shared_ptr<Materials> light = make_shared<DiffuseLight>(Color(7, 7, 7));
    world.Add(make_shared<Quadrilaterals>(Position(123, 554, 147), Vector3(300, 0, 0), Vector3(0, 0, 265), light));

    Position center1 = Position(400, 400, 200);
    Vector3 center2 = center1 + Vector3(100, 0, 0);
    shared_ptr<Materials> sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.Add(make_shared<Sphere>(center1, center2, 50, sphere_material));

    world.Add(make_shared<Sphere>(Position(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    world.Add(make_shared<Sphere>(Position(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

    shared_ptr<Sphere> boundary = make_shared<Sphere>(Position(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    world.Add(boundary);
    world.Add(make_shared<ConstantDensityMedium>(boundary, 0.05, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Position(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    world.Add(make_shared<ConstantDensityMedium>(boundary, .0001, Color(1, 1, 1)));

    //shared_ptr<ImageTexture> emat = make_shared<Lambertian>(make_shared<ImageTexture>("earthmap.jpg"));
    //world.Add(make_shared<Sphere>(Position(400, 200, 400), 100, emat));
    shared_ptr<NoiseTexture> pertext = make_shared<NoiseTexture>(0.65);
    world.Add(make_shared<Sphere>(Position(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

    HittableCollection boxes2;
    shared_ptr<Lambertian> white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    for (int j = 0; j < clusterAmount; j++)
    {
        boxes2.Add(make_shared<Sphere>(Position::Random(0, 165), 10, white));
    }

    world.Add(make_shared<Translate>(make_shared<RotateY>(make_shared<BVHNode>(boxes2), 15), Vector3(-100, 270, 395)));

    //light source
    HittableCollection lights;
    shared_ptr<Materials> lightMat = shared_ptr<Materials>();
    lights.Add(make_shared<Quadrilaterals>(Position(343, 554, 332), Vector3(-130, 0, 0), Vector3(0, 0, 0), lightMat));

    //Camera(double imageWidth, double ratio, int samplePerPixel, int bounces, double fov, Position lookfrom, Position lookat, Vector3 upVector, double defocus_Angle, double focusDistance, bg)
    Camera camera(imageWidth, 1.0, samplePerPixel, bounces, 27, Position(478, 278, -1200), Position(278, 278, 0), Vector3(0, 1, 0), 0, 100, Color(0, 0, 0));
    camera.Render(world, lights);
}

int main(int argc, char* argv[])
{
    switch (8)
    {
        //BaseBalls(Set 2 (3 Different Balls) = 1, Reflective = 1 / Transparent = 0)
    case 1: BaseBalls(1, 0);
        break;
    case 2: Checkers();
        break;
        //Amount of spheres (smol), recomended 7~11
    case 3: RandomSpheres(7);
        break;
    case 4: //Earth();
        break;
    case 5: PerlinSphere();
        break;
    case 6: Quads();
        break;
    case 7: SimpleLight();
        break;
    case 8: CornellBox();
        break;
    case 9: CornellSmoke();
        break;
        //FinalSceneB2(int imageWidth, int samplePerPixel, int bounces, int floorAmount, int clusterAmount)
    case 10: FinalSceneB2(600, 200, 50, 20, 500);
        //case 10: FinalSceneB2(600, 2000, 50, 20, 500); <--this took 5 hours to render
        break;
    default: FinalSceneB2(400, 70, 30, 20, 100); //switch(0) for default
        break;
    }

    return 0;
}
