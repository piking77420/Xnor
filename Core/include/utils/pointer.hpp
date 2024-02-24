#pragma once

#include <format>
#include <ostream>
#include <utility>

#include "core.hpp"
#include "reference_counter.hpp"

BEGIN_XNOR_CORE

/// @brief Used to default-construct the value of a @ref Pointer.
/// @see @ref Pointer::Pointer(Construct)
struct Construct
{
};

/// @brief Custom XNOR smart pointer.
///        Represents both a @c std::shared_ptr and a @c std::weak_ptr.
///
/// @paragraph reason Reason
/// While using @c std::weak_ptr, we realized that it was not very practical because a @c std::shared_ptr needs to be
/// constructed from the former for the pointed type to be used. The @ref Pointer type is meant to fix this issue
/// by being both a strong and a weak shared pointer.
///
/// @paragraph usage Usage
/// The simplest way to create a @ref Pointer to wrap your type is by using @ref Pointer::Pointer<Args...>() "the forwarding variadic template constructor"
/// which allows you to do the following:
/// @code
/// struct Type { int i; explicit Type(int i) : i(i) {} };
///
/// Pointer<Type> ptr(7);
/// @endcode
/// ... and 7 will be forwarded as a parameter to the @c Type(int) constructor.
///
/// There are 3 other ways of initializing a @ref Pointer:
/// @code
/// // 1 - Default initialize the Pointer: this wraps a nullptr
/// Pointer<Type> ptr;
///
/// // 2 - Default initialize the wrapped value: this effectively calls the wrapped type's default constructor
/// Pointer<Type> ptr(Construct{}); // or Construct()
///
/// // 3 - Manually set the Pointer to nullptr: this is actually the same as default initializing it
/// Pointer<Type> ptr = nullptr;
/// @endcode
///
/// @paragraph references Weak and Strong References
/// By default, creating a @ref Pointer with constructor arguments from the pointed type allocates this type on the heap.
/// Copying this instance of @ref Pointer creates a new weak reference by default, meaning that the copy won't keep the raw
/// pointer alive. When all the strong references go out of scope or are destroyed, the underlying pointed type is freed.
/// A strong reference can still be created if needed, by calling either @ref Pointer::CreateStrongReference(),
/// @ref Pointer::ToStrongReference(), or by creating a copy using @ref Pointer::Pointer(const Pointer&, bool) "the copy constructor"
/// and giving a @c true value to the second argument.
///
/// @tparam T The type to point to. Most of the time, this shouldn't be a pointer type.
/// 
/// @see <a href="https://en.cppreference.com/book/intro/smart_pointers">Smart Pointers</a>
/// @see <a href="https://en.cppreference.com/w/cpp/memory/shared_ptr">std::shared_ptr</a>
/// @see <a href="https://en.cppreference.com/w/cpp/memory/weak_ptr">std::weak_ptr</a>
template<typename T>
class Pointer
{
public:
    /// @brief Creates an empty @ref Pointer without a reference counter and pointing to @c nullptr.
    Pointer() = default;
    
    /// @brief Creates a copy of another @ref Pointer, specifying whether it is a weak or strong reference.
    Pointer(const Pointer& other, bool strongReference = false);

    Pointer(Pointer&& other) noexcept;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    Pointer(nullptr_t);

    /// @brief Creates a @ref Pointer with a default-initialized value.
    ///
    /// The parameter is actually ignored in this call, and is only used to differentiate between a call to
    /// @ref Pointer::Pointer() "the default constructor" and this one. We need this specific constructor
    /// because the C++ language doesn't allow us to call a template constructor with empty template
    /// parameters without template deduction.
    /// 
    /// This means that you can actually call this constructor like this:
    /// @code
    /// Pointer<Type> ptr(Construct{});
    /// @endcode
    explicit Pointer(Construct);

    template<typename U>
    explicit Pointer(const Pointer<U>& other, bool strongReference = false);

    template<typename U>
    explicit Pointer(Pointer<U>& other, bool strongReference = false);

    template<typename U>
    explicit Pointer(Pointer<U>&& other) noexcept;

    template<typename... Args>
    explicit Pointer(Args&&... args);

    virtual ~Pointer();

    /// @brief Creates a new strong reference to this pointer
    [[nodiscard]]
    Pointer CreateStrongReference() const;

    [[nodiscard]]
    const T* Get() const;

    [[nodiscard]]
    T* Get();

    [[nodiscard]]
    bool_t IsValid() const;

    /// @brief Converts this @ref Pointer to a strong reference.
    void ToStrongReference();

    /// @brief Converts this @ref Pointer to a weak reference.
    void ToWeakReference();

    [[nodiscard]]
    const ReferenceCounter<T>* GetReferenceCounter() const;

