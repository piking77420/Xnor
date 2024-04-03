#pragma once

#include <coroutine>
#include <exception>

#include "core.hpp"
#include "utils/concepts.hpp"

/// @file coroutine.hpp
/// @brief Defines the Coroutine class.

BEGIN_XNOR_CORE

// Everything named using the snake_case convention in this file is required by C++20 coroutines: https://en.cppreference.com/w/cpp/language/coroutines

/// @brief Wrapper around C++20 Coroutines.
/// @see <a href="https://en.cppreference.com/w/cpp/language/coroutines">C++20 Coroutines</a>
class Coroutine // TODO Add template for chrono seconds
{
    struct Awaitable;
    
public:
    /// @brief Duration type for Coroutine wait
    using AwaitType = std::chrono::milliseconds;

    /// @brief Promise type for C++20 coroutines.
    ///
    /// This is necessary for the coroutine to function and needs to be public but mustn't be used
    /// manually.
    struct promise_type
    {
        /// @brief The last @c co_await value
        AwaitType awaitValue;

        /// @brief Returns the object that will be returns to the caller of a CoroutineFunc
        XNOR_ENGINE Coroutine get_return_object();

        /// @brief Empty implementation
        XNOR_ENGINE std::suspend_always initial_suspend();

        /// @brief Empty implementation
        XNOR_ENGINE std::suspend_always final_suspend() noexcept;

        /// @brief Logs the exception and rethrows it
        XNOR_ENGINE void unhandled_exception();

        /// @brief Called when @c co_return is used in a Coroutine body
        XNOR_ENGINE void return_void();

        /// @brief Converts a AwaitType value to an Awaitable. Called when @c co_await is used with an AwaitType value.
        XNOR_ENGINE Awaitable await_transform(const AwaitType& duration);
    };
    
private:
    struct Awaitable
    {
        /// @brief Returns false if await_suspend should be called
        XNOR_ENGINE bool_t await_ready();

        /// @brief Effectively calls @c std::this_thread::sleep with promise_type::awaitValue
        XNOR_ENGINE bool_t await_suspend(std::coroutine_handle<promise_type> h);

        /// @brief Empty implementation
        XNOR_ENGINE void await_resume();
    };
    
public:
    /// @brief The coroutine handle type.
    using HandleType = std::coroutine_handle<promise_type>;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    /// @brief Constructs a new Coroutine from the given handle.
    ///
    /// This can be obtained as easily as this:
    /// @code
    /// Coroutine Function()
    /// {
    ///     using std::chrono_literals;
    ///     
    ///     // Do something
    ///     
    ///     co_await 750ms;
    ///
    ///     // Do something else
    /// }
    ///
    /// // Somewhere else
    /// Coroutine routine = Function();
    ///
    /// // Here you can do whatever you want with the Coroutine
    /// routine.Resume();
    /// @endcode
    XNOR_ENGINE Coroutine(HandleType handle);

    XNOR_ENGINE ~Coroutine();

    DEFAULT_COPY_MOVE_OPERATIONS(Coroutine)

    /// @brief Resumes the Coroutine.
    /// @return Whether the Coroutine, after being resumed, is back to the waiting state.
    XNOR_ENGINE bool_t Resume();

private:
    HandleType m_Handle; // TODO Use Pointer instead
};

/// @brief Coroutine function prototype.
template <typename... Args>
using CoroutineFunc = Coroutine(*)(Args...);

END_XNOR_CORE
