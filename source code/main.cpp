#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

bool isHitSphere(const Point3& sphereCenter, double sphereRadius, const Ray& inputRay) {
    Vec3 cq = sphereCenter - inputRay.getOrigin();      // C - Q
    auto a = performDot(inputRay.getDirection(), inputRay.getDirection());
    auto b = -2.0 * performDot(inputRay.getDirection(), cq);
    auto c = performDot(cq, cq) - sphereRadius * sphereRadius;
    auto determinant = b * b - 4 * a * c;
    return determinant >= 0;
}


Color getRayColor(const Ray& inputRay) {
    if (isHitSphere(Point3(0, 0, -1), 0.5, inputRay))
        return Color(0, 1, 0);

    Vec3 unitDirection = getUnitVector(inputRay.getDirection());
    auto a = 0.5 * (unitDirection.getY() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() {
    auto aspectRatio = 16.0 / 9.0;
    int imageWidth = 400;
    int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    auto focalLength = 1.0, viewportHeight = 2.0;
    auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);
    auto cameraCenter = Point3(0, 0, 0);

    auto vectorViewportWidth = Vec3(viewportWidth, 0, 0), vectorViewportHeight = Vec3(0, -viewportHeight, 0);
    auto vectorDeltaWidth = vectorViewportWidth / imageWidth, vectorDeltaHeight = vectorViewportHeight / imageHeight;
    
    auto pointViewportTopLeft = cameraCenter - Vec3(0, 0, focalLength) - vectorViewportHeight / 2 - vectorViewportWidth / 2;
    auto pixelCenterTopLeft = pointViewportTopLeft + 0.5 * (vectorDeltaHeight + vectorDeltaWidth);


    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    for (int currentHeight = 0; currentHeight < imageHeight; ++currentHeight) {
        std::clog << "\rScanlines remaining: " << (imageHeight - currentHeight) << ' ' << std::flush;
        for (int currentWidth = 0; currentWidth < imageWidth; ++currentWidth) {
            auto currentPixelCenter = pixelCenterTopLeft + (currentWidth * vectorDeltaWidth) + (currentHeight * vectorDeltaHeight);
            auto rayDirection = currentPixelCenter - cameraCenter;
            Ray currentRay(cameraCenter, rayDirection);

            Color pixelColor = getRayColor(currentRay);
            writeColor(std::cout, pixelColor);
        }
    }
    std::clog << "\rDone.                 \n";
    return 0;
}
