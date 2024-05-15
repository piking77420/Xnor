#pragma once

#include "gui_component.hpp"
#include "resource/font.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class TextComponent : public GuiComponent
{
public:
    Pointer<Font> font;

    std::string text = "text";

    Color color = Color::White();
    
    float_t fontSize = 1.f;

    DEFAULT_COPY_MOVE_OPERATIONS(TextComponent)

    TextComponent() = default;

    ~TextComponent() override = default;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::TextComponent, bases<XnorCore::GuiComponent>),
    field(font),
    field(text),
    field(color),
    field(fontSize)
);
