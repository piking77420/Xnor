#pragma once

BEGIN_XNOR_CORE

template <typename T>
PolyPtr<T>::PolyPtr()
{
    m_Ptr = nullptr;
    m_Hash = 0;
}

template <typename T>
PolyPtr<T>::~PolyPtr()
{
    Destroy();
}

template <typename T>
template <typename U>
void PolyPtr<T>::Create()
{
    static_assert(std::is_base_of_v<T, U>, "Type must be a child of the PolyPtr type");

    Destroy();
    
    m_Ptr = new U();
    m_Hash = Utils::GetTypeHash<U>();
}

template <typename T>
template <typename U>
void PolyPtr<T>::Create(U* const value)
{
    static_assert(std::is_base_of_v<T, U>, "Type must be a child of the PolyPtr type");

    Destroy();
    
    m_Ptr = value;
    m_Hash = Utils::GetTypeHash<U>();
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
bool_t PolyPtr<T>::IsOfType() const
{
    static_assert(std::is_base_of_v<T, U>, "Type must be a child of the PolyPtr type");
    
    return dynamic_cast<U*>(m_Ptr) != nullptr;
}

template <typename T>
size_t PolyPtr<T>::GetHash() const
{
    return m_Hash;
}

template <typename T>
void PolyPtr<T>::Destroy()
{
    if (m_Ptr)
    {
        delete m_Ptr;
        m_Hash = 0;
        m_Ptr = nullptr;
    }
}

END_XNOR_CORE
