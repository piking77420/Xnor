#pragma once

#include "core.hpp"
#include "Maths/vector3.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

/// @private
class TestComponent : public Component
{
    REFLECTABLE_IMPL_MINIMAL(TestComponent)
    
public:
    XNOR_ENGINE TestComponent() = default;
    
    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;
    
private:
    float_t m_RotationSpeed = 1.0f;
    
    float_t m_Radius = 1.0f;

    float_t m_CurrentAngle = 0.f;
    
    Vector3 m_BasePosition;
    
    bool_t m_Rotate = true;

    Entity* m_Test;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::TestComponent, bases<XnorCore::Component>),
    field(m_Rotate),
    field(m_CurrentAngle),
    field(m_RotationSpeed),
    field(m_Radius),
    field(m_Test)
);
