// ReSharper disable CppClangTidyCppcoreguidelinesRvalueReferenceParamNotMoved
// ReSharper disable CppClangTidyCppcoreguidelinesMissingStdForward

#pragma once

#include <ranges>

BEGIN_XNOR_CORE

template <typename T>
List<T>::List(const size_t size)
    : m_Vector(size)
{
}

template <typename T>
List<T>::List(const size_t size, const T& defaultValue)
    : m_Vector(size, defaultValue)
{
}

template <typename T>
List<T>::List(const size_t size, const T* const values)
    : m_Vector(size)
{
    for (size_t i = 0; i < size; i++)
        m_Vector[i] = values[i];
}

template <typename T>
List<T>::List(const std::initializer_list<T>& values)
    : m_Vector(values)
{
}

template <typename T>
void List<T>::Resize(const size_t size)
{
    m_Vector.resize(size);
}

template <typename T>
void List<T>::Clear()
{
    m_Vector.clear();
}

template <typename T>
void List<T>::Add()
{
    m_Vector.emplace_back();
}

template <typename T>
void List<T>::Add(const T& element)
{
    m_Vector.emplace_back(element);
}

template <typename T>
void List<T>::Add(T&& element)
{
    m_Vector.emplace_back(std::forward<T>(element));
}

template <typename T>
void List<T>::AddRange(const T* const data, const size_t count)
{
    for (size_t i = 0; i < count; i++)
        m_Vector.emplace_back(data[i]);
}

template <typename T>
void List<T>::AddRange(const std::initializer_list<T>& values)
{
    const size_t size = values.size();
    const T* const begin = values.begin();
    
    for (size_t i = 0; i < size; i++)
        m_Vector.emplace_back(begin[i]);
}

template <typename T>
void List<T>::Fill(const T& value)
{
    for (size_t i = 0; i < m_Vector.size(); i++)
        m_Vector[i] = value;
}

template <typename T>
void List<T>::Fill(T&& value)
{
    for (size_t i = 0; i < m_Vector.size(); i++)
        m_Vector[i] = value;
}

template <typename T>
template <class... Args>
void List<T>::Emplace(Args&&... args)
{
    m_Vector.emplace_back(std::forward<Args>(args)...);
}

template <typename T>
void List<T>::Insert(size_t index)
{
    m_Vector.insert(m_Vector.begin() + index, T());
}

template <typename T>
void List<T>::Insert(const T& element, const size_t index)
{
    m_Vector.insert(m_Vector.begin() + index, element);
}

template <typename T>
void List<T>::Insert(T&& element, const size_t index)
{
    m_Vector.insert(m_Vector.begin() + index, std::forward<T>(element));
}

template <typename T>
void List<T>::Remove(const T& element)
{
    Iterator position = std::find(m_Vector.begin(), m_Vector.end(), element);

    if (position != m_Vector.end())
        m_Vector.erase(position);
}

template <typename T>
void List<T>::RemoveAt(const size_t index)
{
    m_Vector.erase(m_Vector.begin() + index);
}

template <typename T>
void List<T>::RemoveRange(const size_t start, const size_t end)
{
    m_Vector.erase(m_Vector.begin() + start, m_Vector.begin() + end);
}

template <typename T>
bool_t List<T>::Contains(const T& element) const
{
    return std::ranges::find(m_Vector, element) != m_Vector.end();
}

template <typename T>
void List<T>::Iterate(const std::function<void(T*)>& lambda)
{
    for (size_t i = 0; i < m_Vector.size(); i++)
        lambda(&m_Vector[i]);
}

template <typename T>
void List<T>::Iterate(const std::function<void(const T*)>& lambda) const
{
    for (size_t i = 0; i < m_Vector.size(); i++)
        lambda(&m_Vector[i]);
}

template <typename T>
bool_t List<T>::Exists(const std::function<bool_t(const T*)>& lambda) const
{
    for (size_t i = 0; i < m_Vector.size(); i++)
    {
        if (lambda(&m_Vector[i]))
            return true;
    }

    return false;
}

template <typename T>
T* List<T>::Find(const std::function<bool_t(const T*)>& lambda)
{
    for (size_t i = 0; i < m_Vector.size(); i++)
    {
        if (lambda(&m_Vector[i]))
            return &m_Vector[i];
    }

    return nullptr;
}

template <typename T>
T* List<T>::GetData()
{
    return m_Vector.data();
}

template <typename T>
const T* List<T>::GetData() const
{
    return m_Vector.data();
}

template <typename T>
size_t List<T>::GetSize() const
{
    return m_Vector.size();
}

template <typename T>
size_t List<T>::GetCapacity() const
{
    return m_Vector.capacity();
}

template <typename T>
T& List<T>::operator[](const size_t index)
{
    return m_Vector[index];
}

template <typename T>
const T& List<T>::operator[](const size_t index) const
{
    return m_Vector[index];
}

END_XNOR_CORE
