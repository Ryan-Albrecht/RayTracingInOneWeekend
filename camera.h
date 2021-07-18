#pragma once

#include "rtweekend.h"

class camera {
public:
    // verticalFieldOfView is in degrees
    camera(point3 lookFrom, point3 lookAt, vec3 vup, double verticalFieldOfView,
           double aspectRatio, double aperture, double focusDist) {
        const auto theta = degreesToRadians(verticalFieldOfView);
        const auto height = tan(theta / 2);
        const auto viewportHeight = 2.0 * height;
        const auto viewportWidth = aspectRatio * viewportHeight;

        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(vup, w));
        v = cross(w, u);

        origin = lookFrom;
        horizontal = focusDist * viewportWidth * u;
        vertical = focusDist * viewportHeight * v;
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - focusDist * w;

        lensRadius = aperture / 2;
    }

    ray getRay(double s, double t) const {
        vec3 rd = lensRadius * randomInUnitDisk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
    }

private:
    point3 origin;
    point3 lowerLeftCorner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lensRadius;
};