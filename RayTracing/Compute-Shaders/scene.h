#pragma once

#include "Maths/math_util.h"
#include "../simd.h"

#if defined(PLATFORM_WIN32) && defined(WIN32_GPU)
#define ALIGN_GPU __declspec(align(16))
#else
#define ALIGN_GPU 
#endif // PLATFORM_WIN32

ALIGN_GPU struct Material
{
    Vector3 color{ 0.0f, 0.0f, 0.0f };
    float refractiveIndex; // Refractive index of material. 0 means no refraction.
    Vector3 emitColor{ 0.0f, 0.0f, 0.0f };
    float reflection; // 0 is pure diffuse, 1 is mirror.
};

ALIGN_GPU struct Sphere
{
    Vector3 position;
    float radius;
    uint32_t materialIndex;
};

struct SphereSoALane
{
    LaneVector3 position;
    LaneF32 radiusSquared;
    LaneF32 materialIndex;
};

ALIGN_GPU struct Plane
{
    Vector3 normal;
    float d;
    uint32_t materialIndex;
};

// XY plane axis-aligned rectangle
// RULE: Default Rectangle are 2 unit square, and it's center point always on the origin.
// Use transform matrices to manipulate position, scale and orientation.
static const Vector3 rectDefaultMinPoint{ -1.0f, -1.0f, 0.0f };
static const Vector3 rectDefaultMaxPoint{  1.0f,  1.0f, 0.0f };
ALIGN_GPU struct RectangleXY
{
    Matrix4 transformMatrix;
    Vector3 normal{ 0.0f, 0.0, 1.0f };
    uint32_t materialIndex;
};

struct RectangleLane
{
    LaneMatrix4 transformMatrix;
    LaneVector3 normal{ 0.0f, 0.0, 1.0f };
    LaneF32 materialIndex;
};

static const Vector3 XAxis = Vector3(1.0f, 0.0f, 0.0f);
static const Vector3 YAxis = Vector3(0.0f, 1.0f, 0.0f);
static const Vector3 ZAxis = Vector3(0.0f, 0.0f, 1.0f);

static void RotateRectangle(RectangleXY* rect, Vector3 axis, float angle)
{
    Matrix4 rotationMatrix = IdentityMatrix;
    if (axis == XAxis)
    {
        RotateMatrixXAxis(rotationMatrix, angle);
    } else if (axis == YAxis)
    {
        RotateMatrixYAxis(rotationMatrix, angle);
    } else if (axis == ZAxis)
    {
        RotateMatrixZAxis(rotationMatrix, angle);
    } else
    {
        throw("Rotation around arbitary axis not supported yet");
    }

    //Vector3 rotatedNormal = (rotationMatrix * Vector4(rect->normal, 0.0f)).xyz();

    ///
    // For some reason the multiplication of matrix4 & vec4 needs to be here and doesn't want to be pulled from maths_utils.h
    Vector4 result;
    Vector4 right = Vector4(rect->normal, 0.0f);
    result.x = DotProduct(rotationMatrix[0], right);
    result.y = DotProduct(rotationMatrix[1], right);
    result.z = DotProduct(rotationMatrix[2], right);
    result.w = DotProduct(rotationMatrix[3], right);
    Vector3 rotatedNormal = result.xyz();
    ///
    
    rect->normal = rotatedNormal;
    rect->transformMatrix = rotationMatrix * rect->transformMatrix;
}

