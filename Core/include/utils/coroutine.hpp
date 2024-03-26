#pragma once

#include <coroutine>
#include <exception>

#include "core.hpp"
#include "utils/concepts.hpp"

BEGIN_XNOR_CORE

/// @brief Wrapper around [C++20 Coroutines](https://en.cppreference.com/w/cpp/language/coroutines).
/// @tparam AwaitT 
template <Concepts::DurationT AwaitT = std::chrono::milliseconds>
class Coroutine
{
    struct Awaitable
    {
        template<Concepts::ConvertibleToT<AwaitT> From>
        bool_t await_ready(From&& from);

        bool_t await_suspend(std::coroutine_handle<> h);

        void await_resume();
    };
    
    // Required by C++20 coroutines: https://en.cppreference.com/w/cpp/language/coroutines
    struct promise_type
    {
        AwaitT awaitValue;
        
        std::exception_ptr exception;
 
        Coroutine get_return_object();

        std::suspend_always initial_suspend();

        std::suspend_always final_suspend() noexcept;

        void unhandled_exception();

        void return_void();

        template<Concepts::DurationT OtherDurationT>
        Awaitable await_transform(OtherDurationT&& duration);

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

#include "utils/coroutine.inl"
