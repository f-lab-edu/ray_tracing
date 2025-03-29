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
        auto scatteredVector = record.normalizedVector + getRandomUnitVector();

        // handling invalid scattering
        if (scatteredVector.isNearZero())
            scatteredVector = record.normalizedVector;

        scatteredRay = Ray(record.hitPosition, scatteredVector);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};


class Metal : public Material {
public:
    Metal(const Color& inputAlbedo, double inputFuzz) : albedo(inputAlbedo), fuzz(inputFuzz) {
        if (inputFuzz < -1)
            inputFuzz = -1;
        else if (inputFuzz > 1)
            inputFuzz = 1;
    }

    bool doesScatter(const Ray &inputRay, const HitRecord &record, Color &attenuation, Ray &scatteredRay) const override {
        Vec3 reflectedVector = getReflectedMirror(inputRay.getDirection(), record.normalizedVector);
        reflectedVector = getUnitVector(reflectedVector) + (fuzz * getRandomUnitVector());
        scatteredRay = Ray(record.hitPosition, reflectedVector);
        attenuation = albedo;
        return (performDot(scatteredRay.getDirection(), record.normalizedVector) > 0);
    }

private:
    Color albedo;
    double fuzz;
};

#endif
