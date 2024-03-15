#pragma once

#include "core.hpp"

/// @file mouse_input.hpp
/// @brief Defines enumerators related to mouse input

BEGIN_XNOR_CORE

/// @brief %List of mouse buttons
BEGIN_ENUM(MouseButton)
{
    /// @brief Left click
    Left,
    /// @brief Right click
    Right,
    /// @brief Wheel click
    Middle,
    /// @brief Extension button 3
    Button3,
    /// @brief Extension button 4
    Button4,
    /// @brief Extension button 5
    Button5,
    /// @brief Extension button 6
    Button6,
    /// @brief Extension button 7
    Button7,
    /// @brief Extension button 8
    Button8,
    
    Count
};
END_ENUM

/// @brief %List of mouse button states
BEGIN_ENUM(MouseButtonStatus)
{
    /// @brief Pressed
    Pressed,
    /// @brief Held down
    Down,
    /// @brief Released
    Release,
    
    Count
}
END_ENUM

END_XNOR_CORE
