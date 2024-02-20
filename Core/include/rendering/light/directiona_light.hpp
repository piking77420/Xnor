#pragma once
#include "light.hpp"

class XNOR_ENGINE Directionalight : public XnorCore::Light
{
public:

    Vector3 direction = Vector3::UnitZ();
    
    Directionalight() = default;
    ~Directionalight() = default;
};
