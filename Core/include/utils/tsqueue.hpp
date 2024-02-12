#pragma once

#include <mutex>
#include <functional>
#include <queue>

BEGIN_XNOR_CORE

/// <summary>
/// Thread-Safe Queue
/// </summary>
/// <typeparam name="T">The type of data to store</typeparam>
template <typename T>
class TsQueue
{
public:
    // Default constructor
    TsQueue() = default;

    // Prevent copy construction because of the mutex
    TsQueue(const TsQueue&) = delete;

    /// <summary>
    /// Gets a reference to the front item in the queue
    /// </summary>
    /// <returns>Item</returns>
    const T& Front();

    /// <summary>
    /// Gets a reference to the back item in the queue
    /// </summary>
    /// <returns>Item</returns>
    const T& Back();

    /// <summary>
    /// Pushes a new item on the back of the queue
    /// </summary>
    /// <param name="item">Item</param>
    void Push(const T& item);

    /// <summary>
    /// Checks if the queue is empty
    /// </summary>
    /// <returns>Empty</returns>
    bool Empty();

    /// <summary>
    /// Get the number of items in the queue
    /// </summary>
    /// <returns>Count</returns>
    size_t Count();

    /// <summary>
    /// Pops the item on the front of the queue
    /// </summary>
    /// <returns>Item</returns>
    T Pop();

private:
    // Mutex guarding the queue
    std::mutex mQueueMutex;

    std::queue<T> mQueue;
};

template <typename T>
const T& TsQueue<T>::Front()
{
    std::scoped_lock lock(mQueueMutex);
    return mQueue.front();
}

template <typename T>
const T& TsQueue<T>::Back()
{
    std::scoped_lock lock(mQueueMutex);
    return mQueue.back();
}

template <typename T>
void TsQueue<T>::Push(const T& item)
{
    std::scoped_lock lock(mQueueMutex);
    mQueue.push(std::move(item));
}

template <typename T>
bool TsQueue<T>::Empty()
{
    std::scoped_lock lock(mQueueMutex);
    return mQueue.empty();
}

template <typename T>
size_t TsQueue<T>::Count()
{
    std::scoped_lock lock(mQueueMutex);
    return mQueue.size();
}

template <typename T>
T TsQueue<T>::Pop()
{
    std::scoped_lock lock(mQueueMutex);
    T item = std::move(mQueue.front());
    mQueue.pop();
    return item;
}

END_XNOR_CORE
