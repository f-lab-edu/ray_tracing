#ifndef BVH_H
#define BVH_H

#include "AABB.h"
#include "Hittable.h"
#include "hittable_list.h"

#include <algorithm>

class BVHNode : public Hittable {
public:
    BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size()) {
        // There's a C++ subtlety here. This constructor (without span indices) creates an
        // implicit copy of the Hittable list, which we will modify. The lifetime of the copied
        // list only extends until this constructor exits. That's OK, because we only need to
        // persist the resulting bounding volume hierarchy.
    }

    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
        boundingBox = AABB::empty;

        // now calculate the final boundingBox first
        for (size_t currentObjectIndex = start; currentObjectIndex < end; ++currentObjectIndex)
            boundingBox = AABB(boundingBox, objects[currentObjectIndex]->getBoundingBox());
        
        int axis = boundingBox.getLongestAxisIndex();       // optimization applied
        auto comparator = compareBoxZ;
        if (axis == 0)
            comparator = compareBoxX;
        else if (axis == 1)
            comparator = compareBoxY;

        // then calculate the boundingBoxes for its left and right children
        size_t objectSpan = end - start;
        if (objectSpan == 1)
            left = right = objects[start];
        else if (objectSpan == 2) {
            left = objects[start];
            right = objects[start + 1];
        }
        else {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            auto mid = start + objectSpan / 2;
            left = std::make_shared<BVHNode>(objects, start, mid);
            right = std::make_shared<BVHNode>(objects, mid, end);
        }
    }

    bool isHit(const Ray& inputRay, Interval timeIntervalToCheck, HitRecord& record) const override {
        if (!boundingBox.isHit(inputRay, timeIntervalToCheck))
            return false;

        bool isHitLeft = left->isHit(inputRay, timeIntervalToCheck, record);
        bool isHitRight = right->isHit(inputRay, Interval(timeIntervalToCheck.min, isHitLeft ? record.hitTime : timeIntervalToCheck.max), record);

        return isHitLeft || isHitRight;
    }

    AABB getBoundingBox() const override { return boundingBox; }

private:
    static bool compareBox(const std::shared_ptr<Hittable> lhs, const std::shared_ptr<Hittable> rhs, int axisIndex) {
        auto leftInterval = lhs->getBoundingBox().getAxisInterval(axisIndex);
        auto rightIterval = rhs->getBoundingBox().getAxisInterval(axisIndex);
        return leftInterval.min < rightIterval.min;
    }

    static bool compareBoxX(const std::shared_ptr<Hittable> lhs, const std::shared_ptr<Hittable> rhs) {
        return compareBox(lhs, rhs, 0);
    }

    static bool compareBoxY(const std::shared_ptr<Hittable> lhs, const std::shared_ptr<Hittable> rhs) {
        return compareBox(lhs, rhs, 1);
    }

    static bool compareBoxZ(const std::shared_ptr<Hittable> lhs, const std::shared_ptr<Hittable> rhs) {
        return compareBox(lhs, rhs, 2);
    }




    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB boundingBox;
};

#endif
