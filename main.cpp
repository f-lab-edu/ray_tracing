#include "ray_utility.h"

#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "Hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "Quad.h"
#include "Sphere.h"
#include "texture.h"

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

void renderBouncingSpheres() {
    HittableList world;

    auto ground = std::make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(ground)));

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
                    // Metal
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
    camera.maxDepth = 20;
    camera.backgroundColor = Color(0.7, 0.8, 1.0);

    camera.verticalFOV = 20;
    camera.lookFrom = Point3(13, 2, 3);
    camera.lookAt = Point3(0, 0, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0.6;
    camera.focusDistance = 10.0;


    camera.render(world);
}

void renderCheckeredSpheres() {
    HittableList world;

    auto checker = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));

    world.add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
    world.add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel= 100;
    camera.maxDepth = 50;
    camera.backgroundColor = Color(0.7, 0.8, 1.0);


    camera.verticalFOV = 20;
    camera.lookFrom = Point3(13, 2, 3);
    camera.lookAt = Point3(0, 0, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0;

    camera.render(world);
}

void renderEarth() {
    auto earthTexture = std::make_shared<ImageTexture>("earthmap.jpg");
    auto earthSurface = std::make_shared<Lambertian>(earthTexture);
    auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earthSurface);

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 50;
    camera.backgroundColor = Color(0.7, 0.8, 1.0);


    camera.verticalFOV = 20;
    camera.lookFrom = Point3(0, 0, 12);
    camera.lookAt = Point3(0, 0, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0;

    camera.render(HittableList(globe));
}

void renderPerlinSpheres() {
    HittableList world;

    auto pertext = std::make_shared<NoiseTexture>(4);
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    world.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    Camera camera;

    camera.aspectRatio = 16.0 / 9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 50;
    camera.backgroundColor = Color(0.7, 0.8, 1.0);


    camera.verticalFOV = 20;
    camera.lookFrom = Point3(13, 2, 3);
    camera.lookAt = Point3(0, 0, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0;

    camera.render(world);
}

void renderQuads() {
    HittableList world;

    // Materials
    auto leftRed = std::make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto backGreen = std::make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto rightBlue = std::make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upperOrange = std::make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lowerTeal = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // Quads
    world.add(std::make_shared<Quad>(Point3(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), leftRed));
    world.add(std::make_shared<Quad>(Point3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), backGreen));
    world.add(std::make_shared<Quad>(Point3(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), rightBlue));
    world.add(std::make_shared<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upperOrange));
    world.add(std::make_shared<Quad>(Point3(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), lowerTeal));

    Camera camera;

    camera.aspectRatio = 1.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 50;
    camera.backgroundColor = Color(0.7, 0.8, 1.0);


    camera.verticalFOV = 80;
    camera.lookFrom = Point3(0, 0, 9);
    camera.lookAt = Point3(0, 0, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0;

    camera.render(world);
}

void renderSimpleLight() {
    HittableList world;

    //auto perlinTexture = std::make_shared<NoiseTexture>(4);
    auto constantTextureGround = std::make_shared<ConstantTexture>(Color(0.8, 0.8, 0.8));
    auto constantTextureSphere = std::make_shared<ConstantTexture>(Color(0.3, 0.4, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(constantTextureGround)));
    world.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(constantTextureSphere)));

    auto lightTexture = std::make_shared<DiffuseLight>(Color(4, 4, 4));
    world.add(std::make_shared<Sphere>(Point3(0, 2000, 0), 300, lightTexture));         // sun
    world.add(std::make_shared<Quad>(Point3(3, 1, -5), Vec3(2, 0, 0), Vec3(0, 0, 2), lightTexture));

    Camera camera;

    camera.aspectRatio = 16.0/9.0;
    camera.imageWidth = 400;
    camera.samplesPerPixel = 100;
    camera.maxDepth = 50;
    camera.backgroundColor = Color(0,0,0);


    camera.verticalFOV = 20;
    camera.lookFrom = Point3(26, 3, 6);
    camera.lookAt = Point3(0, 2, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0;

    camera.render(world);
}

