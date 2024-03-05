#pragma once

#include <cmath>

#include "utils/meta_programming.hpp"
#include "core.hpp"

/// @file time.hpp
/// @brief Defines the static class Time

BEGIN_XNOR_CORE

/// @brief Defines utility functions to access information regarding time
class Time
{
    STATIC_CLASS(Time)

public:
    /// @brief Gets the total elapsed time
    /// @tparam T Result type
    /// @return Total time
    template <typename T = float_t>
    static T GetTotalTime();

    /// @brief Gets the last total elapsed time
    /// @tparam T Result type
    /// @return Last total time
    template <typename T = float_t>
    static T GetLastTotalTime();
    
    /// @brief Gets the delta time
    /// @tparam T Result type
    /// @return Delta time
    template <typename T = float_t>
    static T GetDeltaTime();

    /// @brief Gets the total frame count
    /// @tparam T Result type
    /// @return Total frame count
    template <typename T = uint32_t>
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

template <typename T>
T Time::GetTotalTime()
{
    static_assert(Meta::IsFloatingPoint<T>, "You can only get total time as a floating point type");
    
    return static_cast<T>(m_TotalTime);
}

template float_t Time::GetLastTotalTime<float_t>();
template double_t Time::GetLastTotalTime<double_t>();

template <typename T>
T Time::GetLastTotalTime()
{
    static_assert(Meta::IsFloatingPoint<T>, "You can only get last total time as a floating point type");
    
    return static_cast<T>(m_LastTotalTime);
}

template float_t Time::GetDeltaTime<float_t>();
template double_t Time::GetDeltaTime<double_t>();

template <typename T>
T Time::GetDeltaTime()
{
    static_assert(Meta::IsFloatingPoint<T>, "You can only get delta time as a floating point type");
    
    return static_cast<T>(m_DeltaTime);
}

template uint32_t Time::GetTotalFrameCount<uint32_t>();
template uint64_t Time::GetTotalFrameCount<uint64_t>();
template int32_t Time::GetTotalFrameCount<int32_t>();

template <typename T>
T Time::GetTotalFrameCount()
{
    static_assert(Meta::IsIntegral<T>, "You can only get total frame count as an integral type");
    
    return static_cast<T>(m_TotalFrameCount);
}

END_XNOR_CORE
