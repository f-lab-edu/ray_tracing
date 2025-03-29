#ifndef VEC3_H
#define VEC3_H

#include "ray_utility.h"

class Vec3 {
public:
    double e[3];

    Vec3() : e{ 0,0,0 } {}
    Vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    double getX() const { return e[0]; }
    double getY() const { return e[1]; }
    double getZ() const { return e[2]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    Vec3& operator+=(const Vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    double getLength() const {
        return std::sqrt(getLengthSquared());
    }

    double getLengthSquared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    bool isNearZero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static Vec3 getRandomVector(double min, double max) {
        return Vec3(getRandomDouble(min, max), getRandomDouble(min, max), getRandomDouble(min, max));
    }
};

// point3 is just an alias for Vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;


// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t) {
    return (1 / t) * v;
}

inline double performDot(const Vec3& u, const Vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline Vec3 performCross(const Vec3& u, const Vec3& v) {
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 getUnitVector(const Vec3& v) {
    return v / v.getLength();
}

inline Vec3 getRandomUnitVector() {
    while (true) {
        auto p = Vec3::getRandomVector(-1, 1);
        auto lengthSquared = p.getLengthSquared();
        if (1e-160 < lengthSquared && lengthSquared <= 1)       // enable blockhole
            return p / sqrt(lengthSquared);
    }
}

inline Vec3 getRandomOnHemisphere(const Vec3& normal) {
    Vec3 onUnitSphere = getRandomUnitVector();
    if (performDot(onUnitSphere, normal) > 0.0) // In the same hemisphere as the normal
        return onUnitSphere;
    else
        return -onUnitSphere;
}

#endif
