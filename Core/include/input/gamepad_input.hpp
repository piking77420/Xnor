#pragma once

#include <cmath>

#include "core.hpp"
#include <array>

/// @file gamepad_input.hpp
/// @brief Defines gamepad input types and enumerations.
/// 
/// Defines the @ref GamepadInput struct and the @ref GamepadAxes and @ref GamepadButton enumerations.

BEGIN_XNOR_CORE

enum class GamepadAxes : uint32_t
{
    LeftStickHorizontalAxis,
    LeftStickVerticalAxis,
    RightStickHorizontalAxis,
    RightStickVerticalAxis,
    GamepadLtAxis,
    GamepadRtAxis,

    Count
};

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

constexpr double_t NullAnalogValue = 1.5259022e-05;

struct GamepadInput
{
    bool isConnected = false;
    std::array<float_t,static_cast<uint32_t>(GamepadAxes::Count)> axesValue;
};

END_XNOR_CORE
