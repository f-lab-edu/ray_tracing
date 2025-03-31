#ifndef Sphere_H
#define Sphere_H

#include "ray_utility.h"
#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const Point3& staticCenter, double radiusInput, std::shared_ptr<Material> materialInput) 
        : center(staticCenter, Vec3(0,0,0)), radius(std::fmax(0, radiusInput)), material(materialInput) {}
    Sphere(const Point3& centerStart, const Point3& centerEnd, double radiusInput, std::shared_ptr<Material> materialInput)
        : center(centerStart, centerEnd - centerStart), radius(std::fmax(0, radiusInput)), material(materialInput) {}

    bool isHit(const Ray& inputRay, Interval rayInterval, HitRecord& record) const override {
        Point3 currentCenter = center.getPosition(inputRay.getTime());
        Vec3 oc = currentCenter - inputRay.getOrigin();
        auto a = inputRay.getDirection().getLengthSquared();
        auto h = performDot(inputRay.getDirection(), oc);
        auto c = oc.getLengthSquared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (root <= rayInterval.min || rayInterval.max <= root) {
            root = (h + sqrtd) / a;
            if (root <= rayInterval.min || rayInterval.max <= root)
                return false;
        }

        record.root = root;
        record.hitPosition = inputRay.getPosition(record.root);
        Vec3 outwardNormal = (record.hitPosition - currentCenter) / radius;
        record.setFaceNormal(inputRay, outwardNormal);
        record.material = material;

        return true;
    }

private:
    Ray center;
    double radius;
    std::shared_ptr<Material> material;
};

#endif
