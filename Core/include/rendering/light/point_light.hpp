#pragma once

#include "core.hpp"
#include "light.hpp"

BEGIN_XNOR_CORE
class XNOR_ENGINE PointLight :  public Light
{
public:
    PointLight() = default;

    ~PointLight() = default;
};
END_XNOR_CORE