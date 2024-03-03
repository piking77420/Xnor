#pragma once

#include "core.hpp"
#include "light.hpp"

BEGIN_XNOR_CORE

class PointLight :  public Light
{
    REFLECTABLE_IMPL_MINIMAL(PointLight)
    
public:
    XNOR_ENGINE PointLight() = default;

    XNOR_ENGINE ~PointLight() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(PointLight)
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::PointLight, bases<XnorCore::Light>)
);

