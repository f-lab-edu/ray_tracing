#ifndef QUAD_H
#define QUAD_H

#include "ray_utility.h"
#include "hittable.h"

class Quad : public Hittable {
public:
    Quad(const Point3& inputQ, const Vec3& inputU, const Vec3& inputV, std::shared_ptr<Material> inputMaterial)
        : q(inputQ), u(inputU), v(inputV), material(inputMaterial) {
        setBoundingBox();
    }

    virtual void setBoundingBox() {
        // Compute the bounding box of all four vertices.
        auto boundingBoxA = AABB(q, q + u + v);
        auto boundingBoxB = AABB(q + u, q + v);
        boundingBox = AABB(boundingBoxA, boundingBoxB);
    }

    AABB getBoundingBox() const override {
        return boundingBox;
    }

    bool isHit(const Ray& inputRay, Interval timeIntervalToCheck, HitRecord& record) const override {
        return false; // To be implemented
    }

private:
    Point3 q;       // bottom-left corner
    Vec3 u, v;      // two sides
    std::shared_ptr<Material> material;
    AABB boundingBox;
};

#endif
