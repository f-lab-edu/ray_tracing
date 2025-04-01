#ifndef TEXTURE_H
#define TEXTURE_H

#include "ray_utility.h"


class Texture {
public:
    virtual ~Texture() = default;

    virtual Color getColor(double u, double v, const Point3& position) const = 0;
};

class ConstantTexture : public Texture {
public:
    ConstantTexture(const Color& albedo) : albedo(albedo) {}

    ConstantTexture(double red, double green, double blue) : ConstantTexture(Color(red, green, blue)) {}

    Color getColor(double u, double v, const Point3& position) const override {
        return albedo;
    }

private:
    Color albedo;
};

#endif