    [[nodiscard]]
    bool_t GetIsStrongReference() const;

    /// @brief Resets this @ref Pointer to a @c nullptr.
    ///
    /// @warning
    /// This function is used internally and is not meant to be used except if you really know what you
    /// are doing. This doesn't free the potentially allocated memory. Use it at your own risks.
    void Reset();

    Pointer& operator=(const Pointer& other);

    Pointer& operator=(Pointer&& other) noexcept;

    Pointer& operator=(nullptr_t);

    template<typename U>
    Pointer& operator=(const Pointer<U>& other);

    template<typename U>
    Pointer& operator=(Pointer<U>&& other) noexcept;

    [[nodiscard]]
    explicit operator T*() const;

    [[nodiscard]]
    explicit operator std::string() const;

    // ReSharper disable once CppNonExplicitConversionOperator
    /// @brief Converts this @ref Pointer to a @c bool_t the same way a raw pointer would.
    [[nodiscard]]
    operator bool_t() const;

    [[nodiscard]]
    T& operator*();
    
    [[nodiscard]]
    const T& operator*() const;

    [[nodiscard]]
    T* operator->();
    
    [[nodiscard]]
    const T* operator->() const;
    
private:
    ReferenceCounter<T>* m_ReferenceCounter = nullptr;

    bool m_IsStrongReference = false;

    void SetReferenceCounter(ReferenceCounter<T>* newReferenceCounter);

    void CheckReferenceCounterValid();
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
    if (!m_IsStrongReference)
    {
        m_ReferenceCounter->DecWeak(&other);
        m_ReferenceCounter->IncWeak(this);
    }
    
    other.Reset();
}

template<typename T>
Pointer<T>::Pointer(nullptr_t)
{
}

template<typename T>
Pointer<T>::Pointer(Construct)
    : m_ReferenceCounter(new ReferenceCounter<T>)
    , m_IsStrongReference(true)
{
}

template<typename T>
template<typename U>
Pointer<T>::Pointer(const Pointer<U>& other, const bool strongReference)
    : m_ReferenceCounter(reinterpret_cast<ReferenceCounter<T>*>(const_cast<ReferenceCounter<U>*>(other.GetReferenceCounter())))
    , m_IsStrongReference(strongReference)
{
    if (strongReference)
        m_ReferenceCounter->IncStrong();
    else
        m_ReferenceCounter->IncWeak(this);
}

template<typename T>
template<typename U>
Pointer<T>::Pointer(Pointer<U>& other, const bool strongReference)
    : Pointer(const_cast<const Pointer<U>&>(other), strongReference)
{
}

template<typename T>
template<typename U>
Pointer<T>::Pointer(Pointer<U>&& other) noexcept  // NOLINT(cppcoreguidelines-rvalue-reference-param-not-moved)
    : m_ReferenceCounter(reinterpret_cast<ReferenceCounter<T>*>(const_cast<ReferenceCounter<U>*>(std::move(other.GetReferenceCounter()))))
    , m_IsStrongReference(std::move(other.GetIsStrongReference()))
{
    if (!m_IsStrongReference)
    {
        m_ReferenceCounter->DecWeak(reinterpret_cast<Pointer*>(&other));
        m_ReferenceCounter->IncWeak(this);
    }
    
    other.Reset();
}

template<typename T>
template<typename... Args>
Pointer<T>::Pointer(Args&&... args) // NOLINT(cppcoreguidelines-missing-std-forward)
    : m_ReferenceCounter(new ReferenceCounter<T>(std::forward<Args>(args)...))
    , m_IsStrongReference(true)
{
}

template<typename T>
Pointer<T>::~Pointer()
{
    SetReferenceCounter(nullptr);
}

template<typename T>
Pointer<T> Pointer<T>::CreateStrongReference() const
{
    return Pointer(*this, true);
}

template<typename T>
const T* Pointer<T>::Get() const
{
    return m_ReferenceCounter->GetPointer();
}

template<typename T>
T* Pointer<T>::Get()
{
    return m_ReferenceCounter->GetPointer();
}

template<typename T>
Pointer<T>& Pointer<T>::operator=(const Pointer& other)  // NOLINT(bugprone-unhandled-self-assignment)
{
    if (this == &other)
        return *this;
    
    SetReferenceCounter(other.m_ReferenceCounter);
    if (m_ReferenceCounter)
        m_ReferenceCounter->IncWeak(this);
    
    return *this;
}

template<typename T>
Pointer<T>& Pointer<T>::operator=(Pointer&& other) noexcept
{
    if (this == &other)
        return *this;

    SetReferenceCounter(std::move(other.m_ReferenceCounter));
    m_IsStrongReference = std::move(other.m_IsStrongReference);

    if (m_ReferenceCounter && !m_IsStrongReference)
    {
        m_ReferenceCounter->DecWeak(&other);
        m_ReferenceCounter->IncWeak(this);
    }

    other.Reset();
    
    return *this;
}

