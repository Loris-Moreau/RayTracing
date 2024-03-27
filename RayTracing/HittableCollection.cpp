#include "HittableCollection.h"

using namespace std;

HittableCollection::HittableCollection(const std::shared_ptr<Hittable>& hittable)
{
    Add(hittable);
}

void HittableCollection::Clear()
{
    mPool.clear();
}

void HittableCollection::Add(const std::shared_ptr<Hittable>& hittable)
{
    mPool.push_back(hittable);
    bBox = AABB(bBox, hittable->BoundingBox());
}

bool HittableCollection::Hit(const Ray& rRay, const Interval rayTime, HitInfo& hitInfo) const
{
    HitInfo tempInfo;
    bool hasHit = false;
    auto closestHit = rayTime.max;

    for (const shared_ptr<Hittable>& hittable : mPool)
    {
        if (hittable->Hit(rRay, Interval(rayTime.min, closestHit), tempInfo))
        {
            hasHit = true;
            closestHit = tempInfo.time;
            hitInfo = tempInfo;
        }
    }

    return hasHit;
}

AABB HittableCollection::BoundingBox() const
{
    return bBox;
}
