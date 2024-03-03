#pragma once

#include "light.hpp"

BEGIN_XNOR_CORE

class SpotLight : public Light
{
    REFLECTABLE_IMPL_MINIMAL(SpotLight)

public:
    float_t cutOff = 12.5f * Calc::Deg2Rad;	
    float_t outerCutOff = 15.f * Calc::Deg2Rad;
    
    XNOR_ENGINE SpotLight() = default;
    
    XNOR_ENGINE ~SpotLight() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(SpotLight)
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::SpotLight, bases<XnorCore::Light>),
    field(cutOff),
    field(outerCutOff)
);
