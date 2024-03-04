﻿#pragma once

BEGIN_XNOR_CORE

template <typename T>
class PolyPtr
{
public:
    using Type = T;
    
    PolyPtr();
    ~PolyPtr();

    DEFAULT_COPY_MOVE_OPERATIONS(PolyPtr)

    template <typename U>
    void Create();

    template <typename U>
    void Create(U* value);

    T& operator*();
    
    const T& operator*() const;

    T* operator->();
    
    const T* operator->() const;

    template <typename U>
    [[nodiscard]]
    U* Cast();
    
    template <typename U>
    [[nodiscard]]
    const U* Cast() const;

    [[nodiscard]]
    void* AsVoid();

    [[nodiscard]]
    const void* AsVoid() const;

    template <typename U>
    [[nodiscard]]
    bool IsOfType() const;

    [[nodiscard]]
    size_t GetHash() const;

private:
    T* m_Ptr;
    size_t m_Hash;
};

template <typename T>
PolyPtr<T>::PolyPtr()
{
    m_Ptr = nullptr;
    m_Hash = 0;
}

template <typename T>
PolyPtr<T>::~PolyPtr()
{
    if (m_Ptr)
    {
        delete m_Ptr;
        m_Hash = 0;
        m_Ptr = nullptr;
    }
}

template <typename T>
template <typename U>
void PolyPtr<T>::Create()
{
    static_assert(std::is_base_of_v<T, U>, "Type must be a child of the PolyPtr type");
    
    m_Ptr = new U();
    m_Hash = typeid(U).hash_code();
}

template <typename T>
template <typename U>
void PolyPtr<T>::Create(U* value)
{
    static_assert(std::is_base_of_v<T, U>, "Type must be a child of the PolyPtr type");
    
    m_Ptr = value;
    m_Hash = typeid(U).hash_code();
}

template <typename T>
T& PolyPtr<T>::operator*()
{
    return *m_Ptr;
}

template <typename T>
const T& PolyPtr<T>::operator*() const
{
    return *m_Ptr;
}

template <typename T>
T* PolyPtr<T>::operator->()
{
    return m_Ptr;
}

template <typename T>
const T* PolyPtr<T>::operator->() const
{
    return m_Ptr;
}

template <typename T>
template <typename U>
U* PolyPtr<T>::Cast()
{
    static_assert(std::is_base_of_v<T, U>, "Type must be a child of the PolyPtr type");
    
    return reinterpret_cast<U*>(m_Ptr);
}

template <typename T>
template <typename U>
const U* PolyPtr<T>::Cast() const
{
    static_assert(std::is_base_of_v<T, U>, "Type must be a child of the PolyPtr type");
    
    return reinterpret_cast<U*>(m_Ptr);
}

template <typename T>
void* PolyPtr<T>::AsVoid()
{
    return static_cast<void*>(m_Ptr);
}

template <typename T>
const void* PolyPtr<T>::AsVoid() const
{
    return static_cast<const void*>(m_Ptr);
}

template <typename T>
template <typename U>
bool PolyPtr<T>::IsOfType() const
{
    static_assert(std::is_base_of_v<T, U>, "Type must be a child of the PolyPtr type");
    
    return dynamic_cast<U*>(m_Ptr) != nullptr;
}

template <typename T>
size_t PolyPtr<T>::GetHash() const
{
    return m_Hash;
}

END_XNOR_CORE
