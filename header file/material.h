#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool doesScatter(const Ray &inputRay, const HitRecord &record, Color &attenuation, Ray &scatteredRay) const {
        return false;
    }
};

class Lambertian : public Material {
public:
    Lambertian(const Color& inputAlbedo) : albedo(inputAlbedo) {}

    bool doesScatter(const Ray &inputRay, const HitRecord &record, Color &attenuation, Ray &scatteredRay) const override {
        auto scatterDirection = record.normalizedVector + getRandomUnitVector();
        scatteredRay = Ray(record.hitPosition, scatterDirection);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};

#endif
