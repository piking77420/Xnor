#pragma once

#include <Maths/vector3.hpp>

#include "core.hpp"
#include "reflection/reflection.hpp"
#include "rendering/light/light.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

/// @private
class TestComponent : public Component
{
    REFLECTABLE_IMPL(TestComponent)
    
public:
    DEFAULT_COPY_MOVE_OPERATIONS(TestComponent)
    
    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;

    XNOR_ENGINE TestComponent() = default;
    XNOR_ENGINE ~TestComponent() override = default;

private:
    enum TestEnum
    {
        None = 0,
        One = 1 << 0,
        Two = 1 << 1,
        Four = 1 << 2,
        Eight = 1 << 3,
        Sixteen = 1 << 4
    };
    
    Light* m_Light = nullptr;

    ColorHsva m_Color;
    
    float_t m_RotationSpeed = 1.0f;
    
    float_t m_Radius = 1.0f;

    float_t m_CurrentAngle = 0.f;
    
    Vector3 m_BasePosition;

    bool_t m_Rotate = true;

    Vector2 m_PlottingTest1;
    Vector2 m_PlottingTest2;

    List<int32_t> m_List = { 0, 1, 2, 3, 4, 5 };
    float_t m_Array[5] = { 0, 1, 2.5, 6, 8 };
    TestEnum m_Enum = static_cast<TestEnum>(One | Sixteen);

    std::string m_String = "Hello";

    int32_t m_ConstTest = 0;
    static inline int32_t m_StaticTest1 = 5;
    static inline int32_t m_StaticTest2 = 10;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::TestComponent, bases<XnorCore::Component>),
    field(m_Rotate),
    field(m_CurrentAngle, XnorCore::Reflection::NotSerializable()),
    field(m_RotationSpeed),
    field(m_Radius),
    field(m_PlottingTest1, XnorCore::Reflection::GridPlotting(0.f, 1.f)),
    field(m_PlottingTest2, XnorCore::Reflection::GridPlotting(-1.f, 1.f)),
    field(m_List, XnorCore::Reflection::Tooltip("I'm a list"), XnorCore::Reflection::Range<int32_t>(0, 5)),
    field(m_Array),
    field(m_Enum, XnorCore::Reflection::EnumFlags()),
    field(m_String),
    field(m_ConstTest),
    field(m_StaticTest1),
    field(m_StaticTest2)
);
