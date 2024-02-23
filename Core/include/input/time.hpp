#pragma once

#include <cmath>

#include "core.hpp"

BEGIN_XNOR_CORE

class Time
{
    STATIC_CLASS(Time)

public:
    template<typename T = float>
    static T GetTotalTime();
    
    template<typename T = float>
    static T GetLastTotalTime();

    template<typename T = float>
    static T GetDeltaTime();

    template<typename T = uint32_t>
    static T GetTotalFrameCount();
    
    /// @brief Updates the time variables using GLFW.
    ///        This function should be called exactly once each frame.
    XNOR_ENGINE static void Update();

private:
    XNOR_ENGINE static inline double_t m_TotalTime = 0.0;
    XNOR_ENGINE static inline double_t m_LastTotalTime = 0.0;
    XNOR_ENGINE static inline double_t m_DeltaTime = 0.0;
    XNOR_ENGINE static inline uint64_t m_TotalFrameCount = 0;
};

template float_t Time::GetTotalTime<float_t>();
template double_t Time::GetTotalTime<double_t>();

template<typename T>
T Time::GetTotalTime()
{
    static_assert(std::is_floating_point_v<T>, "You can only get total time as a floating point type");
    
    return static_cast<T>(m_TotalTime);
}

template float_t Time::GetLastTotalTime<float_t>();
template double_t Time::GetLastTotalTime<double_t>();

template<typename T>
T Time::GetLastTotalTime()
{
    static_assert(std::is_floating_point_v<T>, "You can only get last total time as a floating point type");
    
    return static_cast<T>(m_LastTotalTime);
}

template float_t Time::GetDeltaTime<float_t>();
template double_t Time::GetDeltaTime<double_t>();

template<typename T>
T Time::GetDeltaTime()
{
    static_assert(std::is_floating_point_v<T>, "You can only get delta time as a floating point type");
    
    return static_cast<T>(m_DeltaTime);
}

template uint32_t Time::GetTotalFrameCount<uint32_t>();
template uint64_t Time::GetTotalFrameCount<uint64_t>();
template int32_t Time::GetTotalFrameCount<int32_t>();

template<typename T>
T Time::GetTotalFrameCount()
{
    static_assert(std::is_integral_v<T>, "You can only get total frame count as an integral type");
    
    return static_cast<T>(m_TotalFrameCount);
}

END_XNOR_CORE
