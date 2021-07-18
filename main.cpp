#include "camera.h"
#include "color.h"
#include "hittableList.h"
#include "material.h"
#include "rtweekend.h"
#include "sphere.h"

#include <iostream>

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

    const double aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    // World
    hittableList world;

    const auto materialGround = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    const auto materialCenter = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    const auto materialLeft = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    const auto materialRight = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, materialGround));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, materialCenter));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, materialRight));

    // Camera

    camera cam;

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