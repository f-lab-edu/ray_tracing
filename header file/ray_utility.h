#ifndef RAYUTILITY_H
#define RAYUTILITY_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>


// Constants
constexpr double RT_INFINITY = std::numeric_limits<double>::infinity();
constexpr double PI = 3.1415926535897932385;

// Utility Functions
inline double convertDegreesToRadians(double degrees) {
    return degrees * PI / 180.0;
}


inline double getRandomDouble(double min, double max) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

// Common Headers
#include "vec3.h"
#include "interval.h"
#include "color.h"
#include "ray.h"

#endif
