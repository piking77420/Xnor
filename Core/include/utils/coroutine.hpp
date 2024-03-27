#pragma once

#include <coroutine>
#include <exception>

#include "core.hpp"
#include "utils/concepts.hpp"

BEGIN_XNOR_CORE

// Everything named using the snake_case convention in this file is required by C++20 coroutines: https://en.cppreference.com/w/cpp/language/coroutines

/// @brief Wrapper around C++20 Coroutines.
/// @see <a href="https://en.cppreference.com/w/cpp/language/coroutines">C++20 Coroutines</a>
class Coroutine
{
public:
    using AwaitType = std::chrono::milliseconds;
    
private:
    struct Awaitable
    {
        AwaitType duration;
        
        bool_t await_ready(const AwaitType& from);

        bool_t await_suspend(std::coroutine_handle<> h);

        void await_resume();
    };
    
    struct promise_type
    {
        AwaitType awaitValue;
        
        std::exception_ptr exception;
 
        Coroutine get_return_object();

        std::suspend_always initial_suspend();

        std::suspend_always final_suspend() noexcept;

        void unhandled_exception();

        void return_void();

        Awaitable await_transform(const AwaitType& duration);

        void return_value();
    };
    
public:
    using HandleType = std::coroutine_handle<promise_type>;

    Coroutine(HandleType handle);

    ~Coroutine();

    DEFAULT_COPY_MOVE_OPERATIONS(Coroutine)

    bool_t Resume();

private:
    HandleType m_Handle;
};

END_XNOR_CORE
