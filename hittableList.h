#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittableList : public hittable {
public:
    hittableList() {}
    hittableList(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray &r, double tMin, double tMax, hitRecord &rec) const override;

private:
    std::vector<shared_ptr<hittable>> objects;
};

bool hittableList::hit(const ray &r, double tMin, double tMax, hitRecord &rec) const {
    hitRecord tempRec;
    bool hitAnything = false;
    auto closestSoFar = tMax;

    for (const auto &object : objects) {
        if (object->hit(r, tMin, closestSoFar, tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}