#pragma once
#include "light.hpp"

BEGIN_XNOR_CORE

class SpotLight : public XnorCore::Light
{
public:
    Vector3 direction = Vector3::UnitZ();

    float_t cutOff = 12.5f * Calc::Deg2Rad;	
    float_t outerCutOff = 15.f * Calc::Deg2Rad;
    
    XNOR_ENGINE SpotLight() = default;
    
    XNOR_ENGINE ~SpotLight() = default;
};

END_XNOR_CORE
