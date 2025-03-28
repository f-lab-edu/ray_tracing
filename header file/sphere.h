#ifndef Sphere_H
#define Sphere_H

#include "ray_utility.h"
#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> inputMaterial) : center(center), radius(std::fmax(0, radius)), material(inputMaterial) {}

    bool isHit(const Ray& inputRay, Interval rayInterval, HitRecord& record) const override {
        Vec3 oc = center - inputRay.getOrigin();
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
        Vec3 outwardNormal = (record.hitPosition - center) / radius;
        record.setFaceNormal(inputRay, outwardNormal);
        record.material = material;

        return true;
    }

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> material;
};

#endif
