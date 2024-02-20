#pragma once

#include "core.hpp"
#include "Maths/vector3.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE
class Light : public Component
{
public:
    
    Vector3 color = 1.f;
    float_t intensity = 1.f;

    XNOR_ENGINE Light() = default;

    XNOR_ENGINE ~Light() = default;
};

END_XNOR_CORE
