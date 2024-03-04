#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

enum class MouseButton : uint8_t
{
    Left,
    Right,
    Middle,
    Button3,
    Button4,
    Button5,
    Button6,
    Button7,
    Button8,
    
    Count
};

enum class MouseButtonStatus : uint8_t
{
    Press,
    Down,
    Release,
    
    Count
};

END_XNOR_CORE
