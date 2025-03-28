#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera {
public:
    double aspectRatio = 1.0;           // Ratio of image width over height
    int    imageWidth = 100;            // Rendered image width in pixel count
    int    samplesPerPixel = 10;        // Count of random sampels for each pixel

    void render(const Hittable &world) {
        initialize();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (int currentHeight = 0; currentHeight < imageHeight; ++currentHeight) {
            std::clog << "\rScanlines remaining: " << (imageHeight - currentHeight) << ' ' << std::flush;
            for (int currentWidth = 0; currentWidth < imageWidth; ++currentWidth) {
                Color pixelColor(0, 0, 0);
                for (int currentSample = 0; currentSample < samplesPerPixel; ++currentSample) {
                    Ray currentRay = getRayToSample(currentWidth, currentHeight);
                    pixelColor += getRayColor(currentRay, world);
                }
                writeColor(std::cout, pixelSamplesScale * pixelColor);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    void initialize() {
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        pixelSamplesScale = 1.0 / samplesPerPixel;

        center = Point3(0, 0, 0);

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto vectorViewportWidth = Vec3(viewportWidth, 0, 0);
        auto vectorViewportHeight = Vec3(0, -viewportHeight, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixelDeltaWidth = vectorViewportWidth / imageWidth;
        pixelDeltaHeight = vectorViewportHeight / imageHeight;

        // Calculate the location of the upper left pixel.
        auto pointViewportTopLeft = center - Vec3(0, 0, focal_length) - vectorViewportWidth / 2 - vectorViewportHeight / 2;
        pixelCenterTopLeft = pointViewportTopLeft + 0.5 * (pixelDeltaWidth + pixelDeltaHeight);
    }

    Ray getRayToSample(int currentWidth, int currentHeight) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location currentWidth, currentHeight.

        auto offset = getSampleSquare();
        auto pixelSample = pixelCenterTopLeft
            + ((currentWidth + offset.getX()) * pixelDeltaWidth)
            + ((currentHeight + offset.getY()) * pixelDeltaHeight);

        auto rayDirection = pixelSample - center;

        return Ray(center, rayDirection);
    }

    Vec3 getSampleSquare() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return Vec3(getRandomDouble(0, 1) - 0.5, getRandomDouble(0, 1) - 0.5, 0);
    }


    Color getRayColor(const Ray &inputRay, const Hittable &world) const {
        HitRecord record;

        if (world.isHit(inputRay, Interval(0, RT_INFINITY), record)) {
            Vec3 direction = getRandomOnHemisphere(record.normalizedVector);
            return 0.5 * getRayColor(Ray(record.hitPosition, direction), world);
        }

        Vec3 unitDirection = getUnitVector(inputRay.getDirection());
        auto lerp = 0.5 * (unitDirection.getY() + 1.0);
        return (1.0 - lerp) * Color(1.0, 1.0, 1.0) + lerp * Color(0.5, 0.7, 1.0);
    }


    int    imageHeight;   // Rendered image height
    double pixelSamplesScale;        // Color scale factor for a sum of pixel samples
    Point3 center;         // Camera center
    Point3 pixelCenterTopLeft;    // Location of pixel 0, 0
    Vec3   pixelDeltaWidth;  // Offset to pixel to the right
    Vec3   pixelDeltaHeight;  // Offset to pixel below
};

#endif
