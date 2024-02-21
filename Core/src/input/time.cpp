#include "input/time.hpp"

#include <GLFW/glfw3.h>

using namespace XnorCore;

double Time::GetTimeDouble()
{
    return glfwGetTime();
}

float Time::GetTime()
{
    return static_cast<float>(glfwGetTime());
}

void Time::ComputeDeltaTime()
{
    float m_CurrentFrame = static_cast<float>(glfwGetTime());
    m_DeltaTime = m_CurrentFrame - m_LastFrame;
    m_LastFrame = m_CurrentFrame;
}

float_t Time::GetDeltaTime()
{
    return m_DeltaTime;
}
