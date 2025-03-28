#include "ray_utility.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
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

    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 100;

    camera.render(world);

    return 0;
}
