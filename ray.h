#pragma once

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const point3 &origin, const vec3 &direction) : origin(origin), direction(direction) {}

    point3 getOrigin() const { return origin; }
    vec3 getDirection() const { return direction; }

    point3 at(double t) const {
        return origin + t * direction;
    }

private:
    point3 origin;
    vec3 direction;
};