#pragma once

#include "core.hpp"
#include "Maths/vector3.hpp"
#include "reflection/reflection.hpp"
#include "rendering/light/light.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

/// @private
class TestComponent : public Component
{
    REFLECTABLE_IMPL_MINIMAL_DLL(TestComponent)
    
public:
    XNOR_ENGINE void Begin() override;

    XNOR_ENGINE void Update() override;
    
private:
    Light* m_Light = nullptr;

    ColorHsva m_Color;
    
    float_t m_RotationSpeed = 1.0f;
    
    float_t m_Radius = 1.0f;

    float_t m_CurrentAngle = 0.f;
    
    Vector3 m_BasePosition;

    bool_t m_Rotate = true;

    Vector2 m_PlottingTest1;
    Vector2 m_PlottingTest2;

    List<int32_t> m_List;

    const int32_t m_ConstTest = 0;
    static inline int32_t m_StaticTest1 = 5;
    static inline int32_t m_StaticTest2 = 10;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::TestComponent, bases<XnorCore::Component>),
    field(m_Rotate),
    field(m_CurrentAngle),
    field(m_RotationSpeed),
    field(m_Radius),
    field(m_PlottingTest1, XnorCore::Reflection::GridPlotting(0.f, 1.f)),
    field(m_PlottingTest2, XnorCore::Reflection::GridPlotting(-1.f, 1.f)),
    field(m_List, XnorCore::Reflection::Tooltip("I'm a list"), XnorCore::Reflection::Range<int32_t>(0, 5)),
    field(m_ConstTest),
    field(m_StaticTest1),
    field(m_StaticTest2)
);
