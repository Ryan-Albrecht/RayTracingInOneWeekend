#include "rtweekend.h"

#include "color.h"
#include "hittableList.h"
#include "sphere.h"

#include <iostream>

color rayColor(const ray &r, const hittable &world) {
    hitRecord rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
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

    // World
    hittableList world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera

    const auto viewportHeight = 2.0;
    const auto viewportWidth = aspectRatio * viewportHeight;
    const auto focalLength = 1.0;

    const auto origin = point3(0, 0, 0);
    const auto horizontal = vec3(viewportWidth, 0, 0);
    const auto vertical = vec3(0, viewportHeight, 0);
    const auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);

    // Render to PPM (Portable Pix Map) image format

    std::cout << "P3\n"
              << imageWidth << " " << imageHeight << "\n255\n";

    for (int y = imageHeight - 1; y >= 0; y--) {
        std::cerr << "\rScanlines remaining: " << y << " ";
        for (int x = 0; x < imageWidth; x++) {
            const auto u = double(x) / (imageWidth - 1);
            const auto v = double(y) / (imageHeight - 1);
            ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
            color pixelColor = rayColor(r, world);
            writeColor(std::cout, pixelColor);
        }
    }

    std::cerr << "\nDone.\n";
}