#pragma once
#include "core.hpp"
#include "gui_component.hpp"
#include "image.hpp"

BEGIN_XNOR_CORE
class Slider : public GuiComponent
{
    REFLECTABLE_IMPL(Slider)
public:

    enum class Direction
    {
        LeftToRight,
        RightToLeft,
        TopToBottom,
        BottomToTop,
        
    };
    
    float_t value;

    Vector2 minMaxValue;

    Colorf color;

    Image* fillImage = nullptr;

    Image* handleGui = nullptr;

    DEFAULT_COPY_MOVE_OPERATIONS(Slider)

    Slider() = default;

    ~Slider() = default;
    
};

END_XNOR_CORE
REFL_AUTO(
    type(XnorCore::Slider, bases<XnorCore::GuiComponent>),
    field(value));