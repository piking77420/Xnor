#include "input/input.hpp"

#include <GLFW/glfw3.h>

#include "utils/logger.hpp"
#include "utils/windows.hpp"

using namespace XnorCore;

bool Input::GetKey(const Key::Key key, const KeyStatus::KeyStatus status)
{
    return m_Keyboard.at(key).at(status);
}

bool Input::GetMouseButton(const MouseButton::MouseButton mouseButton, const MouseButtonStatus::MouseButtonStatus status)
{
    return m_Mouse.at(mouseButton).at(status);
}

bool Input::GetGamepadButton(const uint32_t gamePadId, const GamepadButton::GamepadButton gamepadButton, const GamepadButtonStatus::GamepadButtonStatus buttonStatus)
{
    if (gamePadId > GamepadMax)
    {
        Logger::LogFatal("Invalid gamepad ID {}", gamePadId);
        throw std::invalid_argument("Invalid gamepad ID");
    }

    return m_GamepadsButton.at(gamePadId).at(gamepadButton).at(buttonStatus);
}

void Input::HandleEvent()
{
    HandleGamepad();
}

void Input::HandleKeyboard(GLFWwindow*, const int32_t key, const int32_t, const int32_t action, const int32_t)
{
    if (static_cast<size_t>(key) > m_Keyboard.size())
        return;
    
    KeyStatuses& keyStatuses = m_Keyboard.at(key);
    
    switch (action)
    {
        case GLFW_RELEASE:
            keyStatuses.at(KeyStatus::Down) = false;
            keyStatuses.at(KeyStatus::Release) = true;
            keyStatuses.at(KeyStatus::Repeat) = false;
            break;
        
        case GLFW_PRESS:
            keyStatuses.at(KeyStatus::Pressed) = true;
            keyStatuses.at(KeyStatus::Down) = true;
            break;
        
        case GLFW_REPEAT:
            keyStatuses.at(KeyStatus::Repeat) = true;
            break;
        
        default:
            break;
    }
}

void Input::HandleMouseButton(GLFWwindow*, const int32_t mouseButton, const int32_t action, int32_t)
{
    MouseStatuses& keyStatuses = m_Mouse.at(mouseButton);
    
    switch (action)
    {
        case GLFW_RELEASE:
            keyStatuses.at(MouseButtonStatus::Down) = false;
            keyStatuses.at(MouseButtonStatus::Release) = true;
            break;

        case GLFW_PRESS:
            keyStatuses.at(MouseButtonStatus::Pressed) = true;
            keyStatuses.at(MouseButtonStatus::Down) = true;
            break;

        default:
            break;
    }
}

void Input::MouseCursorPos(GLFWwindow*, const double_t xpos, const double_t ypos)
{
    //m_MousePosition = Vector2(static_cast<float_t>(xpos), static_cast<float_t>(ypos));
}

void Input::HandleJoyStickCallBack(const int32_t jid, const int32_t event)
{
    switch (event)
    {
        case GLFW_CONNECTED:
            m_Gamepads[jid].isConnected = true;
            break;

        case GLFW_DISCONNECTED:
            m_Gamepads[jid].isConnected = false;
            break;
        
        default:
            break;
    }
}
void Input::HandleGamepad()
{
    for (uint32_t i = 0; i < GamepadMax ; i++)
    {
        if (!m_Gamepads[i].isConnected)
            continue;

        GLFWgamepadstate state;

        if (!glfwGetGamepadState(static_cast<int32_t>(i), &state))
            return;
        
        for (uint32_t k = 0; k < static_cast<uint32_t>(GamepadAxis::Count); k++)
        {
            if (state.axes[k] <= GamepadInput::NullAnalogValue)
                continue;

            m_Gamepads[i].axisValues[k] = state.axes[k];
        }

        for (uint32_t k = 0; k < static_cast<uint32_t>(GamepadButton::Count) - 1; k++)
        {
            GamepadButtonStatuses& statuses = m_GamepadsButton[i].at(k);
            const bool_t isDown = statuses.at(GamepadButtonStatus::Down);
            const bool_t isPress = statuses.at(GamepadButtonStatus::Pressed);
            
            switch (state.buttons[k])
            {
                case GLFW_RELEASE:
                    if (isDown || isPress)
                    {
                        statuses.at(GamepadButtonStatus::Release) = true;
                        statuses.at(GamepadButtonStatus::Down) = false;
                        statuses.at(GamepadButtonStatus::Pressed) = false;
                    }
                    break;
                
                case GLFW_PRESS:
                    statuses.at(GamepadButtonStatus::Release) = false;
                    statuses.at(GamepadButtonStatus::Down) = true;
                    statuses.at(GamepadButtonStatus::Pressed) = true;
                    
                    break;
                
                default:
                    break;
            }
        }
    }
}

