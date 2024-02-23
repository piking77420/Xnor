#pragma once

#include <functional>
#include "core.hpp"

BEGIN_XNOR_CORE

template <typename T>
class List
{
public:
    static_assert(std::is_default_constructible_v<T>, "T must have a default constructor");
    static_assert(std::is_copy_constructible_v<T>, "T must have a copy constructor");
    // static_assert(std::is_assignable_v<T, T>, "T must have a copy operator");
    // static_assert(std::is_assignable_v<T, std::add_const_t<T>>, "T must have a copy operator");

    using value_type = T;

    explicit List();
    explicit List(size_t size);
    explicit List(size_t size, T&& defaultValue);
    explicit List(size_t size, const T values[]);
    List(const std::initializer_list<T>& values);

    ~List();

    void Reserve(size_t capacity);
    void Clear();

    void Add();
    void Add(T& element);
    void Add(T&& element);
    
    void AddRange(const T* data, size_t number);
    void AddRange(const std::initializer_list<T>& values);

    template <class... Args>
    void Emplace(Args&&... args);

    void Insert(const T& elemnt, size_t index);
    void Insert(const T&& elemnt, size_t index);

    void Remove(const T& element);
    void Remove(const T&& element);
    void RemoveAt(size_t index);

    [[nodiscard]]
    bool Contains(const T& element) const;
    [[nodiscard]]
    bool Contains(const T&& element) const;

    void Iterate(std::function<void(T*, size_t)> lambda);
    bool Exists(std::function<bool(const T*, size_t)> lambda);
    T* Find(std::function<bool(const T*, size_t)> lambda);

    [[nodiscard]]
    bool IsValid() const;

    [[nodiscard]]
    T* GetData() const;

    [[nodiscard]]
    size_t GetSize() const;

    [[nodiscard]]
    size_t GetCapacity() const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

#ifndef UNIT_TEST
private:
#endif
    T* m_Data;
    size_t m_Size;
    size_t m_Capacity;

    void Malloc(size_t size);
    void Calloc(size_t size);
    void Realloc(size_t size);

    void CheckGrow(size_t newSize);
    void CheckShrink(size_t newSize);
};

template <typename T>
List<T>::List()
    : m_Size(0)
{
    m_Capacity = 1;

    Calloc(m_Capacity);
}

template <typename T>
List<T>::List(const size_t size)
    : m_Size(size)
{
    m_Capacity = std::bit_ceil(m_Size);
    
    Malloc(m_Capacity);

    for (size_t i = 0; i < m_Size; i++)
        m_Data[i] = T();
}

template <typename T>
List<T>::List(const size_t size, T&& defaultValue)
    : m_Size(size)
{
    m_Capacity = std::bit_ceil(m_Size);
    
    Malloc(m_Capacity);

    for (size_t i = 0; i < m_Size; i++)
        m_Data[i] = defaultValue;
}

template <typename T>
List<T>::List(const size_t size, const T values[])
    : m_Size(size)
{
    m_Capacity = std::bit_ceil(m_Size);
    
    Malloc(m_Capacity);

    for (size_t i = 0; i < m_Size; i++)
        m_Data[i] = values[i];
}

template <typename T>
List<T>::List(const std::initializer_list<T>& values)
{
    m_Size = values.size();
    m_Capacity = std::bit_ceil(m_Size);
    
    Malloc(m_Capacity);
    
    const T* const it = values.begin();

    for (size_t i = 0; i < m_Size; i++)
        m_Data[i] = it[i];
}

template <typename T>
List<T>::~List()
{
    std::free(m_Data);

    m_Data = nullptr;
    
    m_Size = 0;
    m_Capacity = 0;
}

template <typename T>
void List<T>::Reserve(const size_t capacity)
{
    if (m_Capacity >= capacity)
        return;
    
    m_Capacity = capacity;

    Realloc(m_Capacity);
}

template <typename T>
void List<T>::Clear()
{
}

