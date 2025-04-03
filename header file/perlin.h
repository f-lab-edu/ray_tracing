#ifndef TEXTURE_H
#define TEXTURE_H

#include "ray_utility.h"
#include "perlin.h"
#include "stb_image_helper.h"


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

class ImageTexture : public Texture {
public:
    ImageTexture(const char* fileName) : image(fileName) {}

    Color getColor(double u, double v, const Point3& position) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.getHeight() <= 0) return Color(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Interval(0, 1).clamp(u);
        v = 1.0 - Interval(0, 1).clamp(v);  // Flip V to image coordinates

        auto x = int(u * image.getWidth());
        auto y = int(v * image.getHeight());
        auto pixel = image.getPixelData(x, y);

        auto colorScale = 1.0 / 255.0;
        return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
    }

private:
    STBImageHelper image;
};


class NoiseTexture : public Texture {
public:
    NoiseTexture(double inputScale) : scale(inputScale) {}

    Color getColor(double u, double v, const Point3& position) const override {
        return Color(1, 1, 1) * perlin.getNoise(scale * position);
    }

private:
    Perlin perlin;
    double scale;
};

#endif
