#pragma once

#include <format>
#include <ostream>
#include <utility>

#include "core.hpp"
#include "reference_counter.hpp"

BEGIN_XNOR_CORE

/// @brief Custom XNOR smart pointer.
///        Represents both a <c>std::shared_ptr</c> and a <c>std::weak_ptr</c>.
///
/// @paragraph reason Reason
/// While using <c>std::weak_ptr</c>, we realized that it was not very practical because a <c>std::shared_ptr</c> needs to be
/// constructed from the former for the pointed type to be used. The <c>Pointer</c> type is meant to fix this issue
/// by being both a strong and a weak shared pointer.
///
/// @paragraph references Weak and Strong References
/// By default, creating a <c>Pointer</c> with constructor arguments from the pointed type allocates this type on the heap.
/// Copying this instance of <c>Pointer</c> creates a new weak reference by default, meaning that the copy won't keep the raw
/// pointer alive. When all the strong references go out of scope or are destroyed, the underlying pointed type is freed.
/// A strong reference can still be created if needed, by calling either <c>Pointer<T>::CreateStrongReference() const</c>,
/// <c>Pointer::ToStrongReference()</c>, or by creating a copy with <c>Pointer::Pointer(const Pointer&, bool)</c>
/// and giving a <c>true</c> value to the second argument.
///
/// @param T The type to point to.
/// 
/// @see <a href="https://en.cppreference.com/book/intro/smart_pointers">Smart Pointers</a>
/// @see <a href="https://en.cppreference.com/w/cpp/memory/shared_ptr">std::shared_ptr</a>
/// @see <a href="https://en.cppreference.com/w/cpp/memory/weak_ptr">std::weak_ptr</a>
template<typename T>
class Pointer
{
public:
    /// @brief Creates an empty <c>Pointer</c> without a reference counter and pointing to <c>nullptr</c>.
    Pointer() = default;
    
    Pointer(const Pointer& other, bool strongReference = false);

    Pointer(Pointer&& other) noexcept;

    template<typename Ty>
    explicit Pointer(const Pointer<Ty>& other, bool strongReference = false);

    template<typename Ty>
    explicit Pointer(Pointer<Ty>&& other) noexcept;

    template<typename... Args>
    explicit Pointer(Args... args);

    virtual ~Pointer();

    Pointer CreateStrongReference() const;

    Pointer& operator=(const Pointer& other);

    Pointer& operator=(Pointer&& other) noexcept;

    template<typename Ty>
    Pointer& operator=(const Pointer<Ty>& other);

    template<typename Ty>
    Pointer& operator=(Pointer<Ty>&& other) noexcept;

    explicit operator T*() const;

    explicit operator std::string() const;

    T& operator*();
    
    const T& operator*() const;

    T* operator->();
    
    const T* operator->() const;

    [[nodiscard]]
    bool IsValid();

    [[nodiscard]]
    const ReferenceCounter<T>* GetReferenceCounter() const;

    [[nodiscard]]
    bool GetIsStrongReference() const;

    void ToStrongReference();

    void ToWeakReference();
    
    void Reset();
    
private:
    ReferenceCounter<T>* m_ReferenceCounter = nullptr;

    bool m_IsStrongReference = false;
};

template<typename T>
Pointer<T>::Pointer(const Pointer& other, const bool strongReference)
    : m_ReferenceCounter(other.m_ReferenceCounter)
    , m_IsStrongReference(strongReference)
{
    if (strongReference)
        m_ReferenceCounter->IncStrong();
    else
        m_ReferenceCounter->IncWeak(this);
}

template<typename T>
Pointer<T>::Pointer(Pointer&& other) noexcept
    : m_ReferenceCounter(std::move(other.m_ReferenceCounter))
    , m_IsStrongReference(std::move(other.m_IsStrongReference))
{
    other.Reset();
}

template<typename T>
template<typename Ty>
Pointer<T>::Pointer(const Pointer<Ty>& other, const bool strongReference)
    : m_ReferenceCounter(reinterpret_cast<ReferenceCounter<T>>(other.GetReferenceCounter()))
    , m_IsStrongReference(strongReference)
{
    if (strongReference)
        m_ReferenceCounter->IncStrong();
    else
        m_ReferenceCounter->IncWeak(this);
}

template<typename T>
template<typename Ty>
Pointer<T>::Pointer(Pointer<Ty>&& other) noexcept
    : m_ReferenceCounter(reinterpret_cast<ReferenceCounter<T>*>(const_cast<ReferenceCounter<Ty>*>(std::move(other.GetReferenceCounter()))))
    , m_IsStrongReference(std::move(other.GetIsStrongReference()))
{
    other.Reset();
}

