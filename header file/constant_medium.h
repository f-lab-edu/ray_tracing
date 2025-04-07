#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class ConstantMedium : public Hittable {
public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> tex)
        : boundary(boundary), negativeInverseDensity(-1 / density), phaseFunction(std::make_shared<Isotropic>(tex))
    {}

    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, const Color& albedo)
        : boundary(boundary), negativeInverseDensity(-1 / density), phaseFunction(std::make_shared<Isotropic>(albedo))
    {}

    bool isHit(const Ray& inputRay, Interval timeIntervalToCheck, HitRecord& record) const override {
        HitRecord rec1, rec2;

        if (!boundary->isHit(inputRay, Interval::universe, rec1))
            return false;

        if (!boundary->isHit(inputRay, Interval(rec1.hitTime + 0.0001, RT_INFINITY), rec2))
            return false;

        if (rec1.hitTime < timeIntervalToCheck.min) rec1.hitTime = timeIntervalToCheck.min;
        if (rec2.hitTime > timeIntervalToCheck.max) rec2.hitTime = timeIntervalToCheck.max;

        if (rec1.hitTime >= rec2.hitTime)
            return false;

        if (rec1.hitTime < 0)
            rec1.hitTime = 0;

        auto rayLengthTimeOne = inputRay.getDirection().getLength();
        auto distanceIntersections = (rec2.hitTime - rec1.hitTime) * rayLengthTimeOne;          // distance between two intersection points
        auto distanceScattering = negativeInverseDensity * std::log(getRandomDouble());         // distance between first intersection and the point 
                                                                                                //  where the ray starts to be scattered

        if (distanceScattering > distanceIntersections)
            return false;

        // update hit information to the scattering point
        record.hitTime = rec1.hitTime + distanceScattering / rayLengthTimeOne;
        record.hitPosition = inputRay.getPosition(record.hitTime);

        record.normalizedVector = Vec3(1, 0, 0);    // arbitrary
        record.isFrontFace = true;                  // also arbitrary
        record.material = phaseFunction;            // because phaseFunction handles the random direction

        return true;
    }

    AABB getBoundingBox() const override { return boundary->getBoundingBox(); }

private:
    std::shared_ptr<Hittable> boundary;
    double negativeInverseDensity;
    std::shared_ptr<Material> phaseFunction;
};

#endif
