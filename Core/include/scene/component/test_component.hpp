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
    XNOR_ENGINE TestComponent() = default;
    
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
    Vector2 m_PlottingTest3;
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
    field(m_PlottingTest3, XnorCore::Reflection::GridPlotting(0.f, 100.f))
);
