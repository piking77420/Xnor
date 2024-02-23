#include "input/core_input.hpp"

#include "assimp/Logger.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

bool CoreInput::GetKey(const Key key, const KeyStatus status)
{
    return m_Keyboard.at(static_cast<uint16_t>(key)).at(static_cast<uint16_t>(status));
}

bool CoreInput::GetMouseButton(const MouseButton mouseButton, const MouseButtonStatus status)
{
    return m_Mouse.at(static_cast<uint8_t>(mouseButton)).at(static_cast<uint8_t>(status));
}

void CoreInput::HandleKeyboard(GLFWwindow*, const int32_t key, const int32_t, const int32_t action, const int32_t)
{
    KeyStatuses& keyStatuses = m_Keyboard.at(key);
    
    switch (action)
    {
        case GLFW_RELEASE:
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Down)) = false;
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Release)) = true;
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Repeat)) = false;
            break;
        
        case GLFW_PRESS:
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Press)) = true;
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Down)) = true;
            break;
        
        case GLFW_REPEAT:
            keyStatuses.at(static_cast<uint8_t>(KeyStatus::Repeat)) = true;
            break;
        
        default:
            break;
    }
}

void CoreInput::HandleMouseButton(GLFWwindow*, const int32_t mouseButton, const int32_t action, int32_t)
{
    MouseStatuses& keyStatuses = m_Mouse.at(mouseButton);
    
    switch (action)
    {
        case GLFW_RELEASE:
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Down)) = false;
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Release)) = true;
            break;

        case GLFW_PRESS:
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Press)) = true;
            keyStatuses.at(static_cast<uint8_t>(MouseButtonStatus::Down)) = true;
            break;

        default:
            break;
    }
}

void CoreInput::MouseCursorPos(GLFWwindow*, const double_t xpos, const double_t ypos)
{
    m_MousePos = Vector2(static_cast<float_t>(xpos), static_cast<float_t>(ypos));
}

void CoreInput::HandleJoyStick(int jid, int event)
{
    switch (event)
    {
        
    }
}

void CoreInput::HandleGamePad()
{
    
    for (uint32_t i = 0; i < JoyStickMax; i++)
    {
        const uint32_t present = glfwJoystickPresent(static_cast<uint32_t>(i));
        m_GamePads[i] = static_cast<bool>(present);
    }
    
}

void CoreInput::Initialize()
{
    GLFWwindow* const windowPtr = static_cast<GLFWwindow*>(Window::GetWindow());

    glfwSetKeyCallback(windowPtr, HandleKeyboard);
    glfwSetMouseButtonCallback(windowPtr, HandleMouseButton);
    glfwSetCursorPosCallback(windowPtr, MouseCursorPos);
    glfwSetJoystickCallback(HandleJoyStick);

    KeyStatuses defaultKeys;
    defaultKeys.fill(false);
    m_Keyboard.fill(defaultKeys);

    MouseStatuses defaultMouseButtons;
    defaultMouseButtons.fill(false);
    m_Mouse.fill(defaultMouseButtons);
}

void CoreInput::Reset()
{
    HandleGamePad();
    
    for (uint8_t i = 0; i < static_cast<uint8_t>(MouseButton::Count) - 1; i++)
    {
        MouseStatuses& statuses = m_Mouse.at(i);
        statuses.at(static_cast<uint8_t>(MouseButtonStatus::Press)) = false;
        statuses.at(static_cast<uint8_t>(MouseButtonStatus::Release)) = false;
    }

    for (uint16_t i = 0; i < static_cast<uint16_t>(Key::Count) - 1; i++)
    {
        KeyStatuses& statuses = m_Keyboard.at(i);
        statuses.at(static_cast<uint16_t>(KeyStatus::Press)) = false;
        statuses.at(static_cast<uint16_t>(KeyStatus::Release)) = false;
        statuses.at(static_cast<uint8_t>(KeyStatus::Repeat)) = false;
    }
}

