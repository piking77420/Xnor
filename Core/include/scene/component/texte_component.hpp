#pragma once
#include "resource/font.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE
class TexteComponent : public Component
{
public:
    XnorCore::Pointer<XnorCore::Font> font;

    std::string text;

    XnorCore::Colorf color;

    Vector2 screenSpaceTransform;

    float_t police;

    DEFAULT_COPY_MOVE_OPERATIONS(TexteComponent)

    TexteComponent() = default;

    ~TexteComponent() = default;
};

END_XNOR_CORE