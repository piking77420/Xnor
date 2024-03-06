#pragma once

#include <cmath>

#include "core.hpp"
#include <array>

/// @file gamepad_input.hpp
/// @brief Defines gamepad input types and enumerations.
/// 
/// Defines the GamepadInput struct and the GamepadAxis and GamepadButton enumerations.

BEGIN_XNOR_CORE

/// @brief Gamepad axis enumeration
enum class GamepadAxis : uint32_t
{
    LeftStickHorizontalAxis,
    LeftStickVerticalAxis,
    RightStickHorizontalAxis,
    RightStickVerticalAxis,
    GamepadLtAxis,
    GamepadRtAxis,

    Count
};

/// @brief Gamepad button enumeration
enum class GamepadButton
{
    GamepadButtonA,               
    GamepadButtonB,               
    GamepadButtonX,              
    GamepadButtonY,               
    GamepadButtonLeftBumper,     
    GamepadButtonRightBumper,    
    GamepadButtonBack,            
    GamepadButtonStart,           
    GamepadButtonGuide,          
    GamepadButtonLeftThumb,      
    GamepadButtonRightThumb,     
    GamepadButtonDpadUp,         
    GamepadButtonDpadRight,     
    GamepadButtonDpadDown,      
    GamepadButtonDpadLeft, 
    GamepadButtonLast       = GamepadButtonDpadLeft,
    GamepadButtonCross      = GamepadButtonA,
    GamepadButtonCircle     = GamepadButtonB,
    GamepadButtonSquare     = GamepadButtonX,
    GamepadButtonTriangle   = GamepadButtonY,

    Count = 20
};

/// @brief Gamepad button
enum class GamepadButtonStatus : uint8_t
{
    /// @brief Pressed
    Pressed,
    /// @brief Held down
    Down,
    /// @brief Released
    Release,
    
    Count
};

/// @brief Threshold that dictates that a stick analog value become 0
constexpr float_t NullAnalogValue = 1.5259022e-05f;

/// @brief Information about a gamepad
struct GamepadInput
{
    bool_t isConnected = false;
    std::array<float_t, static_cast<uint32_t>(GamepadAxis::Count)> axesValue{};
};

END_XNOR_CORE
