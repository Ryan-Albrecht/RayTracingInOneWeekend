#pragma once

#include "rtweekend.h"

struct hitRecord;

class material {
public:
    virtual bool scatter(const ray &rIn, const hitRecord &rec,
                         color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color &albedo) : albedo(albedo) {}

    virtual bool scatter(const ray &rIn, const hitRecord &rec,
                         color &attenuation, ray &scattered) const override;

private:
    color albedo;
};

bool lambertian::scatter(const ray &rIn, const hitRecord &rec, color &attenuation, ray &scattered) const {
    auto scatterDirection = rec.normal + randomUnitVector();

    // Catch degenerate scatter direction
    if (scatterDirection.nearZero()) {
        scatterDirection = rec.normal;
    }

    scattered = ray(rec.p, scatterDirection);
    attenuation = albedo;
    return true;
}

class metal : public material {
public:
    metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    virtual bool scatter(const ray &rIn, const hitRecord &rec,
                         color &attenuation, ray &scattered) const override;

private:
    color albedo;
    double fuzz;
};

bool metal::scatter(const ray &rIn, const hitRecord &rec, color &attenuation, ray &scattered) const {
    vec3 reflected = reflect(unitVector(rIn.getDirection()), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * vec3::randomInUnitSphere());
    attenuation = albedo;
    return (dot(scattered.getDirection(), rec.normal) > 0);
}