#pragma once

#include "physics/components/collider.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

/// @private
class TestComponentPhysics : public Component
{
    REFLECTABLE_IMPL(TestComponentPhysics)

public:
    DEFAULT_COPY_MOVE_OPERATIONS(TestComponentPhysics)
    
    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;

    XNOR_ENGINE TestComponentPhysics() = default;
    XNOR_ENGINE ~TestComponentPhysics() override = default;

private:
    Collider* m_Collider = nullptr;
    float_t m_JumpForce = 1.f;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::TestComponentPhysics, bases<XnorCore::Component>),
    field(m_JumpForce)
);
