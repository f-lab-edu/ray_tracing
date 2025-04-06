#ifndef INTERVAL_H
#define INTERVAL_H

#include "ray_utility.h"

class Interval {
public:
    double min, max;

    Interval() : min(+RT_INFINITY), max(-RT_INFINITY) {} // Default Interval is empty

    Interval(double min, double max) : min(min), max(max) {}

    Interval(Interval lhs, Interval rhs) {
        // Create the Interval tightly enclosing the two input intervals.
        min = lhs.min <= rhs.min ? lhs.min : rhs.min;
        max = lhs.max >= rhs.max ? lhs.max : rhs.max;
    }

    double getSize() const {
        return max - min;
    }

    bool doesContain(double x) const {
        return min <= x && x <= max;
    }

    bool doesSurround(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    Interval expand(double delta) const {
        auto padding = delta / 2;
        return Interval(min - padding, max + padding);
    }

    static const Interval empty, universe;
};

const Interval Interval::empty = Interval(+RT_INFINITY, -RT_INFINITY);
const Interval Interval::universe = Interval(-RT_INFINITY, +RT_INFINITY);


Interval operator+(const Interval& inputInterval, double displacement) {
    return Interval(inputInterval.min + displacement, inputInterval.max + displacement);
}

Interval operator+(double displacement, const Interval& inputInterval) {
    return inputInterval + displacement;
}

#endif
