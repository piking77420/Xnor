#pragma once
#include "core.hpp"
#include "Maths/vector2i.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE
class GuiComponent : public Component
{
    REFLECTABLE_IMPL(GuiComponent)
public:
    Vector2 screenTransform;

    Vector2 size;

    DEFAULT_COPY_MOVE_OPERATIONS(GuiComponent)
    
    GuiComponent() = default;

    ~GuiComponent() override = default;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::GuiComponent, bases<XnorCore::Component>),
    field(screenTransform),
    field(size)
);
