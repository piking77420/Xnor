#pragma once

#include <Maths/vector2i.hpp>

#include "rendering/light/light.hpp"

/// @file directional_light.hpp
/// @brief Defines the XnorCore::DirectionalLight class

BEGIN_XNOR_CORE

/// @brief Handles a directional light
class DirectionalLight : public Light
{
    REFLECTABLE_IMPL(DirectionalLight)

public:
    float_t near = 1.0f;

    float_t far = 7.5f;

    Vector2 leftRight = { -10.f ,10.f };

    Vector2 bottomtop = { -10.f ,10.f };
    
    XNOR_ENGINE DirectionalLight() = default;
    
    XNOR_ENGINE ~DirectionalLight() override = default;

    Vector3 GetLightDirection() const;
    
    DEFAULT_COPY_MOVE_OPERATIONS(DirectionalLight)
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::DirectionalLight, bases<XnorCore::Light>))