static RectangleXY CreateRectangle(Vector3 position, Vector3 scale, uint32_t materialIndex, Vector3 initialRotationAxis = Vector3(0.0f, 0.0f, 0.0f), float initialRotationAngle = 0.0f)
{
    RectangleXY result = {};
    Matrix4 scaleMatrixRect = IdentityMatrix;
    Matrix4 translateMatrixRect = IdentityMatrix;
    Matrix4 rotationMatrixRect = IdentityMatrix;

    ScaleMatrix(scaleMatrixRect, scale);
    TranslateMatrix(translateMatrixRect, position);
    if (initialRotationAxis == XAxis)
    {
        RotateMatrixXAxis(rotationMatrixRect, initialRotationAngle);
    }
    else if (initialRotationAxis == YAxis)
    {
        RotateMatrixYAxis(rotationMatrixRect, initialRotationAngle);
    }
    else if (initialRotationAxis == ZAxis)
    {
        RotateMatrixZAxis(rotationMatrixRect, initialRotationAngle);
    }
    else {
        // "Rotation around arbitrary axis not supported yet"
    }

    result.transformMatrix = translateMatrixRect * rotationMatrixRect;
    result.transformMatrix = result.transformMatrix * scaleMatrixRect;
    
    //result.normal = (rotationMatrixRect * Vector4(result.normal, 0.0f)).xyz();
    ///
    // For some reason the multiplication of matrix4 & vec4 needs to be here and doesn't want to be pulled from maths_utils.h
    Vector4 MxVResult;
    Vector4 right = Vector4(result.normal, 0.0f);
    MxVResult.x = DotProduct(rotationMatrixRect[0], right);
    MxVResult.y = DotProduct(rotationMatrixRect[1], right);
    MxVResult.z = DotProduct(rotationMatrixRect[2], right);
    MxVResult.w = DotProduct(rotationMatrixRect[3], right);
    result.normal = MxVResult.xyz();
    ///
    
    result.materialIndex = materialIndex;

    return result;
}


struct Box
{
    Vector3 position;
    RectangleXY rectangles[6];
};

static Box CreateBox(Vector3 position, Vector3 scale, uint32_t materialIndex)
{
    Box result = {};

    Vector3 topRectPosition = position;
    topRectPosition.y += scale.y;
    RectangleXY topRect = CreateRectangle(topRectPosition, Vector3(scale.x, scale.z, 1.0f), materialIndex, XAxis, -HALF_PI);

    Vector3 bottomRectPosition = position;
    bottomRectPosition.y -= scale.y;
    RectangleXY bottomRect = CreateRectangle(bottomRectPosition, Vector3(scale.x, scale.z, 1.0f), materialIndex, XAxis, -HALF_PI);

    Vector3 rightRectPosition = position;
    rightRectPosition.x += scale.x;
    RectangleXY rightRect = CreateRectangle(rightRectPosition, Vector3(scale.z, scale.y, 1.0f), materialIndex, YAxis, -HALF_PI);

    Vector3 leftRectPosition = position;
    leftRectPosition.x -= scale.x;
    RectangleXY leftRect = CreateRectangle(leftRectPosition, Vector3(scale.z, scale.y, 1.0f), materialIndex, YAxis, HALF_PI);

    Vector3 backRectPosition = position;
    backRectPosition.z -= scale.z;
    RectangleXY backRect = CreateRectangle(backRectPosition, Vector3(scale.x, scale.y, 1.0f), materialIndex);

    Vector3 frontRectPosition = position;
    frontRectPosition.z += scale.z;
    RectangleXY frontRect = CreateRectangle(frontRectPosition, Vector3(scale.x, scale.y, 1.0f), materialIndex);

    result.position = position;
    result.rectangles[0] = frontRect;
    result.rectangles[1] = backRect;
    result.rectangles[2] = rightRect;
    result.rectangles[3] = leftRect;
    result.rectangles[4] = topRect;
    result.rectangles[5] = bottomRect;

    return result;
}

static void RotateBox(Box* box, Vector3 axis, float angle)
{
    for (uint32_t rectangleIndex = 0; rectangleIndex < 6; ++rectangleIndex)
    {
        RectangleXY* rect = box->rectangles + rectangleIndex;

        // To be able to rotate all rectangles around the boxes origin, we need to translate back to their position. 
        Matrix4 translateMatrix = IdentityMatrix;
        TranslateMatrix(translateMatrix, box->position);

        Matrix4 inverseOfTranslateMatrix = Inverse(translateMatrix);
        rect->transformMatrix = inverseOfTranslateMatrix * rect->transformMatrix;
        RotateRectangle(rect, axis, angle);
        rect->transformMatrix = translateMatrix * rect->transformMatrix;
    }
}

struct Camera
{
    Vector3 position;
    Vector3 zVec;
    Vector3 yVec;
    Vector3 xVec;

    Camera(Vector3 cameraPosition)
    {
        position = cameraPosition;
        zVec = Normalize(cameraPosition);
        xVec = Normalize(CrossProduct(Vector3(0.0f, 1.0f, 0.0f), zVec));
        yVec = Normalize(CrossProduct(zVec, xVec));
    }
};

