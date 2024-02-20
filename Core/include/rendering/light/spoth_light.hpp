#pragma once
#include "light.hpp"

class XNOR_ENGINE SpothLight : public XnorCore::Light
{
public:
    Vector3 direction = Vector3::UnitZ();

    float_t cutOff = 12.5f * Calc::Deg2Rad;	
    float_t outerCutOff = 15.f * Calc::Deg2Rad;

    SpothLight() = default;
    
    ~SpothLight() = default;
};
