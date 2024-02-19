#include "input/core_input.hpp"

using namespace XnorCore;


bool CoreInput::GetKey(KeyCode keyCode, KeyStatus keyStatus)
{
    for (size_t i = 0 ; i < m_InputQueue->size() ; i++)
    {
        if (m_InputQueue->at(i).status == keyStatus && m_InputQueue->at(i).keyCode == keyCode)
            return true;
    }
    
    return false;
}

CoreInput::CoreInput()
{
    m_InputQueue = new std::vector<InputKey>();
}

CoreInput::~CoreInput()
{
    delete m_InputQueue;
}

void CoreInput::HandleInputsEvents(const Window& window)
{
    int32_t value;
    GLFWwindow* windowPtr = const_cast<GLFWwindow*>(reinterpret_cast<const GLFWwindow*>(window.GetWindow()));

    for (int i = 0 ; i < static_cast<int>(KeyCode::KEY_SIZE); i++)
    {
        value = glfwGetKey(windowPtr,i);
        if (value)
        {
            switch (value)
            {
            case GLFW_RELEASE:
                m_InputQueue->push_back({static_cast<KeyCode>(i),KeyStatus::RELEASE});
                break;
            case GLFW_PRESS:
                m_InputQueue->push_back({static_cast<KeyCode>(i),KeyStatus::DOWN});
                break;
            case GLFW_REPEAT:
                m_InputQueue->push_back({static_cast<KeyCode>(i),KeyStatus::REPEAT});
                break;
            default:
                break;
            }
        }
    }

    
}

void CoreInput::ClearEvents()
{
    m_InputQueue->clear();
}
