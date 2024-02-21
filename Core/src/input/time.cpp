#include "input/time.hpp"

#include <GLFW/glfw3.h>

using namespace XnorCore;

void Time::Update()
{
    m_TotalTime = glfwGetTime();
    m_DeltaTime = m_TotalTime - m_LastTotalTime;
    m_LastTotalTime = m_TotalTime;
}

float_t Time::GetDeltaTime()
{
    return m_DeltaTime;
}
