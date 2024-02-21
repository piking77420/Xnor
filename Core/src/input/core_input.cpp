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

void CoreInput::HandleKeyboard([[maybe_unused]] GLFWwindow* window, const int32_t key, [[maybe_unused]] const int32_t scancode, const int32_t action, [[maybe_unused]] const int32_t mods)
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

void CoreInput::HandleMouseButton([[maybe_unused]] GLFWwindow* window, const int32_t mouseButton, const int32_t action, [[maybe_unused]] int32_t mods)
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

void CoreInput::MouseCursorPos([[maybe_unused]] GLFWwindow* window, const double_t xpos, const double_t ypos)
{
    m_MousePos = Vector2(static_cast<float_t>(xpos), static_cast<float_t>(ypos));
}

void CoreInput::Initialize(const Window& window)
{
    GLFWwindow* const windowPtr = const_cast<GLFWwindow*>(static_cast<const GLFWwindow*>(window.GetWindow()));

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

void CoreInput::Reset()
{
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

