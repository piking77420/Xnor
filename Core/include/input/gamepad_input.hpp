#pragma once

#include <array>
#include <cmath>

#include "core.hpp"

/// @file gamepad_input.hpp
/// @brief Defines gamepad input types and enumerations.
/// 
/// Defines the XnorCore::GamepadInput struct along with the XnorCore::GamepadAxis and XnorCore::GamepadButton enumerations.

BEGIN_XNOR_CORE

/// @brief Gamepad axis enumeration
enum class GamepadAxis : uint32_t
{
    LeftStickHorizontalAxis,
    LeftStickVerticalAxis,
    RightStickHorizontalAxis,
    RightStickVerticalAxis,
    LeftTriggerAxis,
    RightTriggerAxis,

    Count
};

/// @brief Gamepad button enumeration
///
/// Defines the gamepad button values according to the Xbox controller button placements.
enum class GamepadButton
{
    A,               
    B,               
    X,              
    Y,               
    LeftBumper,     
    RightBumper,    
    Back,            
    Start,           
    Guide,          
    LeftThumb,      
    RightThumb,     
    DpadUp,         
    DpadRight,     
    DpadDown,      
    DpadLeft, 
    Last       = DpadLeft,
    Cross      = A,
    Circle     = B,
    Square     = X,
    Triangle   = Y,

    Count = 20
};

/// @brief Gamepad button
enum class GamepadButtonStatus
{
    /// @brief Pressed
    Pressed,
    /// @brief Held down
    Down,
    /// @brief Released
    Release,
    
    Count
};

/// @brief Information about a gamepad
struct GamepadInput
{
    /// @brief Threshold that dictates that a stick analog value becomes 0.
    static constexpr float_t NullAnalogValue = 1.5259022e-05f;

    /// @brief Whether the gamepad is connected.
    bool_t isConnected = false;
    /// @brief Array of axis analog values.
    std::array<float_t, static_cast<uint32_t>(GamepadAxis::Count)> axisValues;
};

END_XNOR_CORE