void Input::ResetKey()
{
    glfwSetKeyCallback(m_WindowHandle, HandleKeyboard);
    glfwSetMouseButtonCallback(m_WindowHandle, HandleMouseButton);
    //glfwSetCursorPosCallback(m_WindowHandle, MouseCursorPos);


    KeyStatuses defaultKeys;
    defaultKeys.fill(false);
    m_Keyboard.fill(defaultKeys);

    MouseStatuses defaultMouseButtons;
    defaultMouseButtons.fill(false);
    m_Mouse.fill(defaultMouseButtons);
}

void Input::CheckGamepadAtLaunch()
{
    for (int32_t i = 0; i < static_cast<int32_t>(GamepadMax); i++)
    {
        const int32_t present = glfwJoystickPresent(i);
        m_Gamepads.at(i).isConnected = static_cast<bool_t>(present);
    }
}

void Input::Update()
{
    POINT point;
    GetCursorPos(&point);
    m_MousePosition.x = static_cast<float_t>(point.x);
    m_MousePosition.y = static_cast<float_t>(point.y);
    
    for (auto& button : m_Mouse)
    {
        button.at(MouseButtonStatus::Pressed) = false;
        button.at(MouseButtonStatus::Release) = false;
    }

    for (auto& key : m_Keyboard)
    {
        key.at(KeyStatus::Pressed) = false;
        key.at(KeyStatus::Release) = false;
        key.at(KeyStatus::Repeat) = false;
    }

    for (auto& gamepad : m_GamepadsButton)
    {
        for (auto& button : gamepad)
        {
            button.at(GamepadButtonStatus::Pressed) = false;
            button.at(GamepadButtonStatus::Release) = false;
        }
    }

    m_MouseDelta = m_MousePosition - m_LastMousePosition;

    if (mouseLocked)
        m_MousePosition -= m_MouseDelta;

}

uint32_t Input::GetBindingId()
{
    size_t index = m_BindedWindowInfo.size();
    m_BindedWindowInfo.resize(index + 1);
    return static_cast<uint32_t>(index);
}

void Input::UpdateBindedWindowInfo(uint32_t binding, BindedWindowInfo windowInfo)
{
    m_BindedWindowInfo[static_cast<size_t>(binding)] = windowInfo;
}

void Input::GetWindowBindedInfo(std::vector<BindedWindowInfo>* BindedWindowsInfo)
{
    *BindedWindowsInfo = m_BindedWindowInfo;
}

void Input::Initialize()
{
    GLFWwindow* const windowPtr = glfwGetCurrentContext();

    glfwSetKeyCallback(windowPtr, HandleKeyboard);
    glfwSetMouseButtonCallback(windowPtr, HandleMouseButton);
    glfwSetCursorPosCallback(windowPtr, MouseCursorPos);
    glfwSetJoystickCallback(HandleJoyStickCallBack);

    KeyStatuses defaultKeys;
    defaultKeys.fill(false);
    m_Keyboard.fill(defaultKeys);

    MouseStatuses defaultMouseButtons;
    defaultMouseButtons.fill(false);
    m_Mouse.fill(defaultMouseButtons);

    CheckGamepadAtLaunch();
}
