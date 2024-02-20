#pragma once

#include "core.hpp"
#include "light.hpp"

BEGIN_XNOR_CORE
class PointLight :  public Light
{
public:
    XNOR_ENGINE PointLight() = default;

   XNOR_ENGINE ~PointLight() = default;
};
END_XNOR_CORE
