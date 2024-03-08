#include "input/input.hpp"

#include "assimp/Logger.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

bool Input::GetKey(const Key key, const KeyStatus status)
{
    return m_Keyboard.at(static_cast<uint16_t>(key)).at(static_cast<uint16_t>(status));
}

bool Input::GetMouseButton(const MouseButton mouseButton, const MouseButtonStatus status)
{
    return m_Mouse.at(static_cast<uint8_t>(mouseButton)).at(static_cast<uint8_t>(status));
}

bool Input::GetGamepadButton(const uint32_t gamePadId, const GamepadButton gamepadButton, const GamepadButtonStatus buttonStatus)
{
    if (gamePadId > GamepadMax)
    {
        Logger::LogFatal("Gamepad Id none valid  = {}",gamePadId);
        throw std::exception("Gamepad Id none valid");
    }

    return m_GamepadsButton.at(gamePadId).at(static_cast<uint32_t>(gamepadButton)).at(static_cast<uint32_t>(buttonStatus));
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
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Down)) = false;
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Release)) = true;
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Repeat)) = false;
            break;
        
        case GLFW_PRESS:
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Pressed)) = true;
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Down)) = true;
            break;
        
        case GLFW_REPEAT:
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Repeat)) = true;
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
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Down)) = false;
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Release)) = true;
            break;

        case GLFW_PRESS:
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Pressed)) = true;
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Down)) = true;
            break;

        default:
            break;
    }
}

void Input::MouseCursorPos(GLFWwindow*, const double_t xpos, const double_t ypos)
{
    m_MousePosition = Vector2(static_cast<float_t>(xpos), static_cast<float_t>(ypos));
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
            const bool_t isDown = statuses.at(static_cast<uint8_t>(GamepadButtonStatus::Down));
            const bool_t isPress = statuses.at(static_cast<uint8_t>(GamepadButtonStatus::Pressed));
            
            switch (state.buttons[k])
            {
                case GLFW_RELEASE:
                    if (isDown || isPress)
                    {
                        statuses.at(static_cast<uint8_t>(GamepadButtonStatus::Release)) = true;
                        statuses.at(static_cast<uint8_t>(GamepadButtonStatus::Down)) = false;
                        statuses.at(static_cast<uint8_t>(GamepadButtonStatus::Pressed)) = false;
                    }
                    break;
                
                case GLFW_PRESS:
                    statuses.at(static_cast<uint8_t>(GamepadButtonStatus::Release)) = false;
                    statuses.at(static_cast<uint8_t>(GamepadButtonStatus::Down)) = true;
                    statuses.at(static_cast<uint8_t>(GamepadButtonStatus::Pressed)) = true;
                    
                    break;
                
                default:
                    break;
            }
        }
    }
}

void Input::ResetKey()
{
    GLFWwindow* const windowPtr = Window::GetHandle();

    glfwSetKeyCallback(windowPtr, HandleKeyboard);
    glfwSetMouseButtonCallback(windowPtr, HandleMouseButton);
    glfwSetCursorPosCallback(windowPtr, MouseCursorPos);

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
    for (auto& button : m_Mouse)
    {
        button.at(static_cast<uint8_t>(MouseButtonStatus::Pressed)) = false;
        button.at(static_cast<uint8_t>(MouseButtonStatus::Release)) = false;
    }

    for (auto& key : m_Keyboard)
    {
        key.at(static_cast<uint16_t>(KeyStatus::Pressed)) = false;
        key.at(static_cast<uint16_t>(KeyStatus::Release)) = false;
        key.at(static_cast<uint8_t>(KeyStatus::Repeat)) = false;
    }

    for (auto& gamepad : m_GamepadsButton)
    {
        for (auto& button : gamepad)
        {
            button.at(static_cast<uint8_t>(GamepadButtonStatus::Pressed)) = false;
            button.at(static_cast<uint8_t>(GamepadButtonStatus::Release)) = false;
        }
    }

    m_MouseDelta = m_MousePosition - m_LastMousePosition;

    if (mouseLocked)
        m_MousePosition -= m_MouseDelta;
}

void Input::Initialize()
{
    GLFWwindow* const windowPtr = Window::GetHandle();

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
