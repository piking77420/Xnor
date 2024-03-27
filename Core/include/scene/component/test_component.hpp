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
    Light* m_Light = nullptr;

    ColorHsva m_Color;

    float_t m_RotationSpeed = 1.0f;

    float_t m_Radius = 1.0f;

    float_t m_CurrentAngle = 0.f;

    Vector3 m_BasePosition;

    bool_t m_Rotate = true;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::TestComponent, bases<XnorCore::Component>),
    field(m_Rotate),
    field(m_CurrentAngle, XnorCore::Reflection::NotSerializable()),
    field(m_RotationSpeed),
    field(m_Radius)
);
