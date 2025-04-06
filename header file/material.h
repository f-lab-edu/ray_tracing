#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool doesScatter(const Ray &inputRay, const HitRecord &record, Color &attenuation, Ray &scatteredRay) const {
        return false;
    }
    virtual Color getEmittedColor(double u, double v, const Point3& hitPosition) const {
        return Color(0, 0, 0);
    }
};

class Lambertian : public Material {
public:
    Lambertian(const Color& inputAlbedo) : texture(std::make_shared<ConstantTexture>(inputAlbedo)) {}
    Lambertian(std::shared_ptr<Texture> inputTexture) : texture(inputTexture) {}

    bool doesScatter(const Ray &inputRay, const HitRecord &record, Color &attenuation, Ray &scatteredRay) const override {
        auto scatteredVector = record.normalizedVector + getRandomUnitVector();

        // handling invalid scattering
        if (scatteredVector.isNearZero())
            scatteredVector = record.normalizedVector;

        scatteredRay = Ray(record.hitPosition, scatteredVector, inputRay.getTime());
        attenuation = texture->getColor(record.u, record.v, record.hitPosition);
        return true;
    }

private:
    std::shared_ptr<Texture> texture;
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
        scatteredRay = Ray(record.hitPosition, reflectedVector, inputRay.getTime());
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
        double finalRefractionIndex = record.isFrontFace ? (1.0 / refractionIndex) : refractionIndex;

        Vec3 normalizedInputVector = getUnitVector(inputRay.getDirection());
        double cosTheta = std::fmin(performDot(-normalizedInputVector, record.normalizedVector), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        bool canRefracted = finalRefractionIndex * sinTheta > 1.0;
        Vec3 finalRay;      // this can be made from reflection or refraction
        if (canRefracted || getReflectance(cosTheta, finalRefractionIndex) > getRandomDouble(0, 1))
            finalRay = getReflectedMirror(normalizedInputVector, record.normalizedVector);
        else
            finalRay = getRefracted(normalizedInputVector, record.normalizedVector, finalRefractionIndex);

        scatteredRay = Ray(record.hitPosition, finalRay, inputRay.getTime());
        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refractionIndex;

    static double getReflectance(double cosine, double inputRefractionIndex) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - inputRefractionIndex) / (1 + inputRefractionIndex);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture> inputTexture) : texture(inputTexture) {}
    DiffuseLight(const Color& emit) : texture(std::make_shared<ConstantTexture>(emit)) {}

    // DiffuseLight doens't perform reflection
    // Hence, only DiffuseLight returns false when the camera calls doesScatter() for Material objects

    Color getEmittedColor(double u, double v, const Point3& hitPosition) const override {
        // returns constant color by default
        return texture->getColor(u, v, hitPosition);
    }

private:
    std::shared_ptr<Texture> texture;
};

#endif
