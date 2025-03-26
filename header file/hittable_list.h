#ifndef Hittable_LIST_H
#define Hittable_LIST_H

#include "ray_utility.h"
#include "Hittable.h"

#include <vector>

class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    bool isHit(const Ray& r, double rayMin, double rayMax, HitRecord& record) const override {
        HitRecord tempRecord;
        bool isHitAnything = false;
        auto closestSoFar = rayMax;

        for (const auto& object : objects) {
            if (object->isHit(r, rayMin, closestSoFar, tempRecord)) {
                isHitAnything = true;
                if (tempRecord.root < closestSoFar) {
                    closestSoFar = tempRecord.root;
                    record = tempRecord;
                }
            }
        }

        return isHitAnything;
    }
};

#endif