void renderCornellBox() {
    HittableList world;

    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    world.add(std::make_shared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.add(std::make_shared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(Point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
    world.add(std::make_shared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));


    // original boxes
    //world.add(getBox(Point3(130, 0, 65), Point3(295, 165, 230), white));
    //world.add(getBox(Point3(265, 0, 295), Point3(430, 330, 460), white));

    // adjusted boxes
    std::shared_ptr<Hittable> box1 = getBox(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.add(box1);
    std::shared_ptr<Hittable> box2 = getBox(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));
    world.add(box2);


    Camera camera;

    camera.aspectRatio = 1.0;
    camera.imageWidth = 600;
    camera.samplesPerPixel = 200;
    camera.maxDepth = 50;
    camera.backgroundColor = Color(0, 0, 0);

    camera.verticalFOV = 40;
    camera.lookFrom = Point3(278, 278, -800);
    camera.lookAt = Point3(278, 278, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0;

    camera.render(world);
}


void renderCornellSmoke() {
    HittableList world;

    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));

    world.add(std::make_shared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.add(std::make_shared<Quad>(Point3(113, 554, 127), Vec3(330, 0, 0), Vec3(0, 0, 305), light));
    world.add(std::make_shared<Quad>(Point3(0, 555, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    std::shared_ptr<Hittable> box1 = getBox(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, Vec3(265, 0, 295));

    std::shared_ptr<Hittable> box2 = getBox(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, Vec3(130, 0, 65));

    world.add(std::make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
    world.add(std::make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

    Camera camera;

    camera.aspectRatio = 1.0;
    camera.imageWidth = 600;
    camera.samplesPerPixel = 200;
    camera.maxDepth = 50;
    camera.backgroundColor = Color(0, 0, 0);

    camera.verticalFOV = 40;
    camera.lookFrom = Point3(278, 278, -800);
    camera.lookAt = Point3(278, 278, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0;

    camera.render(world);
}


void renderFinalScene(int imageWidth, int samplesPerPixel, int maxDepth) {
    HittableList boxes1;
    auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    int boxesPerSide = 20;
    for (int i = 0; i < boxesPerSide; i++) {
        for (int j = 0; j < boxesPerSide; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = getRandomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1.add(getBox(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
        }
    }

    HittableList world;

    world.add(std::make_shared<BVHNode>(boxes1));
    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
    world.add(std::make_shared<Quad>(Point3(123, 554, 147), Vec3(300, 0, 0), Vec3(0, 0, 265), light));
    
    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30, 0, 0);
    auto sphereMaterial = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.add(std::make_shared<Sphere>(center1, center2, 50, sphereMaterial));
    
    world.add(std::make_shared<Sphere>(Point3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
    world.add(std::make_shared<Sphere>(Point3(0, 150, 145), 50, std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

    
    auto boundary = std::make_shared<Sphere>(Point3(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    //boundary = std::make_shared<Sphere>(Point3(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
    //world.add(std::make_shared<ConstantMedium>(boundary, 0.001, Color(1, 1, 1)));
    
    auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
    world.add(std::make_shared<Sphere>(Point3(400, 200, 400), 100, emat));
    auto pertext = std::make_shared<NoiseTexture>(0.2);
    world.add(std::make_shared<Sphere>(Point3(220, 280, 300), 80, std::make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++)
        boxes2.add(std::make_shared<Sphere>(Point3::getRandomVector(0, 165), 10, white));

    world.add(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BVHNode>(boxes2), 15), Vec3(-100, 270, 395)));

    

    Camera camera;

    camera.aspectRatio = 1.0;
    camera.imageWidth = imageWidth;
    camera.samplesPerPixel = samplesPerPixel;
    camera.maxDepth = maxDepth;
    camera.backgroundColor = Color(0, 0, 0);

    camera.verticalFOV = 40;
    camera.lookFrom = Point3(478, 278, -600);
    camera.lookAt = Point3(278, 278, 0);
    camera.upVector = Vec3(0, 1, 0);

    camera.defocusAngle = 0;

    camera.render(world);
}



int main() {
    //renderBouncingSpheres();
    //renderCheckeredSpheres();
    //renderEarth();
    //renderPerlinSpheres();
    //renderQuads();
    //renderSimpleLight();
    //renderCornellBox();
    //renderCornellSmoke();
    renderFinalScene(800, 10000, 40);   
    //renderFinalScene(400, 250, 4);

    return 0;
}
