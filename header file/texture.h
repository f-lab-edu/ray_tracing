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

class CheckerTexture  : public Texture {
public:
    CheckerTexture (double scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd)
        : inverseScale(1.0 / scale), textureForEven(even), textureForOdd(odd) {
    }

    CheckerTexture (double scale, const Color& c1, const Color& c2)
        : CheckerTexture (scale, std::make_shared<ConstantTexture>(c1), std::make_shared<ConstantTexture>(c2)) {
    }

    Color getColor(double u, double v, const Point3& position) const override {
        auto xInteger = int(std::floor(inverseScale * position.getX()));
        auto yInteger = int(std::floor(inverseScale * position.getY()));
        auto zInteger = int(std::floor(inverseScale * position.getZ()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? textureForEven->getColor(u, v, position) : textureForOdd->getColor(u, v, position);
    }

private:
    double inverseScale;
    std::shared_ptr<Texture> textureForEven;
    std::shared_ptr<Texture> textureForOdd;
};

#endif
