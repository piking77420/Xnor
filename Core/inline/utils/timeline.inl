#pragma once

#include "input/time.hpp"
#include "utils/logger.hpp"

BEGIN_XNOR_CORE

template <typename... Args>
Timeline<Args...>::Timeline(const float_t duration)
    : m_Duration(duration)
{
}

template <typename... Args>
void Timeline<Args...>::Start()
{
    m_Time = 0.f;
    m_LastEvent = m_Events.begin();
}

template <typename... Args>
bool_t Timeline<Args...>::Update(Args&&... args)
{
    m_Time += Time::GetDeltaTime<float_t>();

    if (m_Time >= m_Duration)
        return true;

    if (m_LastEvent == m_Events.end())
        return false;

    if (m_LastEvent->first <= m_Time)
    {
        m_LastEvent->second.Invoke(std::forward<Args>(args)...);

        ++m_LastEvent;
    }

    return false;
}

template <typename... Args>
void Timeline<Args...>::AddEvent(const float_t when, const FunctionT function)
{
    if (when >= m_Duration)
    {
        Logger::LogInfo("Can't add an event that after after the end of the timeline : {} >= {}", when, m_Duration);
        return;
    }

    m_Events[when] += function;
}

END_XNOR_CORE
