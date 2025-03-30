#include "ray_utility.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

double isHitSphere(const Point3& sphereCenter, double sphereRadius, const Ray& inputRay) {
    Vec3 cq = sphereCenter - inputRay.getOrigin();      // C - Q
    auto a = performDot(inputRay.getDirection(), inputRay.getDirection());
    auto h = performDot(inputRay.getDirection(), cq);
    auto c = performDot(cq, cq) - sphereRadius * sphereRadius;
    auto determinant = h * h - a * c;
    if (determinant < 0)
        return -1;
    else
        return (h -sqrt(determinant)) / a;
}


int main() {
    HittableList world;

    auto radius = std::cos(PI / 4);

    auto materialLeft = std::make_shared<Lambertian>(Color(0, 0, 1));
    auto materialRight = std::make_shared<Lambertian>(Color(1, 0, 0));

    world.add(std::make_shared<Sphere>(Point3(-radius, 0, -1), radius, materialLeft));
    world.add(std::make_shared<Sphere>(Point3(radius, 0, -1), radius, materialRight));


    Camera camera;
    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 50;
    camera.verticalFOV = 90;

    camera.render(world);

    return 0;
}
