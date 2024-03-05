#pragma once

#include "light.hpp"

/// @file spot_light.hpp
/// @brief Defines the DirectionalLight class

BEGIN_XNOR_CORE

/// @brief Handles a spot light
class SpotLight : public Light
{
    REFLECTABLE_IMPL_MINIMAL(SpotLight)

public:
    /// @brief Cut-off
    float_t cutOff = 12.5f * Calc::Deg2Rad;
    /// @brief Outer cut-off	
    float_t outerCutOff = 15.f * Calc::Deg2Rad;
    
    XNOR_ENGINE SpotLight() = default;
    
    XNOR_ENGINE ~SpotLight() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(SpotLight)
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::SpotLight, bases<XnorCore::Light>),
    field(cutOff),
    field(outerCutOff)
)
