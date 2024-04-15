#pragma once

#include "core.hpp"

#include <functional>

#include "list.hpp"
#include "reflection/reflection.hpp"

/// @file event.hpp
/// @brief Defines the Event class.

BEGIN_XNOR_CORE

/// @brief Encapsulates a delegate system similar to its C# implementation
///
/// It works by having an underlying list of functions that always return @c void and have the provided types as arguments
/// The += and -= operators can be used to respectively add and remove events to the list
/// @tparam Args Event function arguments
template <typename... Args>
class Event
{
    REFLECTABLE_IMPL(Event)
    
public:
    /// @brief Signature of the event function
    using FunctionT = void(Args...);
    using StdFunctionT = std::function<FunctionT>;

    /// @brief Invokes the currently registered events with the provided parameters
    /// @param args Parameters
    void Invoke(Args... args) const;

    /// @brief Clears the event list
    void Clear();

    /// @brief Adds a function to the event list
    /// @param func Function
    /// @return This
    Event& operator+=(StdFunctionT func);

    /// @brief Removes a function from the event list
    /// @param func Function
    /// @return This
    Event& operator-=(const StdFunctionT& func);

private:
    List<StdFunctionT> m_Functions;
};

END_XNOR_CORE

REFL_AUTO(template((typename... Args), (XnorCore::Event<Args...>)),
    field(m_Functions)
)

#include "utils/event.inl"
