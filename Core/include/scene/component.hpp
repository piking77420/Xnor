#pragma once

#include "core.hpp"
#include "entity.hpp"

class XNOR_ENGINE Component
{
public:
    Entity* entity = nullptr;

    virtual ~Component() = default;

    virtual void Begin();

    virtual void Update();
};

class TestComponent : public Component
{
public:
    char data;
};
