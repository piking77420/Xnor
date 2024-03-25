#pragma once

BEGIN_XNOR_CORE

template <Concepts::DurationT AwaitT>
template <Concepts::ConvertibleToT<AwaitT> From>
bool_t Coroutine<AwaitT>::Awaitable::await_ready(From&& from)
{
    return false;
}

template <Concepts::DurationT AwaitT>
bool_t Coroutine<AwaitT>::Awaitable::await_suspend(const std::coroutine_handle<> h)
{
    promise_type* promise = static_cast<promise_type*>(h.address());
}

template <Concepts::DurationT AwaitT>
void Coroutine<AwaitT>::Awaitable::await_resume()
{
}

template <Concepts::DurationT AwaitT>
Coroutine<AwaitT> Coroutine<AwaitT>::promise_type::get_return_object()
{
    return Coroutine(HandleType::from_promise(*this));
}

template <Concepts::DurationT AwaitT>
std::suspend_always Coroutine<AwaitT>::promise_type::initial_suspend()
{
    return {};
}

template <Concepts::DurationT AwaitT>
std::suspend_always Coroutine<AwaitT>::promise_type::final_suspend() noexcept
{
    return {};
}

template <Concepts::DurationT AwaitT>
void Coroutine<AwaitT>::promise_type::unhandled_exception()
{
    exception = std::current_exception();
}

template <Concepts::DurationT AwaitT>
void Coroutine<AwaitT>::promise_type::return_void()
{
}

template <Concepts::DurationT AwaitT>
template <Concepts::DurationT OtherDurationT>
typename Coroutine<AwaitT>::Awaitable Coroutine<AwaitT>::promise_type::await_transform(OtherDurationT&& duration)
{
}

template <Concepts::DurationT AwaitT>
void Coroutine<AwaitT>::promise_type::return_value()
{
}

template <Concepts::DurationT AwaitT>
Coroutine<AwaitT>::Coroutine(HandleType handle)
    : m_Handle(handle)
{
}

template <Concepts::DurationT AwaitT>
Coroutine<AwaitT>::~Coroutine()
{
    m_Handle.destroy();
}

template <Concepts::DurationT AwaitT>
bool_t Coroutine<AwaitT>::Resume()
{
    m_Handle.resume();
    return true;
}

END_XNOR_CORE
