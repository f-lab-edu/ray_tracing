#ifndef RAY_H
#define RAY_H

#include "ray_utility.h"

class Ray {
public:
    Ray() {}

    Ray(const Point3& originInput, const Vec3& directionInput, double timeInput) : origin(originInput), direction(directionInput), time(timeInput) {}
    Ray(const Point3& originInput, const Vec3& directionInput) : Ray(originInput, directionInput, 0) {}

    const Point3& getOrigin() const { return origin; }
    const Vec3& getDirection() const { return direction; }

    double getTime() const {
        return time;
    }

    Point3 getPosition(double t) const {
        return origin + t * direction;
    }

private:
    Point3 origin;
    Vec3 direction;
    double time;
};

#endif
