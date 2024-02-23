#pragma once

#include "core.hpp"
#include "utils/reflectable.hpp"

BEGIN_XNOR_CORE

class Entity;

class XNOR_ENGINE Component : public Reflectable
{
    REFLECTABLE_IMPL(Component)
    
public:
    Entity* entity = nullptr;

    Component();
    ~Component() override = default;

    virtual void Begin();

    virtual void Update();
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Component));
