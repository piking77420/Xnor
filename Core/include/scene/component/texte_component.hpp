#pragma once
#include "gui_component.hpp"
#include "resource/font.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE
class TexteComponent : public GuiComponent
{
public:
    XnorCore::Pointer<XnorCore::Font> font;

    std::string text;

    XnorCore::Colorf color;
    
    float_t police;

    DEFAULT_COPY_MOVE_OPERATIONS(TexteComponent)

    TexteComponent() = default;

    ~TexteComponent() = default;
};

END_XNOR_CORE