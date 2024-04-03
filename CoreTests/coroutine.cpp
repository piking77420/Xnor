#include "pch.hpp"

#include "utils/logger.hpp"

Coroutine WaitRoutine()
{
    using namespace std::chrono_literals;

    DEBUG_LOG("First Coroutine log");
    
    co_await 1500ms;

    DEBUG_LOG("Second Coroutine log");
    
    co_await 1500ms;

    DEBUG_LOG("Third Coroutine log");

    co_return;
}

TEST(Coroutine, GeneralTests)
{
    Coroutine c = WaitRoutine();
    
    DEBUG_LOG("First outside log");
    
    c.Resume();
    
    DEBUG_LOG("Second outside log");
    
    c.Resume();
    
    DEBUG_LOG("Third outside log");
    
    c.Resume();
    
    DEBUG_LOG("Fourth outside log");
}