template<typename T>
template<typename... Args>
Pointer<T>::Pointer(Args... args)
    : m_ReferenceCounter(new ReferenceCounter<T>(std::forward<Args>(args)...))
    , m_IsStrongReference(true)
{
}

template<typename T>
Pointer<T>::~Pointer()
{
    if (!m_ReferenceCounter)
        return;
    
    if (!m_IsStrongReference)
    {
        m_ReferenceCounter->DecWeak(this);
        return;
    }

    if (m_ReferenceCounter->DecStrong())
        delete m_ReferenceCounter;
}

template<typename T>
Pointer<T> Pointer<T>::CreateStrongReference() const
{
    return Pointer(*this, true);
}

template<typename T>
Pointer<T>& Pointer<T>::operator=(const Pointer& other)
{
    if (this == &other)
        return *this;
    
    m_ReferenceCounter = other.m_ReferenceCounter;
    m_ReferenceCounter->IncWeak(this);
    
    return *this;
}

template<typename T>
Pointer<T>& Pointer<T>::operator=(Pointer&& other) noexcept
{
    if (this == &other)
        return *this;
    
    m_ReferenceCounter = std::move(other.m_ReferenceCounter);
    m_IsStrongReference = std::move(other.m_IsStrongReference);

    other.Reset();
    
    return *this;
}

template<typename T>
template<typename Ty>
Pointer<T>& Pointer<T>::operator=(const Pointer<Ty>& other)
{
    if (this == &other)
        return *this;
    
    m_ReferenceCounter = reinterpret_cast<ReferenceCounter<T>>(other.GetReferenceCounter());
    m_ReferenceCounter->IncWeak(this);
    
    return *this;
}

template<typename T>
template<typename Ty>
Pointer<T>& Pointer<T>::operator=(Pointer<Ty>&& other) noexcept
{
    if (this == &other)
        return *this;
    
    m_ReferenceCounter = reinterpret_cast<ReferenceCounter<T>>(std::move(other.GetReferenceCounter()));
    m_IsStrongReference = std::move(other.GetIsStrongReference());

    other.Reset();
    
    return *this;
}

template<typename T>
Pointer<T>::operator T*() const { return m_ReferenceCounter->GetPointer(); }

template<typename T>
Pointer<T>::operator std::string() const { return std::format("{{ ptr={:p}, sRefs={:d}, wRefs={:d}, isSRef={} }}", static_cast<const T*>(this), m_ReferenceCounter->GetStrong(), m_ReferenceCounter->GetWeak(), m_IsStrongReference); }

template<typename T>
T& Pointer<T>::operator*() { return *m_ReferenceCounter->GetPointer(); }

template<typename T>
const T& Pointer<T>::operator*() const { return *m_ReferenceCounter->GetPointer(); }

template<typename T>
T* Pointer<T>::operator->() { return m_ReferenceCounter->GetPointer(); }

template<typename T>
const T* Pointer<T>::operator->() const { return m_ReferenceCounter->GetPointer(); }

template <typename T>
bool Pointer<T>::IsValid() { return m_ReferenceCounter != nullptr; }

template<typename T>
bool Pointer<T>::GetIsStrongReference() const { return m_IsStrongReference; }

template<typename T>
void Pointer<T>::ToStrongReference()
{
    if (m_IsStrongReference)
        return;
    
    m_ReferenceCounter->IncStrong();
    m_ReferenceCounter->DecWeak(this);
    m_IsStrongReference = true;
}

template<typename T>
void Pointer<T>::ToWeakReference()
{
    if (!m_IsStrongReference)
        return;
    
    m_ReferenceCounter->DecStrong();
    m_ReferenceCounter->IncWeak(this);
    m_IsStrongReference = false;
}

template <typename T>
void Pointer<T>::Reset()
{
    m_ReferenceCounter = nullptr;
    m_IsStrongReference = false;
}

template<typename T>
const ReferenceCounter<T>* Pointer<T>::GetReferenceCounter() const { return m_ReferenceCounter; }

template<typename T>
bool operator==(const Pointer<T>& a, const Pointer<T>& b)
{
    return static_cast<const T*>(a) == static_cast<const T*>(b);
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Pointer<T>& ptr)
{
    return stream
        << "Pointer{ ptr=" << static_cast<const T*>(ptr)
        << ", strRefs=" << ptr.GetReferenceCounter()->GetStrong()
        << ", wRefs=" << ptr.GetReferenceCounter()->GetWeak()
        << ", isStrRef=" << std::boolalpha << ptr.GetIsStrongReference()
        << " }" << std::endl;
}

END_XNOR_CORE
