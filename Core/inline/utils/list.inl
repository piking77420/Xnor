// ReSharper disable CppClangTidyCppcoreguidelinesRvalueReferenceParamNotMoved
#pragma once

#include <stdexcept>

#include "utils/allocator.hpp"

BEGIN_XNOR_CORE

template <typename T>
List<T>::List()
    : m_Size(0)
{
    m_Capacity = 1;

    m_Data = Allocator::Allocate<T>(m_Capacity);
}

template <typename T>
List<T>::List(const size_t size)
    : m_Size(size)
{
    m_Capacity = std::bit_ceil(m_Size);

    m_Data = Allocator::Allocate<T>(m_Capacity);

    for (size_t i = 0; i < m_Size; i++)
        Allocator::Construct<T>(&m_Data[i]);
}

template <typename T>
List<T>::List(const size_t size, const T& defaultValue)
    : m_Size(size)
{
    m_Capacity = std::bit_ceil(m_Size);

    m_Data = Allocator::Allocate<T>(m_Capacity);
    
    for (size_t i = 0; i < m_Size; i++)
        Allocator::Construct<T>(&m_Data[i], defaultValue);
}

template <typename T>
List<T>::List(const size_t size, const T* const values)
    : m_Size(size)
{
    m_Capacity = std::bit_ceil(m_Size);

    m_Data = Allocator::Allocate<T>(m_Capacity);

    for (size_t i = 0; i < m_Size; i++)
        Allocator::Construct<T>(&m_Data[i], values[i]);
}

template <typename T>
List<T>::List(const std::initializer_list<T>& values)
{
    m_Size = values.size();
    m_Capacity = std::bit_ceil(m_Size);

    m_Data = Allocator::Allocate<T>(m_Capacity);
    
    const T* const it = values.begin();

    for (size_t i = 0; i < m_Size; i++)
        Allocator::Construct<T>(&m_Data[i], it[i]);
}

template <typename T>
List<T>::~List()
{
    for (size_t i = 0; i < m_Size; i++)
    {
        m_Data[i].~T();
    }

    Allocator::Deallocate<T>(m_Data, m_Capacity);
    m_Data = nullptr;
    
    m_Size = 0;
    m_Capacity = 0;
}

template <typename T>
void List<T>::Resize(const size_t size)
{
    CheckGrow(size);

    const size_t oldSize = m_Size;
    m_Size = size;

    for (size_t i = oldSize; i < size; i++)
        Allocator::Construct<T>(&m_Data[i]);
}

template <typename T>
void List<T>::Clear()
{
    if (m_Size == 0)
        return;
    
    for (size_t i = 0; i < m_Size; i++)
    {
        m_Data[i].~T();
    }

    CheckShrink(0);
    m_Size = 0;
}

template <typename T>
void List<T>::Add()
{
    EmplaceInternal(m_Size);
}

template <typename T>
void List<T>::Add(const T& element)
{
    EmplaceInternal(m_Size, element);
}

template <typename T>
void List<T>::Add(T&& element)
{
    EmplaceInternal(m_Size, std::forward<T>(element));
}

template <typename T>
void List<T>::AddRange(const T* const data, const size_t number)
{
    // FIXME
    CheckGrow(m_Size + number);

    std::memcpy(&m_Data[m_Size], data, number * sizeof(T));

    m_Size += number;
}

template <typename T>
void List<T>::AddRange(const std::initializer_list<T>& values)
{
    // FIXME
    const size_t number = values.size();

    CheckGrow(m_Size + number);

    std::memcpy(&m_Data[m_Size], values.begin(), number * sizeof(T));

    m_Size += number;
}

template <typename T>
void List<T>::Fill(const T& value)
{
    for (size_t i = 0; i < m_Size; i++)
        Allocator::Construct<T>(&m_Data[i], value);
}

template <typename T>
void List<T>::Fill(T&& value)
{
    for (size_t i = 0; i < m_Size; i++)
        Allocator::Construct<T, T&&>(&m_Data[i], std::forward<T>(value));
}

template <typename T>
template <class... Args>
void List<T>::Emplace(Args&&... args)
{
    EmplaceInternal(m_Size, std::forward<Args>(args)...);
}

template <typename T>
void List<T>::Insert(size_t index)
{
    // FIXME
    if (index >= m_Size)
        throw std::invalid_argument("List insert index out of range");

    CheckGrow(m_Size + 1);

    std::memcpy(&m_Data[index + 1], &m_Data[index], (m_Size - index) * sizeof(T));
    Allocator::Construct<T>(&m_Data[m_Size]);
    
    m_Size++;
}

template <typename T>
void List<T>::Insert(const T& element, const size_t index)
{
    // FIXME
    if (index >= m_Size)
        throw std::invalid_argument("List insert index out of range");

    CheckGrow(m_Size + 1);

    std::memcpy(&m_Data[index + 1], &m_Data[index], (m_Size - index) * sizeof(T));
    Allocator::Construct<T>(&m_Data[index], element);
    
    m_Size++;
}

