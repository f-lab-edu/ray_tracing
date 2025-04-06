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

class RotateY : public Hittable {
public:
    RotateY(std::shared_ptr<Hittable> inputObject, double angle) : baseObject(inputObject) {
        auto radians = convertDegreesToRadians(angle);
        sinTheta = std::sin(radians);
        cosTheta = std::cos(radians);
        boundingBox = baseObject->getBoundingBox();

        Point3 min(RT_INFINITY, RT_INFINITY, RT_INFINITY);
        Point3 max(-RT_INFINITY, -RT_INFINITY, -RT_INFINITY);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * boundingBox.intervalX.max + (1 - i) * boundingBox.intervalX.min;
                    auto y = j * boundingBox.intervalY.max + (1 - j) * boundingBox.intervalY.min;
                    auto z = k * boundingBox.intervalZ.max + (1 - k) * boundingBox.intervalZ.min;

                    auto newx = cosTheta * x + sinTheta * z;
                    auto newz = -sinTheta * x + cosTheta * z;

                    Vec3 tester(newx, y, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = std::fmin(min[c], tester[c]);
                        max[c] = std::fmax(max[c], tester[c]);
                    }
                }
            }
        }

        boundingBox = AABB(min, max);
    }

    bool isHit(const Ray& inputRay, Interval timeIntervalToCheck, HitRecord& record) const override {
        // Transform the ray from world space to object space.
        auto adjustedOrigin = Point3(
            (cosTheta * inputRay.getOrigin().getX()) - (sinTheta * inputRay.getOrigin().getZ()),
            inputRay.getOrigin().getY(),
            (sinTheta * inputRay.getOrigin().getX()) + (cosTheta * inputRay.getOrigin().getZ())
        );
        auto adjustedDirection = Vec3(
            (cosTheta * inputRay.getDirection().getX()) - (sinTheta * inputRay.getDirection().getZ()),
            inputRay.getDirection().getY(),
            (sinTheta * inputRay.getDirection().getX()) + (cosTheta * inputRay.getDirection().getZ())
        );
        Ray adjustedRay(adjustedOrigin, adjustedDirection, inputRay.getTime());

        // Determine whether an intersection exists in object space (and if so, where).
        if (!baseObject->isHit(adjustedRay, timeIntervalToCheck, record))
            return false;

        // Transform the intersection from object space back to world space.
        record.hitPosition = Point3(
            (cosTheta * record.hitPosition.getX()) + (sinTheta * record.hitPosition.getZ()),
            record.hitPosition.getY(),
            (-sinTheta * record.hitPosition.getX()) + (cosTheta * record.hitPosition.getZ())
        );
        record.normalizedVector = Vec3(
            (cosTheta * record.normalizedVector.getX()) + (sinTheta * record.normalizedVector.getZ()),
            record.normalizedVector.getY(),
            (-sinTheta * record.normalizedVector.getX()) + (cosTheta * record.normalizedVector.getZ())
        );

        return true;
    }
    AABB getBoundingBox() const override {
        return boundingBox;
    }

private:
    std::shared_ptr<Hittable> baseObject;
    double sinTheta, cosTheta;
    AABB boundingBox;
};



#endif