struct World
{
    uint32_t materialCount;
    Material* materials;
    uint32_t planeCount;
    Plane* planes;
    uint32_t sphereCount;
    Sphere* spheres;
    uint32_t sphereSoAArrayCount;
    SphereSoALane* sphereSoAArray;
    uint32_t rectangleCount;
    RectangleXY* rectangles;
    uint32_t rectangleLaneArrayCount;
    RectangleLane* rectangleLaneArray;
    Camera* camera;
};

World* createScene()
{
    // Y is up.
    Vector3 globalUpVector = Vector3(0.0f, 1.0f, 0.0f);

    // Generate scene
    Plane* plane = new Plane;
    plane->normal = globalUpVector;
    plane->d = 0;
    plane->materialIndex = 1;

    Sphere sphere = {};
    sphere.position = Vector3(0.0f, 1.0f, 0.0f);
    sphere.radius = 1.0f;
    sphere.materialIndex = 2;

    Sphere sphere2 = {};
    sphere2.position = Vector3(-2.0f, 1.0f, 0.0f);
    sphere2.radius = 1.0f;
    sphere2.materialIndex = 3;

    Sphere sphere3 = {};
    sphere3.position = Vector3(-4.0f, 2.0f, 1.0f);
    sphere3.radius = 1.0f;
    sphere3.materialIndex = 4;

    Sphere sphere4 = {};
    sphere4.position = Vector3(2.0f, 1.0f, -1.0f);
    sphere4.radius = 1.0f;
    sphere4.materialIndex = 5;

    Sphere sphere5 = {};
    sphere5.position = Vector3(-2.0f, 1.0f, 5.0f);
    sphere5.radius = 1.0f;
    sphere5.materialIndex = 2;

    Sphere sphere6 = {};
    sphere6.position = Vector3(5.0f, 2.0f, -6.0f);
    sphere6.radius = 2.0f;
    sphere6.materialIndex = 3;

    Sphere sphere7 = {};
    sphere7.position = Vector3(-4.0f, 4.0f, 5.0f);
    sphere7.radius = 1.0f;
    sphere7.materialIndex = 6;

    Sphere sphere8 = {};
    sphere8.position = Vector3(2.0f, 1.0f, 4.0f);
    sphere8.radius = 1.0f;
    sphere8.materialIndex = 5;

    const uint32_t sphereCount = 8;
    Sphere* spheres = new Sphere[sphereCount];
    spheres[0] = sphere;
    spheres[1] = sphere2;
    spheres[2] = sphere3;
    spheres[3] = sphere4;
    spheres[4] = sphere5;
    spheres[5] = sphere6;
    spheres[6] = sphere7;
    spheres[7] = sphere8;


    // We use AoSoA layout for sphere data. fixed simd-lane size arrays of each member.
    const uint32_t sphereSoAArrayCount = (sphereCount + LANE_WIDTH - 1) / LANE_WIDTH;
    SphereSoALane* sphereSoAArray = new SphereSoALane[sphereSoAArrayCount];

    for (int i = 0; i < sphereSoAArrayCount; ++i)
    {
        ALIGN_LANE float spheresPositionX[LANE_WIDTH];
        ALIGN_LANE float spheresPositionY[LANE_WIDTH];
        ALIGN_LANE float spheresPositionZ[LANE_WIDTH];
        ALIGN_LANE float spheresRadiusSquared[LANE_WIDTH];
        ALIGN_LANE float spheresMaterialIndex[LANE_WIDTH];

        uint32_t remainingSpheres = sphereCount - i * LANE_WIDTH;
        uint32_t len = (remainingSpheres / LANE_WIDTH) > 0 ? LANE_WIDTH : remainingSpheres % LANE_WIDTH;
        for (int j = 0; j < len; ++j)
        {
            uint32_t sphereIndex = j + i * LANE_WIDTH;
            Sphere s = spheres[sphereIndex];
            spheresPositionX[j] = s.position.x;
            spheresPositionY[j] = s.position.y;
            spheresPositionZ[j] = s.position.z;
            spheresRadiusSquared[j] = s.radius * s.radius;
            spheresMaterialIndex[j] = s.materialIndex;
        }

        SphereSoALane sphereSoA = {};
        sphereSoA.position = LaneVector3(LaneF32(spheresPositionX), LaneF32(spheresPositionY), LaneF32(spheresPositionZ));
        sphereSoA.radiusSquared = LaneF32(spheresRadiusSquared);
        sphereSoA.materialIndex = LaneF32(spheresMaterialIndex);

        sphereSoAArray[i] = sphereSoA;
    }

    Material defaultMaterial = {};
    //defaultMaterial.emitColor = Vector3(0.1f, 0.2f, 0.4f);

    Material planeMaterial = {};
    planeMaterial.color = Vector3(0.8f, 0.8f, 0.8f);

    Material sphereMaterial = {};
    sphereMaterial.color = Vector3(0.8f, 0.3f, 0.3f);

    Material sphere2Material = {};
    sphere2Material.color = Vector3(1.0f, 1.0f, 1.0f) * 0.9f;
    sphere2Material.reflection = 1.0f;

    Material sphere3Material = {};
    sphere3Material.color = Vector3(0.8f, 0.6f, 0.2f);
    sphere3Material.reflection = 0.9f;

    Material sphere4Material = {};
    sphere4Material.color = Vector3(1.0f, 1.0f, 1.0f) * 0.9f;
    sphere4Material.refractiveIndex = 1.5f;
    sphere4Material.reflection = 1.0f;

    Material sphere5Material = {};
    sphere5Material.emitColor = Vector3(20.0f, 15.0f, 10.0f) * 0.2f;

    Material blueLightMaterial = {};
    blueLightMaterial.emitColor = Vector3(1.0f, 1.0f, 18.0f);

    uint32_t materialCount = 8;
    Material* materials = new Material[materialCount];
    materials[0] = defaultMaterial;
    materials[1] = planeMaterial;
    materials[2] = sphereMaterial;
    materials[3] = sphere2Material;
    materials[4] = sphere3Material;
    materials[5] = sphere4Material;
    materials[6] = sphere5Material;
    materials[7] = blueLightMaterial;

    Camera* camera = new Camera(Vector3(0.0f, 4.0f, 10.0f));

    // I used raw pointers for scene objects. Freeing heap memory is callers responsibility.
    // TODO: I should use smart pointers for scene objects, I will do that later. 
    // Memory automatically will be freed after program terminated (it better has to, or else).
    World* world = new World;
    world->materialCount = materialCount;
    world->materials = materials;
    world->planeCount = 1;
    world->planes = plane;
    world->sphereCount = sphereCount;
    world->sphereSoAArrayCount = sphereSoAArrayCount;
    world->sphereSoAArray = sphereSoAArray;
    world->spheres = spheres;
    world->rectangleCount = 0;
    world->camera = camera;

    return world;
}