template <typename T>
void List<T>::Insert(T&& element, const size_t index)
{
    // FIXME
    if (index >= m_Size)
        throw std::invalid_argument("List insert index out of range");

    CheckGrow(m_Size + 1);

    std::memcpy(&m_Data[index + 1], &m_Data[index], (m_Size - index) * sizeof(T));
    Allocator::Construct<T>(&m_Data[index], std::forward<T>(element));
    
    m_Size++;
}

template <typename T>
void List<T>::Remove(const T& element)
{
    // FIXME
    size_t i;
    for (i = 0; i < m_Size; i++)
    {
        if (m_Data[i] == element)
            break;
    }

    if (i == m_Size)
        return;

    m_Data[i].~T();
    std::memcpy(&m_Data[i], &m_Data[i + 1], (m_Size - i - 1) * sizeof(T));

    CheckShrink(m_Size - 1);
    m_Size--;
}

template <typename T>
void List<T>::RemoveAt(const size_t index)
{
    // FIXME
    if (index >= m_Size)
        throw std::invalid_argument("List remove at subscript out of range");

    m_Data[index].~T();
    std::memcpy(&m_Data[index], &m_Data[index + 1], (m_Size - index - 1) * sizeof(T));

    CheckShrink(m_Size - 1);
    m_Size--;
}

template <typename T>
void List<T>::RemoveRange(const size_t start, const size_t end)
{
    // FIXME
    if (start >= m_Size)
        throw std::invalid_argument("List remove range start subscript out of range");
    
    if (end >= m_Size)
        throw std::invalid_argument("List remove range start subscript out of range");
    
    if (start >= end)
        throw std::invalid_argument("List remove range start was higher than range end");

    const size_t removedSize = end - start;

    for (size_t i = start; i < end; i++)
    {
        m_Data[i].~T();
    }
    
    std::memcpy(&m_Data[start], &m_Data[end + 1], (m_Size - end - 1) * sizeof(T));

    CheckShrink(m_Size - removedSize);
    m_Size -= removedSize;
}

template <typename T>
bool_t List<T>::Contains(const T& element) const
{
    for (size_t i = 0; i < m_Size; i++)
    {
        if (m_Data[i] == element)
            return true;
    }

    return false;
}

template <typename T>
void List<T>::Iterate(const std::function<void(T*, size_t)>& lambda)
{
    for (size_t i = 0; i < m_Size; i++)
        lambda(&m_Data[i], i);
}

template <typename T>
void List<T>::Iterate(const std::function<void(const T*, size_t)>& lambda) const
{
    for (size_t i = 0; i < m_Size; i++)
        lambda(&m_Data[i], i);
}

template <typename T>
bool_t List<T>::Exists(const std::function<bool_t(const T*, size_t)>& lambda) const
{
    for (size_t i = 0; i < m_Size; i++)
    {
        if (lambda(&m_Data[i], i))
            return true;
    }

    return false;
}

template <typename T>
T* List<T>::Find(const std::function<bool_t(const T*, size_t)>& lambda)
{
    for (size_t i = 0; i < m_Size; i++)
    {
        if (lambda(&m_Data[i], i))
            return &m_Data[i];
    }

    return nullptr;
}

template <typename T>
bool_t List<T>::IsValid() const
{
    if (m_Data == nullptr)
        return false;

    if (m_Capacity == 0)
        return false;

    if (m_Size > m_Capacity)
        return false;

    return true;
}

template <typename T>
T* List<T>::GetData() const
{
    return m_Data;
}

template <typename T>
size_t List<T>::GetSize() const
{
    return m_Size;
}

template <typename T>
size_t List<T>::GetCapacity() const
{
    return m_Capacity;
}

template <typename T>
T& List<T>::operator[](const size_t index)
{
    if (index >= m_Size)
        throw std::invalid_argument("List subscript out of range");

    return m_Data[index];
}

template <typename T>
const T& List<T>::operator[](const size_t index) const
{
    if (index >= m_Size)
        throw std::invalid_argument("List subscript out of range");

    return m_Data[index];
}

template <typename T>
template <typename... Args>
void List<T>::EmplaceInternal(const size_t where, Args&&... args)
{
    const size_t oldSize = m_Size;
    const size_t newSize = oldSize + 1;
    T* const oldPtr = m_Data;

    if (newSize > m_Capacity)
    {
        m_Capacity <<= 1;

        T* const newPtr = Allocator::Allocate<T>(m_Capacity);

        for (size_t i = 0; i < oldSize; i++)
            Allocator::Construct<T>(&newPtr[i], m_Data[i]);

        m_Data = newPtr;

        Allocator::Deallocate<T>(oldPtr, oldSize);
    }

    Allocator::Construct<T>(&m_Data[where], std::forward<Args>(args)...);
}

template <typename T>
void List<T>::CheckGrow(const size_t newSize)
{
    /*if (newSize > m_Capacity)
    {
        m_Capacity = std::bit_ceil(newSize);

        Realloc(m_Capacity);
    }*/
}

template <typename T>
void List<T>::CheckShrink(const size_t newSize)
{
    /*if (newSize < m_Capacity)
    {
        m_Capacity = std::bit_ceil(newSize);

        Realloc(m_Capacity);
    }*/
}

END_XNOR_CORE
