#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 center, double radius) : center(center), radius(radius) {}

    virtual bool hit(const ray &r, double tMin, double tMax, hitRecord &rec) const override;

private:
    point3 center;
    double radius;
};

bool sphere::hit(const ray &r, double tMin, double tMax, hitRecord &rec) const {
    // vector from center to point of the ray
    vec3 oc = r.getOrigin() - center;
    // (t^2) * b dot b + 2tb dot (A-C) + (A-C) dot (A-C) - r*2 = 0
    // where t is variable
    const auto a = r.getDirection().lengthSquared();
    const auto half_b = dot(oc, r.getDirection());
    const auto c = oc.lengthSquared() - radius * radius;
    // discriminant of quadratic polynomial where a != 0 is b^2 - 4ac
    const auto discriminant = half_b * half_b - a * c;
    // positive (meaning two real solutions)
    // negative (meaning no real solutions)
    // zero (meaning one real solution)
    // solving for t
    if (discriminant < 0) {
        return false;
    }

    const auto sqrtDiscriminant = sqrt(discriminant);

    // find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrt(discriminant)) / a;
    if (root < tMin || root > tMax) {
        root = (-half_b + sqrt(discriminant)) / a;
        if (root < tMin || root > tMax) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);

    // normal as unit vector
    const vec3 outwardNormal = (rec.p - center) / radius;

    rec.setFaceNormal(r, outwardNormal);

    return true;
}