template <typename T>
void List<T>::Add()
{
    CheckGrow(m_Size + 1);

    m_Data[m_Size] = T();
    m_Size++;
}

template <typename T>
void List<T>::Add(T& element)
{
    CheckGrow(m_Size + 1);

    m_Data[m_Size] = element;
    m_Size++;
}

template <typename T>
void List<T>::Add(T&& element)
{
    CheckGrow(m_Size + 1);

    m_Data[m_Size] = element;
    m_Size++;
}

template <typename T>
void List<T>::AddRange(const T* const data, const size_t number)
{
    CheckGrow(m_Size + number);

    std::memcpy(&m_Data[m_Size], data, number * sizeof(T));

    m_Size += number;
}

template <typename T>
void List<T>::AddRange(const std::initializer_list<T>& values)
{
    const size_t number = values.size();

    CheckGrow(m_Size + number);

    std::memcpy(&m_Data[m_Size], values.begin(), number * sizeof(T));

    m_Size += number;
}

template <typename T>
template <class... Args>
void List<T>::Emplace(Args&&... args)
{
}

template <typename T>
void List<T>::Insert(const T& elemnt, size_t index)
{
}

template <typename T>
void List<T>::Insert(const T&& elemnt, size_t index)
{
}

template <typename T>
void List<T>::Remove(const T& element)
{
}

template <typename T>
void List<T>::Remove(const T&& element)
{
}

template <typename T>
void List<T>::RemoveAt(const size_t index)
{
}

template <typename T>
bool List<T>::Contains(const T& element) const
{
    for (size_t i = 0; i < m_Size; i++)
    {
        if (m_Data[i] == element)
            return true;
    }

    return false;
}

template <typename T>
bool List<T>::Contains(const T&& element) const
{
    for (size_t i = 0; i < m_Size; i++)
    {
        if (m_Data[i] == element)
            return true;
    }

    return false;
}

template <typename T>
void List<T>::Iterate(std::function<void(T*, size_t)> lambda)
{
    for (size_t i = 0; i < m_Size; i++)
        lambda(&m_Data[i], i);
}

template <typename T>
bool List<T>::Exists(std::function<bool(const T*, size_t)> lambda)
{
    for (size_t i = 0; i < m_Size; i++)
    {
        if (lambda(&m_Data[i], i))
            return true;
    }

    return false;
}

template <typename T>
T* List<T>::Find(std::function<bool(const T*, size_t)> lambda)
{
    for (size_t i = 0; i < m_Size; i++)
    {
        if (lambda(&m_Data[i], i))
            return &m_Data[i];
    }

    return nullptr;
}

template <typename T>
bool List<T>::IsValid() const
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
    {
        throw std::invalid_argument("List subscript out of range");
    }

    return m_Data[index];
}

template <typename T>
const T& List<T>::operator[](const size_t index) const
{
    if (index >= m_Size)
    {
        throw std::invalid_argument("List subscript out of range");
    }

    return m_Data[index];
}

template <typename T>
void List<T>::Malloc(const size_t size)
{
    m_Data = static_cast<T*>(std::malloc(size * sizeof(T)));
}

template <typename T>
void List<T>::Calloc(const size_t size)
{
    m_Data = static_cast<T*>(std::calloc(size, sizeof(T)));
}

template <typename T>
void List<T>::Realloc(const size_t size)
{
    m_Data = static_cast<T*>(std::realloc(m_Data, size * sizeof(T)));
}

template <typename T>
void List<T>::CheckGrow(const size_t newSize)
{
    if (newSize > m_Capacity)
    {
        m_Capacity = std::bit_ceil(newSize);

        Realloc(m_Capacity);
    }
}

template <typename T>
void List<T>::CheckShrink(const size_t newSize)
{
    if (newSize < m_Capacity)
    {
        m_Capacity = std::bit_ceil(newSize);

        Realloc(m_Capacity);
    }
}

END_XNOR_CORE
