#pragma once
#include "light.hpp"

BEGIN_XNOR_CORE

class Directionalight : public XnorCore::Light
{
public:

    Vector3 direction = Vector3::UnitZ();
    
   XNOR_ENGINE Directionalight() = default;
   XNOR_ENGINE ~Directionalight() = default;
};
END_XNOR_CORE
