#include "input/core_input.hpp"

#include "assimp/Logger.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;


bool CoreInput::GetKey(KeyCode keyCode, KeyStatus keyStatus)
{
    size_t size = m_InputKeyBoardQueue->size();
    
    for (size_t i = 0 ; i < m_InputKeyBoardQueue->size() ; i++)
    {
        if (m_InputKeyBoardQueue->at(i).status == keyStatus && m_InputKeyBoardQueue->at(i).keyCode == keyCode)
            return true;
    }
    
    return false;
}

bool CoreInput::GetMouseButton(MouseButton mouseButton, KeyStatus keyStatus)
{
    
    for (size_t i = 0 ; i < m_InputMouseButtonQueue->size() ; i++)
    {
        if (m_InputMouseButtonQueue->at(i).status == keyStatus && m_InputMouseButtonQueue->at(i).mouseButton == mouseButton)
            return true;
    }
    
    return false;
}

CoreInput::CoreInput()
{
    m_InputKeyBoardQueue = new std::vector<InputKeyBoard>();
    m_InputKeyBoardQueue->reserve(ReserveKeySize);
    m_InputMouseButtonQueue = new  std::vector<InputMouse>();
    m_InputKeyBoardQueue->reserve(ReserveKeySize);

}

CoreInput::~CoreInput()
{
    delete m_InputKeyBoardQueue;
    delete m_InputMouseButtonQueue;
}

void CoreInput::HandleKeyBoardButton(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action)
    {
    case GLFW_RELEASE:
        m_InputKeyBoardQueue->emplace_back(static_cast<KeyCode>(key),KeyStatus::RELEASE);
        break;
    case GLFW_PRESS:
        m_InputKeyBoardQueue->emplace_back(static_cast<KeyCode>(key),KeyStatus::DOWN);
        break;
    case GLFW_REPEAT:
        m_InputKeyBoardQueue->emplace_back(static_cast<KeyCode>(key),KeyStatus::REPEAT);
        break;
    default:
        break;
    }
}

void CoreInput::HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    switch (action)
    {
    case GLFW_RELEASE:
        m_InputMouseButtonQueue->emplace_back(static_cast<MouseButton>(button),KeyStatus::RELEASE);
        break;
    case GLFW_PRESS:
        m_InputMouseButtonQueue->emplace_back(static_cast<MouseButton>(button),KeyStatus::DOWN);
        break;
    default:
        break;
    }
 
}

void CoreInput::MouseCursorPos(GLFWwindow* window, double xpos, double ypos)
{
    m_MousePos = {static_cast<float>(xpos),static_cast<float>(ypos)};
}

void CoreInput::ClearKey()
{
    m_InputKeyBoardQueue->clear();
}

void CoreInput::InitCallBacks(const Window& window)
{
    GLFWwindow* windowPtr = const_cast<GLFWwindow*>(reinterpret_cast<const GLFWwindow*>(window.GetWindow()));
    glfwSetKeyCallback(windowPtr,CoreInput::HandleKeyBoardButton);
    glfwSetMouseButtonCallback(windowPtr,CoreInput::HandleMouseButton);
    glfwSetCursorPosCallback(windowPtr,CoreInput::MouseCursorPos);
    
}

