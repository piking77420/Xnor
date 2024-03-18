#pragma once

#include "light.hpp"

/// @file directional_light.hpp
/// @brief Defines the XnorCore::DirectionalLight class

BEGIN_XNOR_CORE

/// @brief Handles a directional light
class DirectionalLight : public Light
{
    REFLECTABLE_IMPL_MINIMAL_DLL(DirectionalLight)

public:
    XNOR_ENGINE DirectionalLight() = default;
    XNOR_ENGINE ~DirectionalLight() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(DirectionalLight)
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::DirectionalLight, bases<XnorCore::Light>))
