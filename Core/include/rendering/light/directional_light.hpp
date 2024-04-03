﻿#pragma once

#include <Maths/vector2i.hpp>

#include "rendering/light/light.hpp"

/// @file directional_light.hpp
/// @brief Defines the XnorCore::DirectionalLight class

BEGIN_XNOR_CORE

/// @brief Handles a directional light
class DirectionalLight : public Light
{
    REFLECTABLE_IMPL(DirectionalLight)

    constexpr static inline Vector2 DefaulNearFar = { 1.0f, 7.5f };
    
public:
    /// @brief Orthographic left/right for the shadow map
    Vector2 leftRight = { -10.f, 10.f };

    /// @brief Orthographic bottom/top for the shadow map
    Vector2 bottomTop = { -10.f, 10.f };

    XNOR_ENGINE DirectionalLight();
    XNOR_ENGINE ~DirectionalLight() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(DirectionalLight)

    /// @brief Gets the directional light direction
    /// @returns Light direction
    [[nodiscard]]
    Vector3 GetLightDirection() const;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::DirectionalLight, bases<XnorCore::Light>))
