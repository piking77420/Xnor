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
    enum TestEnum : size_t
    {
        None = 0 << 0,
        One = 1 << 0,
        Two = 1 << 1,
        Four = 1 << 2,
        Eight = 1 << 3,
        Sixteen = 1 << 4,
    };
    
    float_t m_RotationSpeed = 1.0f;
    
    float_t m_Radius = 1.0f;

    float_t m_CurrentAngle = 0.f;
    
    Vector3 m_BasePosition;
    
    bool_t m_Rotate = true;

    Entity* m_Test = nullptr;

    TestEnum m_TestEnum = static_cast<TestEnum>(TestEnum::Four | TestEnum::Sixteen);
    
    float_t m_TestRange;

    List<int32_t> m_List;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::TestComponent, bases<XnorCore::Component>),
    field(m_Rotate),
    field(m_CurrentAngle),
    field(m_RotationSpeed),
    field(m_Radius),
    field(m_Test),
    field(m_TestEnum, XnorCore::Reflection::EnumFlags()),
    field(m_TestRange, XnorCore::Reflection::Range<float_t>(0.f, 5.f), XnorCore::Reflection::Tooltip("This is a tooltip test")),
    field(m_List, XnorCore::Reflection::Tooltip("I'm a list"))
);
