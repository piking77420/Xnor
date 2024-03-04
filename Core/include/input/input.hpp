#pragma once

#include <array>
#include <queue>

#include "core.hpp"
#include "gamepad_input.hpp"
#include "window.hpp"

BEGIN_XNOR_CORE

enum class Key : uint16_t
{
       /* Printable keys */
    Space              = 32,
    Apostrophe         = 39,  /* ' */
    Comma              = 44,  /* , */
    Minus              = 45,  /* - */
    Period             = 46,  /* . */
    Slash              = 47,  /* / */
    Key0               = 48,
    Key1               = 49,
    Key2               = 50,
    Key3               = 51,
    Key4               = 52,
    Key5               = 53,
    Key6               = 54,
    Key7               = 55,
    Key8               = 56,
    Key9               = 57,
    Semicolon          = 59, /* ; */
    Equal              = 61, /* = */
    A                  = 65,
    B                  = 66,
    C                  = 67,
    D                  = 68,
    E                  = 69,
    F                  = 70,
    G                  = 71,
    H                  = 72,
    I                  = 73,  // NOLINT(misc-confusable-identifiers)
    J                  = 74,
    K                  = 75,
    L                  = 76,
    M                  = 77,
    N                  = 78,
    O                  = 79,  // NOLINT(misc-confusable-identifiers)
    P                  = 80,
    Q                  = 81,
    R                  = 82,
    S                  = 83,
    T                  = 84,
    U                  = 85,
    V                  = 86,
    W                  = 87,
    X                  = 88,
    Y                  = 89,
    Z                  = 90,
    LeftBracket        = 91 ,  /* [ */
    Backslash          = 92 ,  /* \ */
    RightBracket       = 93 ,  /* ] */
    GraveAccent        = 96 ,  /* ` */
    World1             = 161,  /* non-US #1 */
    World2             = 162,  /* non-US #2 */

    /* Function keys */
    Escape             = 256,
    Enter              = 257,
    Tab                = 258,
    Backspace          = 259,
    Insert             = 260,
    Delete             = 261,
    Right              = 262,
    Left               = 263,
    Down               = 264,
    Up                 = 265,
    PageUp             = 266,
    PageDown           = 267,
    Home               = 268,
    End                = 269,
    CapsLock           = 280,
    ScrollLock         = 281,
    NumLock            = 282,
    PrintScreen        = 283,
    Pause              = 284,
    F1                 = 290,
    F2                 = 291,
    F3                 = 292,
    F4                 = 293,
    F5                 = 294,
    F6                 = 295,
    F7                 = 296,
    F8                 = 297,
    F9                 = 298,
    F10                = 299,
    F11                = 300,
    F12                = 301,
    F13                = 302,
    F14                = 303,
    F15                = 304,
    F16                = 305,
    F17                = 306,
    F18                = 307,
    F19                = 308,
    F20                = 309,
    F21                = 310,
    F22                = 311,
    F23                = 312,
    F24                = 313,
    F25                = 314,
    Kp0                = 320,
    Kp1                = 321,
    Kp2                = 322,
    Kp3                = 323,
    Kp4                = 324,
    Kp5                = 325,
    Kp6                = 326,
    Kp7                = 327,
    Kp8                = 328,
    Kp9                = 329,
    KpDecimal          = 330,
    KpDivide           = 331,
    KpMultiply         = 332,
    KpSubtract         = 333,
    KpAdd              = 334,
    KpEnter            = 335,
    KpEqual            = 336,
    LeftShift          = 340,
    LeftControl        = 341,
    LeftAlt            = 342,
    LeftSuper          = 343,
    RightShift         = 344,
    RightControl       = 345,
    RightAlt           = 346,
    RightSuper         = 347,
    Menu               = 348,
    
    Count              = 355,
};

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

enum class KeyStatus : uint8_t
{
    Press,
    Down,
    Release,
    Repeat,
    
    Count
};

enum class MouseButtonStatus : uint8_t
{
    Press,
    Down,
    Release,
    
    Count
};

enum class GamepadButtonStatus : uint8_t
{
    Press,
    Down,
    Release,
    
    Count
};

class Input
{
    STATIC_CLASS(Input)
    
public:
    XNOR_ENGINE static bool GetKey(Key key, KeyStatus status = KeyStatus::Down);
    
    XNOR_ENGINE static bool GetMouseButton(MouseButton mouseButton, MouseButtonStatus status = MouseButtonStatus::Down);
    
    XNOR_ENGINE static bool GetGamePadButton(uint32_t gamePadId, GamepadButton gamepadButton, GamepadButtonStatus buttonStatus);

    XNOR_ENGINE static void HandleEvent();
    
    template<class T>
    static T GetCursorPos();
    
    XNOR_ENGINE static void Initialize();

    XNOR_ENGINE static void Reset();

private:
    using KeyStatuses = std::array<bool, static_cast<uint8_t>(KeyStatus::Count)>;
    using MouseStatuses = std::array<bool, static_cast<uint8_t>(MouseButtonStatus::Count)>;

    XNOR_ENGINE static constexpr uint32_t GamePadMax = 15;
    
    XNOR_ENGINE static inline std::array<KeyStatuses, static_cast<uint16_t>(Key::Count) - 1> m_Keyboard;

    XNOR_ENGINE static inline std::array<MouseStatuses, static_cast<uint8_t>(MouseButton::Count) - 1> m_Mouse;
    
    XNOR_ENGINE static inline std::array<GamepadInput,GamePadMax> m_GamePads;

    // For each Status
    using GameButtonStatuses = std::array<bool, static_cast<uint8_t>(GamepadButtonStatus::Count)>;
    // For each Button
    using GamePadsButton = std::array<GameButtonStatuses, static_cast<uint32_t>(GamepadButton::Count)>;
    // For each Gamepad
    XNOR_ENGINE static inline std::array<GamePadsButton, GamePadMax> m_GamePadsButton;

    XNOR_ENGINE static void HandleKeyboard(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    
    XNOR_ENGINE static void HandleMouseButton(GLFWwindow* window, int32_t mouseButton, int32_t action, int32_t mods);
    
    XNOR_ENGINE static void MouseCursorPos(GLFWwindow* window, double_t xpos, double_t ypos);
    
    XNOR_ENGINE static void HandleJoyStickCallBack(int jid, int event);
    
    XNOR_ENGINE static void HandleGamePad();

    XNOR_ENGINE static void ResetKey();

    XNOR_ENGINE static void CheckGamePadAtLaunch();
    
    XNOR_ENGINE static inline Vector2 m_MousePos;
};

template <class T>
T Input::GetCursorPos()
{
    static_assert(std::is_same_v<T, Vector2> || std::is_same_v<T, Vector2i>, "Mouse pos must be a valid Vector2 type");
    
    return static_cast<T>(m_MousePos);
}

END_XNOR_CORE