World* CreateCornellBoxScene()
{
    Vector3 globalUpVector = Vector3(0.0f, 1.0f, 0.0f);

    Material defaultMaterial = {};
    //defaultMaterial.emitColor = Vector3(0.1f, 0.2f, 0.4f);

    Material whiteDiffuseMaterial = {};
    whiteDiffuseMaterial.color = Vector3(0.73f, 0.73f, 0.73f);
    
    Material greenDiffuseMaterial = {};
    greenDiffuseMaterial.color = Vector3(0.12f, 0.45f, 0.15f);
    greenDiffuseMaterial.reflection = 0.0f;
    
    Material redDiffuseMaterial = {};
    redDiffuseMaterial.color = Vector3(0.65f, 0.05f, 0.05f);
    redDiffuseMaterial.reflection = 0.0f;

    Material whiteLightMaterial = {};
    whiteLightMaterial.emitColor = Vector3(15.0f, 15.0f, 15.0f) * 0.75f;

    uint32_t materialCount = 5;
    Material* materials = new Material[materialCount];
    materials[0] = defaultMaterial;
    materials[1] = whiteDiffuseMaterial;
    materials[2] = greenDiffuseMaterial;
    materials[3] = redDiffuseMaterial;
    materials[4] = whiteLightMaterial;

    // Initialize rectangles
    // RectangleXY lightRect = CreateRectangle(Vector3(0.0f, 11.0f, -9.5f), Vector3(10.0f, 4.0f, 1.0f), 4, XAxis, -HALF_PI);
    RectangleXY lightRect = CreateRectangle(Vector3(0.0f, 7.99f, -6.0f), Vector3(4.0f, 4.0f, 1.0f), 4, XAxis, -HALF_PI);
    RectangleXY bottomRect = CreateRectangle(Vector3(0.0f, -8.0f, -8.0f), Vector3(8.0f, 10.0f, 1.0f), 1, XAxis, -HALF_PI);
    RectangleXY rightRect = CreateRectangle(Vector3(8.0f, 0.0f, -8.0f), Vector3(10.0f, 8.0f, 1.0f), 3, YAxis, -HALF_PI);
    RectangleXY leftRect = CreateRectangle(Vector3(-8.0f, 0.0f, -8.0f), Vector3(10.0f, 8.0f, 1.0f), 2, YAxis, HALF_PI);
    RectangleXY backRect = CreateRectangle(Vector3(0.0f, 0.0f, -14.0f), Vector3(8.0f, 8.0f, 1.0f), 1);
    RectangleXY topRect = CreateRectangle(Vector3(0.0f, 8.0f, -8.0f), Vector3(8.0f, 10.0f, 1.0f), 1, XAxis, -HALF_PI);

    Box box = CreateBox(Vector3(2.0f, -6.0f, -3.0f), Vector3(2.0f, 2.0f, 2.0f), 1);
    RotateBox(&box, Vector3(0.0f, 1.0f, 0.0f), -0.3f);

    Box box2 = CreateBox(Vector3(-2.0f, -4.0f, -8.0f), Vector3(2.0f, 4.0f, 2.0f), 1);
    RotateBox(&box2, Vector3(0.0f, 1.0f, 0.0f), 0.3f);

    uint32_t rectangleCount = 18;
    RectangleXY* rectangles = new RectangleXY[rectangleCount];
    rectangles[0] = lightRect;
    rectangles[1] = bottomRect;
    rectangles[2] = rightRect;
    rectangles[3] = leftRect;
    rectangles[4] = backRect;
    rectangles[5] = topRect;
    rectangles[6] = box.rectangles[0];
    rectangles[7] = box.rectangles[1];
    rectangles[8] = box.rectangles[2];
    rectangles[9] = box.rectangles[3];
    rectangles[10] = box.rectangles[4];
    rectangles[11] = box.rectangles[5];
    rectangles[12] = box2.rectangles[0];
    rectangles[13] = box2.rectangles[1];
    rectangles[14] = box2.rectangles[2];
    rectangles[15] = box2.rectangles[3];
    rectangles[16] = box2.rectangles[4];
    rectangles[17] = box2.rectangles[5];

    for (uint32_t rectangleIndex = 0; rectangleIndex < rectangleCount; ++rectangleIndex)
    {
        RectangleXY* rect = rectangles + rectangleIndex;
        rect->transformMatrix = Inverse(rect->transformMatrix);
    }

    // We use AoSoA layout for rectangle data. fixed simd-lane size arrays of each member.
    const uint32_t rectangleLaneArrayCount = (rectangleCount + LANE_WIDTH - 1) / LANE_WIDTH;
    RectangleLane* rectangleLaneArray = new RectangleLane[rectangleLaneArrayCount];

    for (int i = 0; i < rectangleLaneArrayCount; ++i)
    {
        ALIGN_LANE float rectanglesTransformMatrixArray[4][4][LANE_WIDTH];
        ALIGN_LANE float rectanglesNormal[3][LANE_WIDTH];
        ALIGN_LANE float rectanglesMaterialIndex[LANE_WIDTH];

        // Put scalar rectangle values into arrays
        uint32_t remainingRectangles = rectangleCount - i * LANE_WIDTH;
        uint32_t len = (remainingRectangles / LANE_WIDTH) > 0 ? LANE_WIDTH : remainingRectangles % LANE_WIDTH;
        for (int j = 0; j < len; ++j)
        {
            uint32_t rectangleIndex = j + i * LANE_WIDTH;
            RectangleXY* rect = rectangles + rectangleIndex;

            for (uint32_t rowIndex = 0; rowIndex < 4; ++rowIndex)
            {
                for (uint32_t columnIndex = 0; columnIndex < 4; ++columnIndex)
                {
                    rectanglesTransformMatrixArray[rowIndex][columnIndex][j] = rect->transformMatrix[rowIndex][columnIndex];
                }
            }

            rectanglesNormal[0][j] = rect->normal.x;
            rectanglesNormal[1][j] = rect->normal.y;
            rectanglesNormal[2][j] = rect->normal.z;
            rectanglesMaterialIndex[j] = rect->materialIndex;
        }

        // Put those arrays into SIMD registers.
        RectangleLane rectangleLane = {};
        rectangleLane.transformMatrix = LaneMatrix4(rectanglesTransformMatrixArray);
        rectangleLane.normal = LaneVector3(rectanglesNormal);
        rectangleLane.materialIndex = LaneF32(rectanglesMaterialIndex);

        rectangleLaneArray[i] = rectangleLane;
    }


    Camera* camera = new Camera(Vector3(0.0f, 1.0f, 20.0f));

    // I used raw pointers for scene objects. Freeing heap memory is callers responsibility. 
    // TODO: I should use smart pointers for scene objects but I don't want to do that now. 
    // Memory automatically will be freed after program terminated (it better has to, or else).
    World* world = new World;
    world->materialCount = materialCount;
    world->materials = materials;
    world->planeCount = 0;
    world->sphereCount = 0;
    world->sphereSoAArrayCount = 0;
    world->rectangleCount = rectangleCount;
    world->rectangles = rectangles;
    world->rectangleLaneArrayCount = rectangleLaneArrayCount;
    world->rectangleLaneArray = rectangleLaneArray;
    world->camera = camera;

    return world;
}

