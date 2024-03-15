#pragma once

#include "core.hpp"
#include "Maths/vector3.hpp"

BEGIN_XNOR_CORE

struct CollisionData
{
    float_t penetrationDepth = 0.f;
    Vector3 normal;
    // Vector3 contactPoints;
};

END_XNOR_CORE
