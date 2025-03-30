#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class Camera {
public:
    double aspectRatio = 1.0;           // Ratio of image width over height
    int    imageWidth = 100;            // Rendered image width in pixel count
    int    samplesPerPixel = 10;        // Count of random sampels for each pixel
    int    maxDepth = 10;               // Maximum number of ray bounces into scene

    double verticalFOV = 90;            // vertical view angle
    Point3 lookFrom = Point3(0, 0, 0);  // Point camera is looking from
    Point3 lookAt = Point3(0, 0, -1);   // Point camera is looking at
    Vec3   upVector = Vec3(0, 1, 0);    // Camera-relative "up" direction;


    void render(const Hittable& world) {
        initialize();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (int currentHeight = 0; currentHeight < imageHeight; ++currentHeight) {
            std::clog << "\rScanlines remaining: " << (imageHeight - currentHeight) << ' ' << std::flush;
            for (int currentWidth = 0; currentWidth < imageWidth; ++currentWidth) {
                Color pixelColor(0, 0, 0);
                for (int currentSample = 0; currentSample < samplesPerPixel; ++currentSample) {
                    Ray currentRay = getRayToSample(currentWidth, currentHeight);
                    pixelColor += getRayColor(currentRay, maxDepth, world);
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

        center = lookFrom;

        // Determine viewport dimensions.
        auto focalLength = (lookAt - lookFrom).getLength();
        auto theta = convertDegreesToRadians(verticalFOV);
        auto h = std::tan(theta / 2) * focalLength;
        auto viewportHeight = 2 * h;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

        // Calculate the x,y,z unit basis vectors for the camera coordinate frame.
        // x = right, z = view direction, y = final up direction
        axisZ = getUnitVector(lookAt - lookFrom);
        auto pureUpVector = upVector - performDot(upVector, axisZ) * axisZ;
        pureUpVector = pureUpVector / pureUpVector.getLength();
        axisX = getUnitVector(performCross(axisZ, pureUpVector));
        axisY = performCross(axisX, axisZ);  

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto vectorViewportWidth = viewportWidth * axisX;
        auto vectorViewportHeight = viewportHeight * -axisY;  // negating is needed to make the direction start from top to bottom

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixelDeltaWidth = vectorViewportWidth / imageWidth;
        pixelDeltaHeight = vectorViewportHeight / imageHeight;

        // Calculate the location of the upper left pixel.
        auto pointViewportTopLeft = focalLength * axisZ - center - vectorViewportWidth / 2 - vectorViewportHeight / 2;
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


    Color getRayColor(const Ray& inputRay, int depth, const Hittable& world) const {
        if (depth <= 0)
            return Color(0, 0, 0);

        HitRecord record;

        if (world.isHit(inputRay, Interval(0.001, RT_INFINITY), record)) {
            Ray scatteredRay;
            Color attenuation;
            if (record.material->doesScatter(inputRay, record, attenuation, scatteredRay))
                return attenuation * getRayColor(scatteredRay, depth - 1, world);
            return Color(0, 0, 0);
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
    Vec3   axisX, axisY, axisZ; // camera frame basis vectors (u,v,w)
};

#endif
