#include "utils/coroutine.hpp"

using namespace XnorCore;

bool_t Coroutine::Awaitable::await_ready(const AwaitType& from)
{
    return false;
}

bool_t Coroutine::Awaitable::await_suspend(const std::coroutine_handle<> h)
{
    promise_type* promise = static_cast<promise_type*>(h.address());
    return false;
}

void Coroutine::Awaitable::await_resume()
{
}

Coroutine Coroutine::promise_type::get_return_object()
{
    return Coroutine(HandleType::from_promise(*this));
}

std::suspend_always Coroutine::promise_type::initial_suspend()
{
    return {};
}

std::suspend_always Coroutine::promise_type::final_suspend() noexcept
{
    return {};
}

void Coroutine::promise_type::unhandled_exception()
{
    exception = std::current_exception();
}

void Coroutine::promise_type::return_void()
{
}

Coroutine::Awaitable Coroutine::promise_type::await_transform(const AwaitType& duration)
{
    return { duration };
}

void Coroutine::promise_type::return_value()
{
}

Coroutine::Coroutine(HandleType handle)
    : m_Handle(handle)
{
}

Coroutine::~Coroutine()
{
    m_Handle.destroy();
}

bool_t Coroutine::Resume()
{
    m_Handle.resume();
    return true;
}
