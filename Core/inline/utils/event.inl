#pragma once

#include "utils/utils.hpp"

BEGIN_XNOR_CORE

template <typename... Args>
void Event<Args...>::Invoke(Args... args) const
{
    for (const std::function<FunctionT>& function : m_Functions)
        function(std::forward<Args>(args)...);
}

template <typename... Args>
void Event<Args...>::Clear()
{
    m_Functions.clear();
}

template <typename... Args>
Event<Args...>& Event<Args...>::operator+=(std::function<FunctionT> func)
{
    m_Functions.push_back(std::move(func));

    return *this;
}

template <typename ... Args>
Event<Args...>& Event<Args...>::operator-=(const std::function<FunctionT>& func)
{
    for (typename decltype(m_Functions)::const_iterator it = m_Functions.begin(); it != m_Functions.end(); it++)
    {
        if (Utils::FunctionAddress(*it) == Utils::FunctionAddress(func))
        {
            m_Functions.erase(it);
            break;
        }
    }

    return *this;
}

END_XNOR_CORE
