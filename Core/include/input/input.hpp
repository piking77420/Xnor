#pragma once

#include <array>

#include "core.hpp"
#include "window.hpp"
#include "input/gamepad_input.hpp"
#include "input/keyboard_input.hpp"
#include "input/mouse_input.hpp"

/// @file input.hpp
/// @brief Defines the XnorCore::Input static class.

BEGIN_XNOR_CORE

/// @brief Used to fetch inputs from the current @ref Window.
class Input
{
    STATIC_CLASS(Input)
    
public:
    struct BindedWindowInfo
    {
        Vector2 windowPos;
        Vector2 windowSize;
        bool_t isEditor = false;
    };
    
    /// @brief Whether the mouse is locked
    XNOR_ENGINE static inline bool_t mouseLocked = false;
    
    /// @brief Checks if a specified key is of the specified status
    /// @param key Key to check
    /// @param status Key status
    /// @return Result
    XNOR_ENGINE static bool_t GetKey(Key::Key key, KeyStatus::KeyStatus status = KeyStatus::Down);

    /// @brief Checks if a specified mouse button is of the specified status
    /// @param mouseButton Mouse button to check
    /// @param status Mouse button status
    /// @return Result
    XNOR_ENGINE static bool_t GetMouseButton(MouseButton::MouseButton mouseButton, MouseButtonStatus::MouseButtonStatus status = MouseButtonStatus::Down);

    /// @brief Checks if a specified gamepad button is of the specified status
    /// @param gamePadId Gamepad id
    /// @param gamepadButton Gamepad button to check
    /// @param buttonStatus Gamepad button status
    /// @return Result
    XNOR_ENGINE static bool_t GetGamepadButton(uint32_t gamePadId, GamepadButton::GamepadButton gamepadButton, GamepadButtonStatus::GamepadButtonStatus buttonStatus);

    /// @brief Processes the input events
    XNOR_ENGINE static void HandleEvent();

    /// @brief Gets the mouse position
    /// @tparam T Vector2 type
    /// @return Mouse position
    template <class T>
    static T GetMousePosition();

    /// @brief Initializes the input manager
    XNOR_ENGINE static void Initialize();

    /// @brief Updates the input manager
    XNOR_ENGINE static void Update();

    // Retrun A array indexOf WindowInfo
    XNOR_ENGINE static uint32_t GetBindingId();

    XNOR_ENGINE static void UpdateBindedWindowInfo(uint32_t binding, BindedWindowInfo windowInfo);
    
    XNOR_ENGINE static void GetWindowBindedInfo(std::vector<BindedWindowInfo>* BindedWindowsInfo);
private:
    XNOR_ENGINE static inline std::vector<BindedWindowInfo> m_BindedWindowInfo;

    
    using KeyStatuses = std::array<bool_t, KeyStatus::Count>;
    using MouseStatuses = std::array<bool_t, MouseButtonStatus::Count>;

    static constexpr uint32_t GamepadMax = 15;
    
    XNOR_ENGINE static inline std::array<KeyStatuses, Key::Count - 1> m_Keyboard;

    XNOR_ENGINE static inline std::array<MouseStatuses, MouseButton::Count - 1> m_Mouse;
    
    XNOR_ENGINE static inline std::array<GamepadInput, GamepadMax> m_Gamepads;

    // For each Status
    using GamepadButtonStatuses = std::array<bool_t, static_cast<uint8_t>(GamepadButtonStatus::Count)>;
    // For each Button
    using GamepadButtons = std::array<GamepadButtonStatuses, static_cast<uint32_t>(GamepadButton::Count)>;
    // For each Gamepad
    XNOR_ENGINE static inline std::array<GamepadButtons, GamepadMax> m_GamepadsButton;
    
    XNOR_ENGINE static inline Vector2 m_LastMousePosition;
    
    XNOR_ENGINE static inline Vector2 m_MousePosition;
    
    XNOR_ENGINE static inline Vector2 m_MouseDelta;

    XNOR_ENGINE static inline GLFWwindow* m_WindowHandle = nullptr;

    static void HandleKeyboard(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
    
    static void HandleMouseButton(GLFWwindow* window, int32_t mouseButton, int32_t action, int32_t mods);
    
    static void MouseCursorPos(GLFWwindow* window, double_t xpos, double_t ypos);
    
    static void HandleJoyStickCallBack(int32_t jid, int32_t event);
    
    static void HandleGamepad();

    static void ResetKey();

    static void CheckGamepadAtLaunch();
};

END_XNOR_CORE

#include "input/input.inl"
