#include "BVHNode.h"

BVHNode::BVHNode(const std::vector<shared_ptr<Hittable>>& srcObjects, size_t start, size_t end)
{
    {
        std::vector<shared_ptr<Hittable>> objects = srcObjects; // Create a modifiable array of the source scene objects

        int axis = RandomInt(0, 2);
        auto comparator = (axis == 0) ? BoxXCompare
            : (axis == 1) ? BoxYCompare
            : BoxZCompare;

        size_t object_span = end - start;

        if (object_span == 1) {
            left = right = objects[start];
        }
        else if (object_span == 2) {
            if (comparator(objects[start], objects[start + 1]))
            {
                left = objects[start];
                right = objects[start + 1];
            }
            else
            {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        else
        {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            left = make_shared<BVHNode>(objects, start, mid);
            right = make_shared<BVHNode>(objects, mid, end);
        }

        bBox = AABB(left->BoundingBox(), right->BoundingBox());
    }
}
