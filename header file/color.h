#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using Color = Vec3;

inline double convertLinearToGamma(double linearComponent) {
    if (linearComponent > 0)
        return std::sqrt(linearComponent);

    return 0;
}


void writeColor(std::ostream& out, const Color& pixelColor) {
    auto r = pixelColor.getX();
    auto g = pixelColor.getY();
    auto b = pixelColor.getZ();

    // Aply gamma translation
    r = convertLinearToGamma(r);
    g = convertLinearToGamma(g);
    b = convertLinearToGamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const Interval intensity(0.000, 0.999);
    int rbyte = static_cast<int>(256 * intensity.clamp(r));
    int gbyte = static_cast<int>(256 * intensity.clamp(g));
    int bbyte = static_cast<int>(256 * intensity.clamp(b));


    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
