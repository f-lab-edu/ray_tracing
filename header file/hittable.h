#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray_utility.h"
#include "aabb.h"

class Material;

struct HitRecord {
    Point3 hitPosition;
    Vec3 normalizedVector;
    std::shared_ptr<Material> material;
    double hitTime;
    double u, v;
    bool isFrontFace;

    void setFaceNormal(const Ray& inputRay, const Vec3& outwardNormal) {
        // NOTE: the parameter `outward_normal` is assumed to have unit length.
        isFrontFace = performDot(inputRay.getDirection(), outwardNormal) < 0.0;
        normalizedVector = isFrontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool isHit(const Ray& inputRay, Interval timeIntervalToCheck, HitRecord& record) const = 0;
    virtual AABB getBoundingBox() const = 0;
};


class Translate : public Hittable {
public:
    Translate(std::shared_ptr<Hittable> inputObject, const Vec3& inputOffset) : baseObject(inputObject), offset(inputOffset) {
        boundingBox = baseObject->getBoundingBox() + offset;
    }

    bool isHit(const Ray& inputRay, Interval timeIntervalToCheck, HitRecord& record) const override {
        // Move the ray backwards by the offset
        Ray adjustedRay(inputRay.getOrigin() - offset, inputRay.getDirection(), inputRay.getTime());

        // Determine whether an intersection exists along the offset ray (and if so, where)
        if (baseObject->isHit(adjustedRay, timeIntervalToCheck, record) == false)
            return false;
        
        // Move the intersection point forwards by the offset
        record.hitPosition += offset;
        return true;
    }
    AABB getBoundingBox() const override {
        return boundingBox;
    }

private:
    std::shared_ptr<Hittable> baseObject;
    Vec3 offset;
    AABB boundingBox;
};


#endif
