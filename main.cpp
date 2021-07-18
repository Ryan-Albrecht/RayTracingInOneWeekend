#include "camera.h"
#include "color.h"
#include "hittableList.h"
#include "material.h"
#include "rtweekend.h"
#include "sphere.h"

#include <iostream>

hittableList randomScene() {
    hittableList world;

    const auto groundMaterial = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            const auto chooseMaterial = randomDouble();
            const point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphereMaterial;

                if (chooseMaterial < 0.8) {
                    // diffuse
                    const auto albedo = color::random() * color::random();
                    sphereMaterial = make_shared<lambertian>(albedo);
                } else if (chooseMaterial < 0.95) {
                    // metal
                    const auto albedo = color::random(0.5, 1);
                    const auto fuzz = randomDouble(0, 0.5);
                    sphereMaterial = make_shared<metal>(albedo, fuzz);
                } else {
                    // glass
                    sphereMaterial = make_shared<dielectric>(1.5);
                }
                world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
            }
        }
    }

    const auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    const auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    const auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

color rayColor(const ray &r, const hittable &world, int depth) {
    hitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    // 0.001 adds tolerance to fix Shadow Acne
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.materialPtr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * rayColor(scattered, world, depth - 1);
        }
        return color(0, 0, 0);
    }

    const vec3 unitDirection = unitVector(r.getDirection());
    const auto t = 0.5 * (unitDirection.y() + 1.0);
    // linear blend where t = 1 is blue, t = 0 is white, and in between is a blend.
    // formula is blendedValue=(1−t) * startValue+ t * endValue,
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    const double aspectRatio = 3.0 / 2.0;
    const int imageWidth = 1200;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 500;
    const int maxDepth = 50;

    // World

    hittableList world = randomScene();

    // Camera

    const point3 lookFrom(13, 2, 3);
    const point3 lookAt(0, 0, 0);
    const vec3 vup(0, 1, 0);
    const auto distToFocus = 10.0;
    const auto aperture = 0.1;
    const auto verticalFieldOfView = 20;

    camera cam(lookFrom, lookAt, vup, verticalFieldOfView, aspectRatio, aperture, distToFocus);

    // Render to PPM (Portable Pix Map) image format

    std::cout << "P3\n"
              << imageWidth << " " << imageHeight << "\n255\n";

    for (int y = imageHeight - 1; y >= 0; y--) {
        std::cerr << "\rScanlines remaining: " << y << " ";
        for (int x = 0; x < imageWidth; x++) {
            color pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; s++) {
                auto u = (x + randomDouble()) / (imageWidth - 1);
                auto v = (y + randomDouble()) / (imageHeight - 1);

                const ray r = cam.getRay(u, v);
                pixelColor += rayColor(r, world, maxDepth);
            }
            writeColor(std::cout, pixelColor, samplesPerPixel);
        }
    }

    std::cerr << "\nDone.\n";
}