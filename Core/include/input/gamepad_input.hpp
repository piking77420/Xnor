#pragma once

#include <cmath>

#include "core.hpp"
#include <array>

BEGIN_XNOR_CORE

enum class GamePadAxes : uint32_t
{
    LeftStickHorizontalAxis,
    LeftStickVerticalAxis,
    RightStickHorizontalAxis,
    RightStickVerticalAxis,
    GamePadLTAxis,
    GamePadRTAxis,

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
    GamepadButtonLast        =    GamepadButtonDpadLeft,
    GamepadButtonCross      =  GamepadButtonA,
    GamepadButtonCircle     = GamepadButtonB,
    GamepadButtonSquare     = GamepadButtonX,
    GamepadButtonTriangle   = GamepadButtonY,

    Count = 20
};

constexpr double_t NullAnalogValue = 1.5259022e-05;

struct GamepadInput
{
    bool isConnected = false;
    std::array<float_t,static_cast<uint32_t>(GamePadAxes::Count)> m_AxesValue;
};

END_XNOR_CORE