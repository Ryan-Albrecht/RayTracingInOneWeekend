#pragma once

#include "rtweekend.h"

struct hitRecord;

class material {
public:
    virtual bool scatter(const ray &rIn, const hitRecord &rec,
                         color &attenuation, ray &scattered) const = 0;
};