﻿#pragma once

#include <utility>
#include <vector>
#include <stdexcept>

#include "core.hpp"

BEGIN_XNOR_CORE

template<typename T>
class Pointer;

/// @brief Reference counter for the <c>Pointer</c> class. This shouldn't be used for anything else.
template<typename T>
class ReferenceCounter
{
public:
    ReferenceCounter();

    template<typename... Args>
    explicit ReferenceCounter(Args... args);

    ReferenceCounter(const ReferenceCounter& other) = delete;

    ReferenceCounter(ReferenceCounter&& other) = delete;

    ReferenceCounter& operator=(const ReferenceCounter& other) = delete;

    ReferenceCounter& operator=(ReferenceCounter&& other) = delete;

    ~ReferenceCounter();

    void IncStrong();

    void IncWeak(Pointer<T>* weakReferenceOwner);

    // Returns whether the strong ref count becomes 0
    bool DecStrong();

    void DecWeak(Pointer<T>* weakReferenceOwner);

    [[nodiscard]]
    uint64_t GetStrong() const;
    
    [[nodiscard]]
    uint64_t GetWeak() const;

    [[nodiscard]]
    T* GetPointer();

    [[nodiscard]]
    const T* GetPointer() const;

private:
    uint64_t m_Strong = 1;

    T* m_Pointer = nullptr;

    std::vector<Pointer<T>*> m_WeakReferenceOwners;
};

template<typename T>
ReferenceCounter<T>::ReferenceCounter()
{
}

template<typename T>
template<typename ... Args>
ReferenceCounter<T>::ReferenceCounter(Args... args)
    : m_Pointer(new T(std::forward<Args>(args)...))
{
}

template<typename T>
ReferenceCounter<T>::~ReferenceCounter()
{
    for (auto& weakReferenceOwner : m_WeakReferenceOwners)
        weakReferenceOwner->Reset();
    
    delete m_Pointer;
}

template<typename T>
void ReferenceCounter<T>::IncStrong()
{
    m_Strong++;
}

template<typename T>
void ReferenceCounter<T>::IncWeak(Pointer<T>* weakReferenceOwner)
{
    m_WeakReferenceOwners.push_back(weakReferenceOwner);
}

template<typename T>
bool ReferenceCounter<T>::DecStrong()
{
    if (m_Strong == 0 || --m_Strong == 0)
        return true;

    return false;
}

template<typename T>
void ReferenceCounter<T>::DecWeak(Pointer<T>* weakReferenceOwner)
{
    const auto&& it = std::find(m_WeakReferenceOwners.begin(), m_WeakReferenceOwners.end(), weakReferenceOwner);
    
    if (it == m_WeakReferenceOwners.end())
        throw std::runtime_error("Tried to decrement the weak reference count of a smart pointer with one that wasn't registered");
    
    m_WeakReferenceOwners.erase(it);
}

template<typename T>
uint64_t ReferenceCounter<T>::GetStrong() const
{
    return m_Strong;
}

template<typename T>
uint64_t ReferenceCounter<T>::GetWeak() const
{
    return m_WeakReferenceOwners.size();
}

template<typename T>
T* ReferenceCounter<T>::GetPointer()
{
    return m_Pointer;
}

template<typename T>
const T* ReferenceCounter<T>::GetPointer() const
{
    return m_Pointer;
}

END_XNOR_CORE
