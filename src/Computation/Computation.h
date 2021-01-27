#pragma once

#include "Intersection.h"
#include "Ray.h"
#include "Tuple.h"

class Computation {
public:
    float t;
    Object* object;
    Tuple point;
    Tuple eyeDirection;
    Tuple normal;
    bool inside;
};


Computation prepareComputation(Intersection const &i, Ray const &r);