World* createSceneFinalB2()
{
    Vector3 globalUpVector = Vector3(0.0f, 1.0f, 0.0f);

    Material defaultMaterial = {};
    //defaultMaterial.emitColor = Vector3(0.1f, 0.2f, 0.4f) * 0.01f;

    Material whiteDiffuseMaterial = {};
    whiteDiffuseMaterial.color = Vector3(0.73f, 0.73f, 0.73f);
    whiteDiffuseMaterial.reflection = 0.25f;
    
    Material greenDiffuseMaterial = {};
    greenDiffuseMaterial.color = Vector3(0.12f, 0.45f, 0.15f);
    greenDiffuseMaterial.reflection = 0.0f;
    
    Material redDiffuseMaterial = {};
    redDiffuseMaterial.color = Vector3(0.65f, 0.05f, 0.05f);
    redDiffuseMaterial.reflection = 0.0f;

    Material whiteLightMaterial = {};
    whiteLightMaterial.emitColor = Vector3(15.0f, 15.0f, 15.0f) * 0.75f ;
    
    // Base Sphere
    Material sphereMaterial = {};
    sphereMaterial.color = Vector3(0.7f, 0.3f, 0.1f);

    // Shiny Metal
    Material shinyMetalMaterial = {};
    shinyMetalMaterial.color = Vector3(1.0f, 1.0f, 1.0f) * 0.9f;
    shinyMetalMaterial.reflection = 1.0f;

    // Fuzzy Metal
    Material fuzzyMetalMaterial = {};
    fuzzyMetalMaterial.color = Vector3(0.8f, 0.8f, 0.9f);
    fuzzyMetalMaterial.reflection = 0.87f;

    // Glass (reflective)
    Material glassMaterial = {};
    glassMaterial.color = Vector3(1.0f, 1.0f, 1.0f) * 0.9f;
    glassMaterial.refractiveIndex = 1.5f;
    glassMaterial.reflection = 1.0f;
    
    // Sky/Ambience Light
    Material blueLightMaterial = {};
    blueLightMaterial.color = Vector3(0.0f, 0.0f, 0.95f);
    blueLightMaterial.reflection = 0.15f;
    
    uint32_t materialCount = 10;
    Material* materials = new Material[materialCount];
    materials[0] = defaultMaterial;
    materials[1] = whiteDiffuseMaterial;
    materials[2] = greenDiffuseMaterial;
    materials[3] = redDiffuseMaterial;
    materials[4] = whiteLightMaterial;
    materials[5] = sphereMaterial;
    materials[6] = shinyMetalMaterial;
    materials[7] = fuzzyMetalMaterial;
    materials[8] = glassMaterial;
    materials[9] = blueLightMaterial;
    
    /// ----------------------------------------------------------------------------------------------------------------
    
    // Initialize Spheres
    // base sphere
    Sphere sphere1 = {};
    sphere1.position = Vector3(-4.5f, 3.5f, -3.0f);
    sphere1.radius = 1.0f;
    sphere1.materialIndex = 5;
    
    // metal shiny
    Sphere sphere2 = {};
    sphere2.position = Vector3(-6.5f, -5.5f, -6.0f);
    sphere2.radius = 2.5f;
    sphere2.materialIndex = 6;
    
    // Glass 
    Sphere sphere3 = {};
    sphere3.position = Vector3(0.0f, -6.5f, -0.5f);
    sphere3.radius = 2.0f;
    sphere3.materialIndex = 8;
    
    // fuzzy metal
    Sphere sphere4 = {};
    sphere4.position = Vector3(0.5f, 0.0f, -15.0f);
    sphere4.radius = 3.0f;
    sphere4.materialIndex = 7;
    
    // sphere
    Sphere sphere5 = {};
    sphere5.position = Vector3(-4.5f, -1.0f, -5.0f);
    sphere5.radius = 1.5f;
    sphere5.materialIndex = 9;

    // Glass 
    Sphere sphere6 = {};
    sphere6.position = Vector3(2.0f, -3.5f, -8.0f);
    sphere6.radius = 2.0f;
    sphere6.materialIndex = 8;

    // metal shiny
    Sphere sphere7 = {};
    sphere7.position = Vector3(6.5f, 4.5f, -15.0f);
    sphere7.radius = 2.5f;
    sphere7.materialIndex = 6;
    
    const uint32_t sphereCount = 7;
    Sphere* spheres = new Sphere[sphereCount];
    spheres[0] = sphere1;
    spheres[1] = sphere2;
    spheres[2] = sphere3;
    spheres[3] = sphere4;
    spheres[4] = sphere5;
    spheres[5] = sphere6;
    spheres[6] = sphere7;


    // We use AoSoA layout for sphere data. fixed simd-lane size arrays of each member.
    const uint32_t sphereSoAArrayCount = (sphereCount + LANE_WIDTH - 1) / LANE_WIDTH;
    SphereSoALane* sphereSoAArray = new SphereSoALane[sphereSoAArrayCount];

    for (int i = 0; i < sphereSoAArrayCount; ++i)
    {
        ALIGN_LANE float spheresPositionX[LANE_WIDTH];
        ALIGN_LANE float spheresPositionY[LANE_WIDTH];
        ALIGN_LANE float spheresPositionZ[LANE_WIDTH];
        ALIGN_LANE float spheresRadiusSquared[LANE_WIDTH];
        ALIGN_LANE float spheresMaterialIndex[LANE_WIDTH];

        uint32_t remainingSpheres = sphereCount - i * LANE_WIDTH;
        uint32_t len = (remainingSpheres / LANE_WIDTH) > 0 ? LANE_WIDTH : remainingSpheres % LANE_WIDTH;
        for (int j = 0; j < len; ++j)
        {
            uint32_t sphereIndex = j + i * LANE_WIDTH;
            Sphere s = spheres[sphereIndex];
            spheresPositionX[j] = s.position.x;
            spheresPositionY[j] = s.position.y;
            spheresPositionZ[j] = s.position.z;
            spheresRadiusSquared[j] = s.radius * s.radius;
            spheresMaterialIndex[j] = s.materialIndex;
        }

        SphereSoALane sphereSoA = {};
        sphereSoA.position = LaneVector3(LaneF32(spheresPositionX), LaneF32(spheresPositionY), LaneF32(spheresPositionZ));
        sphereSoA.radiusSquared = LaneF32(spheresRadiusSquared);
        sphereSoA.materialIndex = LaneF32(spheresMaterialIndex);

        sphereSoAArray[i] = sphereSoA;
    }

    /// ----------------------------------------------------------------------------------------------------------------
    
    // Initialize rectangles
    RectangleXY lightRect = CreateRectangle(Vector3(0.0f, 11.0f, -9.5f), Vector3(10.0f, 4.0f, 1.0f), 4, XAxis, -HALF_PI);
    RectangleXY bottomRect = CreateRectangle(Vector3(0.0f, -8.0f, -35.0f), Vector3(40.0f, 40.0f, 1.0f), 2, XAxis, -HALF_PI);

    Box box = CreateBox(Vector3(10.0f, -6.0f, -15.0f), Vector3(2.0f, 2.0f, 2.0f), 1);
    RotateBox(&box, Vector3(0.0f, 1.0f, 0.0f), 10.0f);
    
    uint32_t rectangleCount = 8;
    RectangleXY* rectangles = new RectangleXY[rectangleCount];
    rectangles[0] = lightRect;
    rectangles[1] = bottomRect;
    rectangles[2] = box.rectangles[0];
    rectangles[3] = box.rectangles[1];
    rectangles[4] = box.rectangles[2];
    rectangles[5] = box.rectangles[3];
    rectangles[6] = box.rectangles[4];
    rectangles[7] = box.rectangles[5];

    for (uint32_t rectangleIndex = 0; rectangleIndex < rectangleCount; ++rectangleIndex)
    {
        RectangleXY* rect = rectangles + rectangleIndex;
        rect->transformMatrix = Inverse(rect->transformMatrix);
    }

    // We use AoSoA layout for rectangle data. fixed simd-lane size arrays of each member.
    const uint32_t rectangleLaneArrayCount = (rectangleCount + LANE_WIDTH - 1) / LANE_WIDTH;
    RectangleLane* rectangleLaneArray = new RectangleLane[rectangleLaneArrayCount];

    for (int i = 0; i < rectangleLaneArrayCount; ++i)
    {
        ALIGN_LANE float rectanglesTransformMatrixArray[4][4][LANE_WIDTH];
        ALIGN_LANE float rectanglesNormal[3][LANE_WIDTH];
        ALIGN_LANE float rectanglesMaterialIndex[LANE_WIDTH];

        // Put scalar rectangle values into arrays
        uint32_t remainingRectangles = rectangleCount - i * LANE_WIDTH;
        uint32_t len = (remainingRectangles / LANE_WIDTH) > 0 ? LANE_WIDTH : remainingRectangles % LANE_WIDTH;
        for (int j = 0; j < len; ++j)
        {
            uint32_t rectangleIndex = j + i * LANE_WIDTH;
            RectangleXY* rect = rectangles + rectangleIndex;

            for (uint32_t rowIndex = 0; rowIndex < 4; ++rowIndex)
            {
                for (uint32_t columnIndex = 0; columnIndex < 4; ++columnIndex)
                {
                    rectanglesTransformMatrixArray[rowIndex][columnIndex][j] = rect->transformMatrix[rowIndex][columnIndex];
                }
            }

            rectanglesNormal[0][j] = rect->normal.x;
            rectanglesNormal[1][j] = rect->normal.y;
            rectanglesNormal[2][j] = rect->normal.z;
            rectanglesMaterialIndex[j] = rect->materialIndex;
        }

        // Put those arrays into SIMD registers.
        RectangleLane rectangleLane = {};
        rectangleLane.transformMatrix = LaneMatrix4(rectanglesTransformMatrixArray);
        rectangleLane.normal = LaneVector3(rectanglesNormal);
        rectangleLane.materialIndex = LaneF32(rectanglesMaterialIndex);

        rectangleLaneArray[i] = rectangleLane;
    }

    /// ----------------------------------------------------------------------------------------------------------------
    
    Camera* camera = new Camera(Vector3(0.0f, 1.0f, 20.0f));

    // I used raw pointers for scene objects. Freeing heap memory is callers responsibility. 
    // TODO: I should use smart pointers for scene objects but I don't want to do that now. 
    // Memory automatically will be freed after program terminated (it better has to, or else).
    World* world = new World;
    world->materialCount = materialCount;
    world->materials = materials;
    world->planeCount = 0;
    world->sphereCount = sphereCount;
    world->sphereSoAArrayCount = sphereSoAArrayCount;
    world->sphereSoAArray = sphereSoAArray;
    world->spheres = spheres;
    world->rectangleCount = rectangleCount;
    world->rectangles = rectangles;
    world->rectangleLaneArrayCount = rectangleLaneArrayCount;
    world->rectangleLaneArray = rectangleLaneArray;
    world->camera = camera;

    return world;
}
