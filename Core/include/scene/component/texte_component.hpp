#pragma once
#include "gui_component.hpp"
#include "resource/font.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE
class TexteComponent : public GuiComponent
{
    REFLECTABLE_IMPL(Button)
public:
    XnorCore::Pointer<XnorCore::Font> font;

    std::string text = "text";

    XnorCore::Colorf color = Colorf::White();
    
    float_t police = 1.f;

    DEFAULT_COPY_MOVE_OPERATIONS(TexteComponent)

    TexteComponent() = default;

    ~TexteComponent() = default;
};

END_XNOR_CORE


REFL_AUTO(
    type(XnorCore::TexteComponent, bases<XnorCore::GuiComponent>),
    field(font),
    field(text),
    field(color),
    field(police)
);
