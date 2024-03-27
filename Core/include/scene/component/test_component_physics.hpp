#pragma once

#include "scene/component.hpp"

BEGIN_XNOR_CORE

class TestComponentPhysics : public Component
{
    REFLECTABLE_IMPL(TestComponentPhysics)

public:
    DEFAULT_COPY_MOVE_OPERATIONS(TestComponentPhysics)
    
    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;

    XNOR_ENGINE TestComponentPhysics() = default;
    XNOR_ENGINE ~TestComponentPhysics() override = default;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::TestComponentPhysics, bases<XnorCore::Component>));
