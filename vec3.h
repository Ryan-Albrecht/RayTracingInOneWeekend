#pragma once

#include <cmath>
#include <iostream>

class vec3 {
public:
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double &operator[](int i) { return e[i]; }

    vec3 &operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];

        return *this;
    }

    vec3 &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return std::sqrt(lengthSquared());
    }

    double lengthSquared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    // pick a random point in the unit cube where x, y, and z all range from [0, 1.0)
    inline static vec3 random() {
        return vec3(randomDouble(), randomDouble(), randomDouble());
    }

    // pick a random point in the cube where x, y, and z all range from [min, max)
    inline static vec3 random(double min, double max) {
        return vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
    }

    // rejection method algorithm
    //  1. Pick a random point in the unit cube where x, y, and z all range from [-1, 1)
    //  2. Reject this point and try again if the point is outside the sphere
    static vec3 randomInUnitSphere() {
        while (true) {
            auto p = vec3::random(-1, 1);
            if (p.lengthSquared() >= 1) {
                continue;
            }
            return p;
        }
    }

    bool nearZero() const {
        // Returns true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return fabs(e[0]) < s && fabs(e[1]) < s && fabs(e[2]) < s;
    }

    double e[3];
};

// type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

// vec3 Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unitVector(vec3 v) {
    return v / v.length();
}

static vec3 randomUnitVector() {
    return unitVector(vec3::randomInUnitSphere());
}

vec3 randomInHemisphere(const vec3 &normal) {
    vec3 inUnitSphere = vec3::randomInUnitSphere();
    if (dot(inUnitSphere, normal) > 0.0) {
        // In the same hemisphere as the normal
        return inUnitSphere;
    } else {
        return -inUnitSphere;
    }
}

vec3 reflect(const vec3 &incomingRay, const vec3 &normal) {
    return incomingRay - 2 * dot(incomingRay, normal) * normal;
}

vec3 refract(const vec3 &incomingRay, const vec3 &normal, double etaiOverEtat) {
    const auto cosTheta = fmin(dot(-incomingRay, normal), 1.0);
    vec3 rOutPerpendicular = etaiOverEtat * (incomingRay + cosTheta * normal);
    vec3 rOutParallel = -sqrt(fabs(1.0 - rOutPerpendicular.lengthSquared())) * normal;
    return rOutPerpendicular + rOutParallel;
}

// Used in Defocus Blur
vec3 randomInUnitDisk() {
    while (true) {
        const auto p = vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
        if (p.lengthSquared() >= 1) {
            continue;
        }
        return p;
    }
}