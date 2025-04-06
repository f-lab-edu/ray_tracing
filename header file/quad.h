#ifndef QUAD_H
#define QUAD_H

#include "ray_utility.h"
#include "hittable.h"

class Quad : public Hittable {
public:
    Quad(const Point3& inputQ, const Vec3& inputU, const Vec3& inputV, std::shared_ptr<Material> inputMaterial)
        : q(inputQ), u(inputU), v(inputV), material(inputMaterial) {
        auto n = performCross(u, v);
        k = n / performDot(n, n);
        normalVector = getUnitVector(n);

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
        // Hence, we need to calculate alpha and beta to check whether they are inside the Quad
        auto positionIntersect = inputRay.getPosition(timeIntersect);
        auto alpha = performDot(performCross(positionIntersect - q, v), k);
        auto beta = performDot(performCross(u, positionIntersect - q), k);

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
    Point3 q;             // bottom-left corner
    Vec3 u, v;            // two sides
    Vec3 normalVector;    // normal vector to the plane
    Vec3 k;               // cached value before n is normalized
    std::shared_ptr<Material> material;
    AABB boundingBox;
};


inline std::shared_ptr<HittableList> getBox(const Point3& a, const Point3& b, std::shared_ptr<Material> material)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.
    auto sides = std::make_shared<HittableList>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = Point3(std::fmin(a.getX(), b.getX()), std::fmin(a.getY(), b.getY()), std::fmin(a.getZ(), b.getZ()));
    auto max = Point3(std::fmax(a.getX(), b.getX()), std::fmax(a.getY(), b.getY()), std::fmax(a.getZ(), b.getZ()));

    auto dx = Vec3(max.getX() - min.getX(), 0, 0);
    auto dy = Vec3(0, max.getY() - min.getY(), 0);
    auto dz = Vec3(0, 0, max.getZ() - min.getZ());

    sides->add(std::make_shared<Quad>(Point3(min.getX(), min.getY(), max.getZ()), dx, dy, material)); // front
    sides->add(std::make_shared<Quad>(Point3(max.getX(), min.getY(), max.getZ()), -dz, dy, material)); // right
    sides->add(std::make_shared<Quad>(Point3(max.getX(), min.getY(), min.getZ()), -dx, dy, material)); // back
    sides->add(std::make_shared<Quad>(Point3(min.getX(), min.getY(), min.getZ()), dz, dy, material)); // left
    sides->add(std::make_shared<Quad>(Point3(min.getX(), max.getY(), max.getZ()), dx, -dz, material)); // top
    sides->add(std::make_shared<Quad>(Point3(min.getX(), min.getY(), min.getZ()), dx, dz, material)); // bottom

    return sides;
}

#endif