template<typename T>
Pointer<T>& Pointer<T>::operator=(nullptr_t)
{
    SetReferenceCounter(nullptr);
    m_IsStrongReference = false;
    
    return *this;
}

template<typename T>
template<typename U>
Pointer<T>& Pointer<T>::operator=(const Pointer<U>& other)
{
    if (this == &other)
        return *this;
    
    SetReferenceCounter(reinterpret_cast<ReferenceCounter<T>*>(other.GetReferenceCounter()));
    m_ReferenceCounter->IncWeak(this);
    
    return *this;
}

template<typename T>
template<typename U>
Pointer<T>& Pointer<T>::operator=(Pointer<U>&& other) noexcept
{
    if (reinterpret_cast<int8_t*>(this) == reinterpret_cast<int8_t*>(&other))
        return *this;

    SetReferenceCounter(reinterpret_cast<ReferenceCounter<T>*>(const_cast<ReferenceCounter<U>*>(other.GetReferenceCounter())));
    m_IsStrongReference = std::move(other.GetIsStrongReference());

    if (m_ReferenceCounter && !m_IsStrongReference)
    {
        m_ReferenceCounter->DecWeak(reinterpret_cast<const Pointer*>(&other));
        m_ReferenceCounter->IncWeak(this);
    }

    other.Reset();
    
    return *this;
}

template<typename T>
Pointer<T>::operator T*() const
{
    if (m_ReferenceCounter)
        return m_ReferenceCounter->GetPointer();
    return nullptr;
}

template<typename T>
Pointer<T>::operator std::string() const
{
    return std::format(
        "{{ ptr={:p}, sRefs={:d}, wRefs={:d}, isSRef={} }}",
        static_cast<const T*>(this),
        m_ReferenceCounter->GetStrong(),
        m_ReferenceCounter->GetWeak(),
        m_IsStrongReference
    );
}

template<typename T>
Pointer<T>::operator bool_t() const { return IsValid(); }

template<typename T>
T& Pointer<T>::operator*() { return *m_ReferenceCounter->GetPointer(); }

template<typename T>
const T& Pointer<T>::operator*() const { return *m_ReferenceCounter->GetPointer(); }

template<typename T>
T* Pointer<T>::operator->() { return m_ReferenceCounter->GetPointer(); }

template<typename T>
const T* Pointer<T>::operator->() const { return m_ReferenceCounter->GetPointer(); }

template <typename T>
bool_t Pointer<T>::IsValid() const { return m_ReferenceCounter != nullptr; }

template<typename T>
bool_t Pointer<T>::GetIsStrongReference() const { return m_IsStrongReference; }

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
void Pointer<T>::SetReferenceCounter(ReferenceCounter<T>* newReferenceCounter)
{
    if (m_ReferenceCounter)
    {
        if (m_IsStrongReference)
        {
            if (m_ReferenceCounter->DecStrong())
                CheckReferenceCounterValid();
        }
        else
        {
            m_ReferenceCounter->DecWeak(this);
        }
    }

    m_ReferenceCounter = newReferenceCounter;
}

template<typename T>
void Pointer<T>::CheckReferenceCounterValid()
{
    if (!m_ReferenceCounter)
        return;
    
    if (m_ReferenceCounter->GetStrong() == 0)
    {
        delete m_ReferenceCounter;
        Reset();
    }
}

/// @brief Compares two @ref Pointer "Pointers" by checking if they point to the same address.
template<typename T>
bool operator==(const Pointer<T>& a, const Pointer<T>& b) { return static_cast<const T*>(a) == static_cast<const T*>(b); }

/// @brief Compares two @ref Pointer "Pointers" by checking if they point to the same address.
template<typename T>
bool operator!=(const Pointer<T>& a, const Pointer<T>& b) { return !(a == b); }

/// @brief Compares two @ref Pointer "Pointers" by checking if they point to the same address.
template<typename T, typename U>
bool operator==(const Pointer<T>& a, const Pointer<U>& b) { return static_cast<const T*>(a) == reinterpret_cast<const T*>(static_cast<const U*>(b)); }

/// @brief Compares two @ref Pointer "Pointers" by checking if they point to the same address.
template<typename T, typename U>
bool operator!=(const Pointer<T>& a, const Pointer<U>& b) { return !(a == b); }

/// @brief Checks if a @ref Pointer is null.
template<typename T>
bool operator==(const Pointer<T>& a, const nullptr_t) { return !a.IsValid(); }

/// @brief Checks if a @ref Pointer is not null.
template<typename T>
bool operator!=(const Pointer<T>& a, const nullptr_t) { return a.IsValid(); }

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
