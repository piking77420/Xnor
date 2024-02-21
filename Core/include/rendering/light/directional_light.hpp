#pragma once

#include "light.hpp"

BEGIN_XNOR_CORE

class DirectionalLight : public Light
{
public:
    Vector3 direction = Vector3::UnitZ();
    
    XNOR_ENGINE DirectionalLight() = default;
    XNOR_ENGINE ~DirectionalLight() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(DirectionalLight)
};

END_XNOR_CORE
