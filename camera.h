#pragma once

#include "rtweekend.h"

class camera {
public:
    // verticalFieldOfView is in degrees
    camera(point3 lookFrom, point3 lookAt, vec3 vup, double verticalFieldOfView, double aspectRatio) {
        const auto theta = degreesToRadians(verticalFieldOfView);
        const auto height = tan(theta / 2);
        const auto viewportHeight = 2.0 * height;
        const auto viewportWidth = aspectRatio * viewportHeight;

        const auto w = unitVector(lookFrom - lookAt);
        const auto u = unitVector(cross(vup, w));
        const auto v = cross(w, u);

        origin = lookFrom;
        horizontal = viewportWidth * u;
        vertical = viewportHeight * v;
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - w;
    }

    ray getRay(double s, double t) const {
        return ray(origin, lowerLeftCorner + s * horizontal + t * vertical - origin);
    }

private:
    point3 origin;
    point3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
};