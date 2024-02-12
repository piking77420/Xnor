#pragma once

#include <format>
#include <ostream>
#include <utility>

#include "core.hpp"
#include "reference_counter.hpp"

BEGIN_XNOR_CORE

template<typename T>
class Pointer
{
public:
    Pointer() = default;
    
    Pointer(const Pointer& other, bool strongReference = false);

    Pointer(Pointer&& other) noexcept;

    template<typename Ty>
    explicit Pointer(const Pointer<Ty>& other, bool strongReference = false);

    template<typename Ty>
    explicit Pointer(Pointer<Ty>&& other) noexcept;

    template<typename... Args>
    explicit Pointer(Args&&... args);

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
Pointer<T>::Pointer(Args&&... args)
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
Pointer<T>::operator T*() const { return m_ReferenceCounter->m_Pointer; }

template<typename T>
Pointer<T>::operator std::string() const { return std::format("{{ ptr={:p}, sRefs={:d}, wRefs={:d}, isSRef={} }}", static_cast<const T*>(this), m_ReferenceCounter->GetStrong(), m_ReferenceCounter->GetWeak(), m_IsStrongReference); }

template<typename T>
T& Pointer<T>::operator*() { return *m_ReferenceCounter->m_Pointer; }

template<typename T>
const T& Pointer<T>::operator*() const { return *m_ReferenceCounter->m_Pointer; }

template<typename T>
T* Pointer<T>::operator->() { return m_ReferenceCounter->m_Pointer; }

template<typename T>
const T* Pointer<T>::operator->() const { return m_ReferenceCounter->m_Pointer; }

template <typename T>
bool Pointer<T>::IsValid() { return m_ReferenceCounter != nullptr; }

template<typename T>
bool Pointer<T>::GetIsStrongReference() const { return m_IsStrongReference; }

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
