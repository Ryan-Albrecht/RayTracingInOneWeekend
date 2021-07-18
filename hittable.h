#pragma once

#include "rtweekend.h"

class material;

struct hitRecord {
    point3 p;
    vec3 normal;
    shared_ptr<material> materialPtr;
    double t;
    bool frontFace;

    inline void setFaceNormal(const ray &r, const vec3 &outwardNormal) {
        frontFace = dot(r.getDirection(), outwardNormal) < 0;
        normal = (frontFace ? 1 : -1) * outwardNormal;
    }
};

class hittable {
public:
    virtual bool hit(const ray &r, double tMin, double tMax, hitRecord &rec) const = 0;
};