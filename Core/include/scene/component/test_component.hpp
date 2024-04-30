#pragma once

#include <Maths/vector3.hpp>

#include "core.hpp"
#include "transform.hpp"
#include "input/keyboard_input.hpp"
#include "reflection/reflection.hpp"
#include "rendering/light/light.hpp"
#include "scene/component.hpp"
#include "utils/coroutine.hpp"
#include "utils/guid.hpp"
#include "utils/timeline.hpp"

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

    Entity* m_TestEntity = nullptr;

    Guid coroutineId;

    static inline int32_t m_StaticTest = 1;
    static inline Transform m_StaticTransform {};
    Vector2 m_GridPlottingTest;
    float_t m_Array[2][3];
    List<int32_t> m_List;

    ENUM_VALUE(Key) m_Key;

    Coroutine FlashRoutine();

    Timeline<float_t> m_Timeline;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::TestComponent, bases<XnorCore::Component>),
    field(m_Rotate),
    field(m_CurrentAngle, XnorCore::Reflection::NotSerializable()),
    field(m_RotationSpeed),
    field(m_Radius),
    field(m_StaticTest),
    field(m_StaticTransform),
    field(m_TestEntity),
    field(m_GridPlottingTest, XnorCore::Reflection::GridPlotting(0, 1)),
    field(m_Array, XnorCore::Reflection::NotSerializable()),
    field(m_List, XnorCore::Reflection::NotSerializable()),
    field(m_Key),
    field(m_Timeline)
);
