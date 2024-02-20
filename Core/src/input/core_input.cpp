#include "input/core_input.hpp"

#include "assimp/Logger.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

bool CoreInput::GetKey(const KeyCode keyCode, const KeyStatus keyStatus)
{
    return std::ranges::any_of(m_InputKeyBoardQueue, [&](const InputKeyBoard& input) -> bool
    {
        return input.status == keyStatus && input.keyCode == keyCode;
    });
}

bool CoreInput::GetMouseButton(const MouseButton mouseButton, const KeyStatus keyStatus)
{
    return std::ranges::any_of(m_InputMouseButtonQueue, [&](const InputMouse& input) -> bool
    {
        return input.status == keyStatus && input.mouseButton == mouseButton;
    });
}

CoreInput::CoreInput()
{
    m_InputKeyBoardQueue.reserve(ReserveKeySize);
    m_InputMouseButtonQueue.reserve(ReserveKeySize);
}

void CoreInput::HandleKeyBoardButton([[maybe_unused]] GLFWwindow* window, const int32_t key, [[maybe_unused]] const int32_t scancode, const int32_t action, [[maybe_unused]] const int32_t mods)
{
    switch (action)
    {
        case GLFW_RELEASE:
            m_InputKeyBoardQueue.emplace_back(static_cast<KeyCode>(key), KeyStatus::RELEASE);
            break;
        
        case GLFW_PRESS:
            m_InputKeyBoardQueue.emplace_back(static_cast<KeyCode>(key), KeyStatus::DOWN);
            break;
        
        case GLFW_REPEAT:
            m_InputKeyBoardQueue.emplace_back(static_cast<KeyCode>(key), KeyStatus::REPEAT);
            break;
        
        default:
            break;
    }
}

void CoreInput::HandleMouseButton([[maybe_unused]] GLFWwindow* window, int32_t button, const int32_t action, [[maybe_unused]] int32_t mods)
{
    switch (action)
    {
        case GLFW_RELEASE:
            m_InputMouseButtonQueue.emplace_back(static_cast<MouseButton>(button), KeyStatus::RELEASE);
            break;

        case GLFW_PRESS:
            m_InputMouseButtonQueue.emplace_back(static_cast<MouseButton>(button), KeyStatus::DOWN);
            break;

        default:
            break;
    }
}

void CoreInput::MouseCursorPos([[maybe_unused]] GLFWwindow* window, const double_t xpos, const double_t ypos)
{
    m_MousePos = Vector2(static_cast<float_t>(xpos), static_cast<float_t>(ypos));
}

void CoreInput::ClearKey()
{
    m_InputKeyBoardQueue.clear();
}

void CoreInput::InitCallBacks(const Window& window)
{
    GLFWwindow* const windowPtr = const_cast<GLFWwindow*>(static_cast<const GLFWwindow*>(window.GetWindow()));

    glfwSetKeyCallback(windowPtr,CoreInput::HandleKeyBoardButton);
    glfwSetMouseButtonCallback(windowPtr,CoreInput::HandleMouseButton);
    glfwSetCursorPosCallback(windowPtr,CoreInput::MouseCursorPos);
}

