#pragma once

#include "core.hpp"
#include "utils/serializable.hpp"

BEGIN_XNOR_CORE

class Entity;

class XNOR_ENGINE Component : public Serializable
{
    REFLECTABLE_IMPL_MINIMAL(Component)
    
public:
    Entity* entity = nullptr;

    Component() = default;
    
    ~Component() override = default;

    virtual void Begin();

    virtual void Update();
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Component));
