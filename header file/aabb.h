#ifndef AABB_H
#define AABB_H

#include "ray_utility.h"

class AABB {
public:
    Interval intervalX, intervalY, intervalZ;

    AABB() {} // The default AABB is empty, since intervals are empty by default.

    AABB(const Interval& x, const Interval& y, const Interval& z)
        : intervalX(x), intervalY(y), intervalZ(z) {}

    AABB(const Point3& a, const Point3& b) {
        // Treat the two points a and b as extrema for the bounding box, so we don't require a
        // particular minimum/maximum coordinate order.

        intervalX = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        intervalY = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        intervalZ = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
    }

    AABB(const AABB& lhs, const AABB& rhs) {
        intervalX = Interval(lhs.intervalX, rhs.intervalX);
        intervalY = Interval(lhs.intervalY, rhs.intervalY);
        intervalZ = Interval(lhs.intervalZ, rhs.intervalZ);
    }

    const Interval& getAxisInterval(int n) const {
        if (n == 1) return intervalY;
        if (n == 2) return intervalZ;
        return intervalX;
    }

    bool isHit(const Ray& inputRay, Interval timeIntervalToCheck) const {
        const Point3& rayOrigin = inputRay.getOrigin();
        const Vec3& rayDirection = inputRay.getDirection();

        for (int currentAxis = 0; currentAxis < 3; ++currentAxis) {
            const Interval& currentIntervalAABB = getAxisInterval(currentAxis);
            const double currentInverseRayDirection = 1.0 / rayDirection[currentAxis];

            auto timeBegin = (currentIntervalAABB.min - rayOrigin[currentAxis]) * currentInverseRayDirection;
            auto timeEnd = (currentIntervalAABB.max - rayOrigin[currentAxis]) * currentInverseRayDirection;

            // this function checks the overlapping between two intervals in terms of time instead of position
            // [timeBegin, timeEnd], timeIntervalToCheck
            if (timeBegin < timeEnd) {
                if (timeBegin > timeIntervalToCheck.min) timeIntervalToCheck.min = timeBegin;
                if (timeEnd < timeIntervalToCheck.max) timeIntervalToCheck.max = timeEnd;
            }
            else {
                if (timeEnd > timeIntervalToCheck.min) timeIntervalToCheck.min = timeEnd;
                if (timeBegin < timeIntervalToCheck.max) timeIntervalToCheck.max = timeBegin;
            }

            if (timeIntervalToCheck.max <= timeIntervalToCheck.min)
                return false;
        }
        // if all axes are overlapped, then the ray is hit
        return true;
    }

    int getLongestAxisIndex() const {
        // Returns the index of the longest axis of the bounding box.

        if (intervalX.getSize() > intervalY.getSize())
            return intervalX.getSize() > intervalZ.getSize() ? 0 : 2;
        else
            return intervalY.getSize() > intervalZ.getSize() ? 1 : 2;
    }

    static const AABB empty, universe;
};


const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

#endif
