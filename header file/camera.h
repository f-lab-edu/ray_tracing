#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera {
public:
    double aspectRatio = 1.0;  // Ratio of image width over height
    int    imageWidth = 100;  // Rendered image width in pixel count

    void render(const Hittable &world) {
        initialize();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (int j = 0; j < imageHeight; j++) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            for (int i = 0; i < imageWidth; i++) {
                auto pixelCenter = pixelCenterTopLeft + (i * vectorDeltaWidth) + (j * vectorDeltaHeight);
                auto rayDirection = pixelCenter - center;
                Ray r(center, rayDirection);

                Color pixelColor = getRayColor(r, world);
                writeColor(std::cout, pixelColor);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    void initialize() {
        imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        center = Point3(0, 0, 0);

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto vectorViewportWidth = Vec3(viewportWidth, 0, 0);
        auto vectorViewportHeight = Vec3(0, -viewportHeight, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        vectorDeltaWidth = vectorViewportWidth / imageWidth;
        vectorDeltaHeight = vectorViewportHeight / imageHeight;

        // Calculate the location of the upper left pixel.
        auto pointViewportTopLeft = center - Vec3(0, 0, focal_length) - vectorViewportWidth / 2 - vectorViewportHeight / 2;
        pixelCenterTopLeft = pointViewportTopLeft + 0.5 * (vectorDeltaWidth + vectorDeltaHeight);
    }

    Color getRayColor(const Ray &inputRay, const Hittable &world) const {
        HitRecord record;

        if (world.isHit(inputRay, Interval(0, RT_INFINITY), record)) {
            return 0.5 * (record.normalizedVector + Color(1, 1, 1));
        }

        Vec3 unitDirection = getUnitVector(inputRay.getDirection());
        auto lerp = 0.5 * (unitDirection.getY() + 1.0);
        return (1.0 - lerp) * Color(1.0, 1.0, 1.0) + lerp * Color(0.5, 0.7, 1.0);
    }


    int    imageHeight;   // Rendered image height
    Point3 center;         // Camera center
    Point3 pixelCenterTopLeft;    // Location of pixel 0, 0
    Vec3   vectorDeltaWidth;  // Offset to pixel to the right
    Vec3   vectorDeltaHeight;  // Offset to pixel below
};

#endif
