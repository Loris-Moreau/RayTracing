#include "Hittable.h"

void HitInfo::SetFaceNormal(const Ray& ray, const Vector3& outwardNormal)
{
	//NOTE : The outwardNormal vector should be unit length
	frontFace = Dot(ray.GetDirection(), outwardNormal) < 0;
	normal = frontFace ? outwardNormal : -outwardNormal;
}

Translate::Translate(shared_ptr<Hittable> position, const Vector3& displacement)
    : object(position), offset(displacement)
{
    bBox = object->BoundingBox() + offset;
}

bool Translate::Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const
{
    // Move the ray backwards by the offset
    Ray offsetRay(ray.GetOrigin() - offset, ray.GetDirection(), ray.time());

    // Determine where (if any) an intersection occurs along the offset ray
    if (!object->Hit(offsetRay, rayTime, hitInfo))
        return false;

    // Move the intersection point forwards by the offset
    hitInfo.coordinates += offset;

    return true;
}

RotateY::RotateY(shared_ptr<Hittable> position, double angle) : object(position)
{
    double radians = DegToRad(angle);
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    bBox = object->BoundingBox();

    Position min(infinity, infinity, infinity);
    Position max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i * bBox.x.max + (1 - i) * bBox.x.min;
                auto y = j * bBox.y.max + (1 - j) * bBox.y.min;
                auto z = k * bBox.z.max + (1 - k) * bBox.z.min;

                auto newx = cosTheta * x + sinTheta * z;
                auto newz = -sinTheta * x + cosTheta * z;

                Vector3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bBox = AABB(min, max);
}

bool RotateY::Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const
{
    // Change the ray from world space to object space
    Position origin = ray.GetOrigin();
    Vector3 direction = ray.GetDirection();

    origin[0] = cosTheta * ray.GetOrigin()[0] - sinTheta * ray.GetOrigin()[2];
    origin[2] = sinTheta * ray.GetOrigin()[0] + cosTheta * ray.GetOrigin()[2];

    direction[0] = cosTheta * ray.GetDirection()[0] - sinTheta * ray.GetDirection()[2];
    direction[2] = sinTheta * ray.GetDirection()[0] + cosTheta * ray.GetDirection()[2];

    Ray rotatedRay(origin, direction, ray.time());

    // Determine where (if any) an intersection occurs in object space
    if (!object->Hit(rotatedRay, rayTime, hitInfo))
        return false;

    // Change the intersection point from object space to world space
    Position _position = hitInfo.coordinates;
    _position[0] = cosTheta * hitInfo.coordinates[0] + sinTheta * hitInfo.coordinates[2];
    _position[2] = -sinTheta * hitInfo.coordinates[0] + cosTheta * hitInfo.coordinates[2];

    // Change the normal from object space to world space
    Vector3 normal = hitInfo.normal;
    normal[0] = cosTheta * hitInfo.normal[0] + sinTheta * hitInfo.normal[2];
    normal[2] = -sinTheta * hitInfo.normal[0] + cosTheta * hitInfo.normal[2];

    hitInfo.coordinates = _position;
    hitInfo.normal = normal;

    return true;
}

double Hittable::PDFValue(const Position& o, const Vector3& v) const
{
    return 0.0;
}

Vector3 Hittable::Random(const Vector3& o) const
{
    return Vector3(1, 0, 0);
}
