#pragma once

#include "core.hpp"
#include "Maths/vector3.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class Light : public Component
{
    REFLECTABLE_IMPL(Light)
    
public:
    Vector3 color = Vector3(1.f);
    float_t intensity = 1.f;

    XNOR_ENGINE Light() = default;

    XNOR_ENGINE ~Light() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Light)
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Light, bases<XnorCore::Component>),
    field(color),
    field(intensity)
);
