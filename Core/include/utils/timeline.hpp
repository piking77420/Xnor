#pragma once

#include "core.hpp"
#include "event.hpp"

BEGIN_XNOR_CORE

template <typename... Args>
class Timeline
{
    using FunctionT = typename Event<Args...>::StdFunctionT;
    
public:
    explicit Timeline(float_t duration);
    ~Timeline() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Timeline)
    
    void Start();
    bool_t Update(Args&&... args);

    void AddEvent(float_t when, FunctionT function);

private:
    std::map<float_t, Event<Args...>> m_Events;
    float_t m_Duration = 0.f;
    float_t m_Time = 0.f;

    typename decltype(m_Events)::iterator m_LastEvent;
};

END_XNOR_CORE

#include "utils/timeline.inl"
