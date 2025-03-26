#ifndef RAY_H
#define RAY_H

#include "ray_utility.h"

class Ray {
public:
    Ray() {}

    Ray(const Point3& originInput, const Vec3& directionInput) : origin(originInput), direction(directionInput) {}

    const Point3& getOrigin() const { return origin; }
    const Vec3& getDirection() const { return direction; }

    Point3 getPosition(double t) const {
        return origin + t * direction;
    }

private:
    Point3 origin;
    Vec3 direction;
};

#endif
