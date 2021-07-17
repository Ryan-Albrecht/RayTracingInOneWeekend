#pragma once

#include "vec3.h"

#include <iostream>

void writeColor(std::ostream &out, color pixelColor, unsigned int samplesPerPixel) {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    // Divide the color by the number of samples.
    const auto scale = 1.0 / samplesPerPixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0, 255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0, 0.999)) << " "
        << static_cast<int>(256 * clamp(g, 0, 0.999)) << " "
        << static_cast<int>(256 * clamp(b, 0, 0.999)) << "\n";
}