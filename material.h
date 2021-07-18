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

class dielectric : public material {
public:
    dielectric(double indexOfRefraction) : indexOfRefraction(indexOfRefraction) {}

    virtual bool scatter(const ray &rIn, const hitRecord &rec,
                         color &attenuation, ray &scattered) const override;

private:
    double indexOfRefraction;
};

bool dielectric::scatter(const ray &rIn, const hitRecord &rec, color &attenuation, ray &scattered) const {
    attenuation = color(1.0, 1.0, 1.0);
    const double refractionRatio = rec.frontFace ? (1.0 / indexOfRefraction) : indexOfRefraction;

    const vec3 unitDirection = unitVector(rIn.getDirection());
    const double cosTheta = fmin(dot(-unitDirection, rec.normal), 1.0);
    const double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    const bool canRefract = refractionRatio * sinTheta <= 1.0;
    vec3 direction;

    if (canRefract) {
        direction = refract(unitDirection, rec.normal, refractionRatio);
    } else {
        direction = reflect(unitDirection, rec.normal);
    }

    scattered = ray(rec.p, direction);
    return true;
}