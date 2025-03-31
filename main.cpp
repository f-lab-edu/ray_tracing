#include "ray_utility.h"

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "Sphere.h"

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

    auto groundMaterial = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto randomMaterial = getRandomDouble();
            Point3 center(a + 0.9 * getRandomDouble(), 0.2, b + 0.9 * getRandomDouble());

            if ((center - Point3(4, 0.2, 0)).getLength() > 0.9) {
                std::shared_ptr<Material> sphereMaterial;

                if (randomMaterial < 0.8) {
                    // diffuse
                    auto albedo = Color::getRandomVector() * Color::getRandomVector();
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, getRandomDouble(0, 0.5), 0);
                    world.add(std::make_shared<Sphere>(center, center2, 0.2, sphereMaterial));
                }
                else if (randomMaterial < 0.95) {
                    // metal
                    auto albedo = Color::getRandomVector(0.5, 1);
                    auto fuzz = getRandomDouble(0, 0.5);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else {
                    // glass
                    sphereMaterial = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    world = HittableList(std::make_shared<BVHNode>(world));

    Camera camera;
    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 50;

    camera.verticalFOV = 20;
    camera.lookFrom = Point3(13, 2, 3);
    camera.lookAt = Point3(0, 0, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0.6;
    camera.focusDistance = 10.0;


    camera.render(world);

    return 0;
}
