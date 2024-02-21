#pragma once

#include <cmath>

#include <GLFW/glfw3.h>

#include "core.hpp"

BEGIN_XNOR_CORE

class Time
{
    STATIC_CLASS(Time)
    
public:
    template<typename T = float>
    static T GetTime();

    template<typename T = float>
    static T GetDeltaTime();

    XNOR_ENGINE static void Update();
    
private:
    XNOR_ENGINE static inline double_t m_TotalTime = 0.0;
    XNOR_ENGINE static inline double_t m_LastTotalTime = 0.f;
    XNOR_ENGINE static inline double_t m_DeltaTime = 0.f;
};

template float_t Time::GetTime<float_t>();
template double_t Time::GetTime<double_t>();

template<typename T>
T Time::GetTime()
{
    static_assert(std::is_floating_point_v<T>, "You can only get time as a floating point type");
    
    return static_cast<T>(m_TotalTime);
}

template float_t Time::GetDeltaTime<float_t>();
template double_t Time::GetDeltaTime<double_t>();

template<typename T>
T Time::GetDeltaTime()
{
    static_assert(std::is_floating_point_v<T>, "You can only get time as a floating point type");
    
    return static_cast<T>(m_DeltaTime);
}

END_XNOR_CORE
