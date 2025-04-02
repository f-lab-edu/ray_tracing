#ifndef Sphere_H
#define Sphere_H

#include "ray_utility.h"
#include "hittable.h"

class Sphere : public Hittable {
public:
    // stationary
    Sphere(const Point3& staticCenter, double radiusInput, std::shared_ptr<Material> materialInput) 
        : center(staticCenter, Vec3(0,0,0)), radius(std::fmax(0, radiusInput)), material(materialInput) {
        // create a cube as bouding box for sphere
        auto vectorRadius = Vec3(radius, radius, radius);
        boundingBox = AABB(staticCenter - vectorRadius, staticCenter + vectorRadius);
    }

    // moving
    Sphere(const Point3& centerStart, const Point3& centerEnd, double radiusInput, std::shared_ptr<Material> materialInput)
        : center(centerStart, centerEnd - centerStart), radius(std::fmax(0, radiusInput)), material(materialInput) {
        auto vectorRadius = Vec3(radius, radius, radius);
        AABB boxA(center.getPosition(0) - vectorRadius, center.getPosition(0) + vectorRadius);
        AABB boxB(center.getPosition(1) - vectorRadius, center.getPosition(1) + vectorRadius);
        // make the union of boxA and boxB
        boundingBox = AABB(boxA, boxB);
    }

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
        getSphereUV(outwardNormal, record.u, record.v);
        record.material = material;

        return true;
    }

    AABB getBoundingBox() const override {
        return boundingBox;
    }

private:
    static void getSphereUV(const Point3& hitPosition, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = std::acos(-hitPosition.getY());
        auto phi = std::atan2(-hitPosition.getZ(), hitPosition.getX()) + PI;

        u = phi / (2 * PI);
        v = theta / PI;
    }


    Ray center;
    double radius;
    std::shared_ptr<Material> material;
    AABB boundingBox;
};

#endif
