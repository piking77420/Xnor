#pragma once

#include <array>

#include "core.hpp"
#include "window.hpp"
#include "input/gamepad_input.hpp"
#include "input/keyboard_input.hpp"
#include "input/mouse_input.hpp"

/// @file input.hpp
/// @brief Defines the @ref Input static class.

BEGIN_XNOR_CORE

/// @brief Used to fetch inputs from the current @ref Window.
class Input
{
    STATIC_CLASS(Input)
    
public:
    /// @brief 
    XNOR_ENGINE static bool_t GetKey(Key key, KeyStatus status = KeyStatus::Down);
    
    XNOR_ENGINE static bool_t GetMouseButton(MouseButton mouseButton, MouseButtonStatus status = MouseButtonStatus::Down);
    
    XNOR_ENGINE static bool_t GetGamepadButton(uint32_t gamePadId, GamepadButton gamepadButton, GamepadButtonStatus buttonStatus);

    XNOR_ENGINE static void HandleEvent();
    
    template <class T>
    static T GetCursorPos();
    
    XNOR_ENGINE static void Initialize();

    XNOR_ENGINE static void Reset();

private:
    using KeyStatuses = std::array<bool_t, static_cast<uint8_t>(KeyStatus::Count)>;
    using MouseStatuses = std::array<bool_t, static_cast<uint8_t>(MouseButtonStatus::Count)>;

    XNOR_ENGINE static constexpr uint32_t GamepadMax = 15;
    
    XNOR_ENGINE static inline std::array<KeyStatuses, static_cast<uint16_t>(Key::Count) - 1> m_Keyboard;

    XNOR_ENGINE static inline std::array<MouseStatuses, static_cast<uint8_t>(MouseButton::Count) - 1> m_Mouse;
    
    XNOR_ENGINE static inline std::array<GamepadInput, GamepadMax> m_Gamepads;

    // For each Status
    using GameButtonStatuses = std::array<bool_t, static_cast<uint8_t>(GamepadButtonStatus::Count)>;
    // For each Button
    using GamePadsButton = std::array<GameButtonStatuses, static_cast<uint32_t>(GamepadButton::Count)>;
    // For each Gamepad
    XNOR_ENGINE static inline std::array<GamePadsButton, GamepadMax> m_GamepadsButton;

    XNOR_ENGINE static void HandleKeyboard(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    
    XNOR_ENGINE static void HandleMouseButton(GLFWwindow* window, int32_t mouseButton, int32_t action, int32_t mods);
    
    XNOR_ENGINE static void MouseCursorPos(GLFWwindow* window, double_t xpos, double_t ypos);
    
    XNOR_ENGINE static void HandleJoyStickCallBack(int32_t jid, int32_t event);
    
    XNOR_ENGINE static void HandleGamepad();

    XNOR_ENGINE static void ResetKey();

    XNOR_ENGINE static void CheckGamepadAtLaunch();
    
    XNOR_ENGINE static inline Vector2 m_MousePos;
};

template <class T>
T Input::GetCursorPos()
{
    static_assert(std::is_same_v<T, Vector2> || std::is_same_v<T, Vector2i>, "Mouse pos must be a valid Vector2 type");
    
    return static_cast<T>(m_MousePos);
}

END_XNOR_CORE
