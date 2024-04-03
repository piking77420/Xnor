// ReSharper disable CppMemberFunctionMayBeStatic
#include "utils/coroutine.hpp"

#include "utils/logger.hpp"

using namespace XnorCore;

bool_t Coroutine::Awaitable::await_ready() { return false; }

bool_t Coroutine::Awaitable::await_suspend(std::coroutine_handle<promise_type> h)
{
    std::this_thread::sleep_for(h.promise().awaitValue);
    return true;
}

void Coroutine::Awaitable::await_resume() {}

Coroutine Coroutine::promise_type::get_return_object() { return Coroutine(HandleType::from_promise(*this)); }

std::suspend_always Coroutine::promise_type::initial_suspend() { return {}; }

std::suspend_always Coroutine::promise_type::final_suspend() noexcept { return {}; }

void Coroutine::promise_type::unhandled_exception()
{
    try
    {
        std::rethrow_exception(std::current_exception());
    }
    catch (const std::exception& e)
    {
        Logger::LogError("Uncaught exception in coroutine body: {}", e);
        throw;
    }
}

void Coroutine::promise_type::return_void() {}

Coroutine::Awaitable Coroutine::promise_type::await_transform(const AwaitType& duration)
{
    awaitValue = duration;
    return {};
}

Coroutine::Coroutine(const HandleType handle) : m_Handle(handle) {}

Coroutine::~Coroutine() { m_Handle.destroy(); }

// ReSharper disable once CppMemberFunctionMayBeConst
bool_t Coroutine::Resume()
{
    m_Handle.promise().awaitValue = AwaitType::zero();
    m_Handle.resume();

    return m_Handle.promise().awaitValue != AwaitType::zero();
}
