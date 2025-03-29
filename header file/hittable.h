#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray_utility.h"

class Material;

struct HitRecord {
    Point3 hitPosition;
    Vec3 normalizedVector;
    std::shared_ptr<Material> material;
    double root;
    bool frontFace;

    void setFaceNormal(const Ray& inputRay, const Vec3& outwardNormal) {
        // NOTE: the parameter `outward_normal` is assumed to have unit length.
        frontFace = performDot(inputRay.getDirection(), outwardNormal) < 0.0;
        normalizedVector = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool isHit(const Ray& inputRay, Interval rayInterval, HitRecord& record) const = 0;
};

#endif
