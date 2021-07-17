#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hitSphere(const point3 &center, double radius, const ray &r) {
    // vector from center to point of the ray
    vec3 oc = r.getOrigin() - center;
    // (t^2) * b dot b + 2tb dot (A-C) + (A-C) dot (A-C) - r*2 = 0
    // where t is variable
    const auto a = dot(r.getDirection(), r.getDirection());
    const auto b = 2.0 * dot(oc, r.getDirection());
    const auto c = dot(oc, oc) - radius * radius;
    // discriminant of quadratic polynomial where a != 0 is b^2 - 4ac
    const auto discriminant = b * b - 4 * a * c;
    // positive (meaning two real solutions)
    // negative (meaning no real solutions)
    // zero (meaning one real solution)
    // solving for t
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

color rayColor(const ray &r) {
    const point3 circleCenter = point3(0, 0, -1.0);
    auto t = hitSphere(circleCenter, 0.5, r);
    if (t > 0.0) {
        // surface normal vector from hit point
        vec3 N = unitVector(r.at(t) - circleCenter);
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    const vec3 unitDirection = unitVector(r.getDirection());
    t = 0.5 * (unitDirection.y() + 1.0);
    // linear blend where t = 1 is blue, t = 0 is white, and in between is a blend.
    // formula is blendedValue=(1−t) * startValue+ t * endValue,
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image

    const double aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

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
            color pixelColor = rayColor(r);
            writeColor(std::cout, pixelColor);
        }
    }

    std::cerr << "\nDone.\n";
}