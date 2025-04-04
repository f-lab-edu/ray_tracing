#ifndef QUAD_H
#define QUAD_H

#include "ray_utility.h"
#include "hittable.h"

class Quad : public Hittable {
public:
    Quad(const Point3& inputQ, const Vec3& inputU, const Vec3& inputV, std::shared_ptr<Material> inputMaterial)
        : q(inputQ), u(inputU), v(inputV), material(inputMaterial) {
        
        normalVector = getUnitVector(performCross(u, v));

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
        // tempA and tempB are from the plane equation
        double tempA = performDot(normalVector, inputRay.getOrigin() - q), tempB = performDot(normalVector, inputRay.getDirection());

        // No hit if the ray is parallel to the plane
        if (std::fabs(tempB) < 1e-8)
            return false;
        auto timeIntersect = -tempA / tempB;

        // No hit if the time of intersection is not inside the given interval
        if (timeIntervalToCheck.doesContain(timeIntersect) == false)
            return false;

        // Now we know that the ray hits the plane
        // Hence, we need to calculate alpha and beta to check whether they are inside the quad
        auto positionIntersect = inputRay.getPosition(timeIntersect);
        auto alpha = performDot(performCross(positionIntersect - q, v), normalVector / performDot(normalVector, normalVector));
        auto beta = performDot(performCross(u, positionIntersect - q), normalVector / performDot(normalVector, normalVector));

        if (isInterior(alpha, beta, record) == false)
            return false;
        
        // Now we know that the ray hits the Quad
        // update information
        record.hitTime = timeIntersect;
        record.hitPosition = positionIntersect;
        record.material = material;
        record.setFaceNormal(inputRay, normalVector);

        return true;
    }

    virtual bool isInterior(double a, double b, HitRecord& record) const {
        Interval unitInterval = Interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (!unitInterval.doesContain(a) || !unitInterval.doesContain(b))
            return false;

        record.u = a;
        record.v = b;
        return true;
    }

private:
    Point3 q;       // bottom-left corner
    Vec3 u, v;      // two sides
    Vec3 normalVector;    // normal vector to the plane
    std::shared_ptr<Material> material;
    AABB boundingBox;
};

#endif
