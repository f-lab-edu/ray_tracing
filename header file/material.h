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


class Dielectric : public Material {
public:
    Dielectric(double inputRefractionIndex) : refractionIndex(inputRefractionIndex) {}

    bool doesScatter(const Ray &inputRay, const HitRecord &record, Color &attenuation, Ray &scatteredRay) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double finalRefractionIndex = record.frontFace ? (1.0 / refractionIndex) : refractionIndex;

        Vec3 normalizedInputVector = getUnitVector(inputRay.getDirection());
        double cosTheta = std::fmin(performDot(-normalizedInputVector, record.normalizedVector), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        bool canRefracted = finalRefractionIndex * sinTheta > 1.0;
        Vec3 finalRay;      // this can be made from reflection or refraction
        if (canRefracted)
            finalRay = getReflectedMirror(normalizedInputVector, record.normalizedVector);
        else
            finalRay = getRefracted(normalizedInputVector, record.normalizedVector, finalRefractionIndex);

        scatteredRay = Ray(record.hitPosition, finalRay);
        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refractionIndex;
};

#endif
