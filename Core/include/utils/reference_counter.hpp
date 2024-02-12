#pragma once

#include <utility>
#include <vector>

#include "core.hpp"

BEGIN_XNOR_CORE

template<typename T>
class Pointer;

template<typename T>
class ReferenceCounter
{
public:
    ReferenceCounter();

    template<typename... Args>
    explicit ReferenceCounter(Args&&... args);

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
    size_t GetStrong() const;
    
    [[nodiscard]]
    size_t GetWeak() const;

private:
    size_t m_Strong = 1;

    T* m_Pointer = nullptr;

    std::vector<Pointer<T>*> m_WeakReferenceOwners;
};

template<typename T>
ReferenceCounter<T>::ReferenceCounter()
{
}

template<typename T>
template<typename ... Args>
ReferenceCounter<T>::ReferenceCounter(Args&&... args)
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
    if (GetWeak() > 0)
        m_WeakReferenceOwners.erase(std::find(m_WeakReferenceOwners.begin(), m_WeakReferenceOwners.end(), weakReferenceOwner));
}

template<typename T>
size_t ReferenceCounter<T>::GetStrong() const
{
    return m_Strong;
}

template<typename T>
size_t ReferenceCounter<T>::GetWeak() const
{
    return m_WeakReferenceOwners.size();
}

END_XNOR_CORE
