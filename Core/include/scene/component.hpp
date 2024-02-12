#pragma once

#include "core.hpp"
#include "entity.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Component
{
public:
    Entity* entity = nullptr;

    virtual ~Component() = default;

    virtual void Begin();

    virtual void Update();
};

END_XNOR_CORE
