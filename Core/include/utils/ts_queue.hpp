#pragma once

#include <mutex>
#include <queue>

BEGIN_XNOR_CORE

/// @file ts_queue.hpp
/// @brief Thread-Safe Queue
/// 
/// @tparam T The type of data to store.
///
/// @see <a href="https://en.cppreference.com/w/cpp/container/queue">std::queue</a>
template <typename T>
class TsQueue  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
    TsQueue() = default;

    // Prevent copy construction because of the mutex
    TsQueue(const TsQueue&) = delete;

    /// @brief Gets a reference to the front item in the queue
    /// @returns Item
    const T& Front();

    /// @brief Gets a reference to the back item in the queue
    /// @returns Item
    const T& Back();

    /// @brief Pushes a new item on the back of the queue by copying it
    /// @param item Item
    void Push(const T& item);

    /// @brief Pushes a new item on the back of the queue by moving it
    /// @param item Item
    void Push(T&& item);

    /// @brief Checks if the queue is empty
    /// @returns Empty
    bool Empty();

    /// @brief Get the number of items in the queue
    /// @returns Count
    size_t Count();

    /// @brief Pops the item on the front of the queue
    /// @returns Item
    T Pop();
    
    /// @brief Clears the queue
    void Clear();

private:
    // Mutex guarding the queue
    std::mutex m_QueueMutex;

    std::queue<T> m_Queue;
};

END_XNOR_CORE

#include "utils/ts_queue.inl"
