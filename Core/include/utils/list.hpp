#pragma once

#include <functional>
#include <stdexcept>

#include "core.hpp"

/// @file list.hpp
/// @brief Defines the List class.

BEGIN_XNOR_CORE

/// @brief A dynamic array implementation.
/// 
/// @paragraph reasons Reasons
/// A more user friendly list than @c std::vector, based on how @c %List is done in C#
/// The internal structure and workings are similar to how std::vector works, it uses a capacity that grows exponentially based on powers of 2
/// 
/// @tparam T Type stored
///
/// @see <a href="https://en.cppreference.com/w/cpp/container/vector">std::vector</a>
/// @see <a href="https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1?view=net-8.0">C# List</a>
template <typename T>
class List
{    
public:
    static_assert(std::is_default_constructible_v<T>, "T must have a default constructor");
    static_assert(std::is_copy_constructible_v<T>, "T must have a copy constructor");

    /// @brief The type of the List<T>, refers to T
    using Type = T;

    DELETE_COPY_MOVE_OPERATIONS(List)

    /// @brief Creates an empty list with a capacity of 0
    explicit List();

    /// @brief Creates a list with the specified size, and fills it with the default value of T
    /// 
    /// @param size List size
    explicit List(size_t size);

    /// @brief Creates a list with the specified size, and fills it with the provided value
    /// 
    /// @param size List size
    /// @param defaultValue Default value
    explicit List(size_t size, const T& defaultValue);

    /// @brief Creates a list with the specified size, and fills it with the provided values
    /// 
    /// @param size List size
    /// @param values Provided values
    explicit List(size_t size, const T values[]);

    /// @brief Creates a list with the specified values
    /// 
    /// @param values Values
    List(const std::initializer_list<T>& values);

    /// @brief Destroys the list
    ~List();

    /// @brief Reserves a specified amount of elements in the list
    /// 
    /// @param capacity New capacity
    void Reserve(size_t capacity);

    /// @brief Clears the list
    void Clear();

    /// @brief Adds a default element to the end of the list (calls the default constructor of T)
    void Add();

    /// @brief Adds a specified element to the end of the list
    /// 
    /// @param element Element
    void Add(const T& element);
    
    /// @brief Adds a specified element to the end of the list
    /// 
    /// @param element Element
    void Add(T&& element);

    /// @brief Adds a range of elements to the end of the list
    /// 
    /// @param data Data
    /// @param number Number of elements (array size of data)
    void AddRange(const T* data, size_t number);

    /// @brief Adds a range of elements to the end of the list
    /// 
    /// @param values Values
    void AddRange(const std::initializer_list<T>& values);

    /// @brief Adds a zeroed out element to the list  
    void AddZeroed();

    /// @brief Fills the list with a specified value
    /// 
    /// @param value Value
    void Fill(const T& value);

    /// @brief Fills the list with a specified value
    /// 
    /// @param value Value
    void Fill(T&& value);

    /// @brief Constructs a new element and adds it to the end of the list
    /// 
    /// @tparam Args Constructor element types
    /// @param args Arguments
    template <class... Args>
    void Emplace(Args&&... args);

    /**
     * @brief Inserts an element at the given position using the default constructor
     * 
     * @param index Index
     */
    void Insert(size_t index);

    /// @brief Inserts an element in the list at the given position
    /// 
    /// @param element Element
    /// @param index Index
    void Insert(const T& element, size_t index);

    /// @brief Inserts an element in the list at the given position
    /// 
    /// @param element Element
    /// @param index Index
    void Insert(T&& element, size_t index);

    /// @brief Inserts an zeroed element in the list at the given position
    /// 
    /// @param index Index
    void InsertZeroed(size_t index);

    /// @brief Removes an element from the list (only removes the first occurence it finds)
    /// 
    /// @param element Element
    void Remove(const T& element);
    
    /// @brief Removes an element from the list at a given index
    /// 
    /// @param index Index
    void RemoveAt(size_t index);

    /// @brief Removes a range of elements from the list
    /// 
    /// @param start Starting index
    /// @param end End index
    void RemoveRange(size_t start, size_t end);

    /// @brief Checks if the list contains a specified element
    /// 
    /// @param element Element
    /// @return Element exists
    [[nodiscard]]
    bool_t Contains(const T& element) const;

    /// @brief Allows iteration over the list with a lambda
    /// 
    /// <p>The lambda returns void, and has a pointer to the current element and its index as parameters</p>
    /// 
    /// @param lambda Function lambda
    void Iterate(const std::function<void(T*, size_t)>& lambda);

    /// @brief Checks if an element exists that fulfills the requirements provided in a lambda
    /// 
    /// <p>The lambda returns bool_t, and has a pointer to the current element and its index as parameters</p>
    /// 
    /// @param lambda Function lambda
    /// @return Element exists
    bool_t Exists(const std::function<bool_t(const T*, size_t)>& lambda) const;

    /// @brief Tries to find an element that fulfills the requirements provided in a lambda
    /// 
    /// <p>The lambda returns bool_t, and has a pointer to the current element and its index as parameters</p>
    /// 
    /// @param lambda Function lambda
    /// @return Pointer to element
    T* Find(const std::function<bool_t(const T*, size_t)>& lambda);

    /// @brief Checks if the list if valid.
    /// 
    /// A list is valid if it meets these requirements:
    /// <p>- The internal pointer mustn't be nullptr</p>
    /// <p>- Capacity mustn't be 0</p>
    /// <p>- The size mustn't exceed the capacity</p>
    /// 
    /// @return Is valid
    [[nodiscard]]
    bool_t IsValid() const;

    /// @brief Gets the underlying pointer to the list
    /// 
    /// @return Pointer
    [[nodiscard]]
    T* GetData() const;

    /// @brief Gets the size of the list
    /// 
    /// @return Size
    [[nodiscard]]
    size_t GetSize() const;

    /// @brief Gets the capacity of the list
    /// 
    /// @return Capacity
    [[nodiscard]]
    size_t GetCapacity() const;

    /// @brief Gets the type size of T
    /// 
    /// @return Type size
    [[nodiscard]]
    size_t GetTypeSize() const;

    /// @brief Gets an element of the list at a specified index
    /// 
    /// @param index Index
    /// @return Element
    /// 
    /// @throw invalid_argument If index >= list size
    T& operator[](size_t index);
    
    /// @brief Gets an element of the list at a specified index
    /// 
    /// @param index Index
    /// @return Element
    /// 
    /// @throw invalid_argument If index >= list size
    const T& operator[](size_t index) const;

#ifndef UNIT_TEST
private:
#endif
    T* m_Data;
    size_t m_Size;
    size_t m_Capacity;
    size_t m_TypeSize;

    /// @brief Performs a malloc on the data
    /// @param size Size
    void Malloc(size_t size);
    
    /// @brief Performs a calloc on the data
    /// @param size Size
    void Calloc(size_t size);

    /// @brief Performs a realloc on the data
    /// @param size Size
    void Realloc(size_t size);

    /// @brief Checks if the list should grow
    /// @param newSize New size
    void CheckGrow(size_t newSize);
    
    /// @brief Checks if the list should shrink
    /// @param newSize New size
    void CheckShrink(size_t newSize);

    /// @brief Gets the address to the nth element of the list
    /// @param index Index
    /// @return Pointer to nth element
    [[nodiscard]]
    T* Access(size_t index);

    /// @brief Gets the address to the nth element of the list
    /// @param index Index
    /// @return Pointer to nth element
    [[nodiscard]]
    const T* Access(size_t index) const;
};

template <typename T>
List<T>::List()
    : m_Size(0), m_TypeSize(sizeof(T))
{
    m_Capacity = 1;

    Calloc(m_Capacity);
}

template <typename T>
List<T>::List(const size_t size)
    : m_Size(size), m_TypeSize(sizeof(T))
{
    m_Capacity = std::bit_ceil(m_Size);
    
    Malloc(m_Capacity);

    for (size_t i = 0; i < m_Size; i++)
        m_Data[i] = T();
}

template <typename T>
List<T>::List(const size_t size, const T& defaultValue)
    : m_Size(size), m_TypeSize(sizeof(T))
{
    m_Capacity = std::bit_ceil(m_Size);

    Malloc(m_Capacity);
    
    for (size_t i = 0; i < m_Size; i++)
        m_Data[i] = defaultValue;
}

template <typename T>
List<T>::List(const size_t size, const T values[])
    : m_Size(size), m_TypeSize(sizeof(T))
{
    m_Capacity = std::bit_ceil(m_Size);
    
    Malloc(m_Capacity);

    for (size_t i = 0; i < m_Size; i++)
        m_Data[i] = values[i];
}

template <typename T>
List<T>::List(const std::initializer_list<T>& values)
    : m_TypeSize(sizeof(T))
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
    Clear();
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
    CheckGrow(m_Size + 1);

    m_Data[m_Size] = T();
    m_Size++;
}

template <typename T>
void List<T>::Add(const T& element)
{
    CheckGrow(m_Size + 1);

    m_Data[m_Size] = element;
    m_Size++;
}

template <typename T>
void List<T>::Add(T&& element)
{
    CheckGrow(m_Size + 1);

    m_Data[m_Size] = std::move(element);
    m_Size++;
}

template <typename T>
void List<T>::AddRange(const T* const data, const size_t number)
{
    CheckGrow(m_Size + number);

    std::memcpy(&m_Data[m_Size], data, number * m_TypeSize);

    m_Size += number;
}

template <typename T>
void List<T>::AddRange(const std::initializer_list<T>& values)
{
    const size_t number = values.size();

    CheckGrow(m_Size + number);

    std::memcpy(&m_Data[m_Size], values.begin(), number * m_TypeSize);

    m_Size += number;
}

template <typename T>
void List<T>::AddZeroed()
{
    CheckGrow(m_Size + 1);

    std::memset(Access(m_Size), 0, m_TypeSize);
    m_Size++;
}

template <typename T>
void List<T>::Fill(const T& value)
{
    for (size_t i = 0; i < m_Size; i++)
        m_Data[i] = value;
}

template <typename T>
void List<T>::Fill(T&& value)
{
    for (size_t i = 0; i < m_Size; i++)
        m_Data[i] = std::move(value);
}

template <typename T>
template <class... Args>
void List<T>::Emplace(Args&&... args) // NOLINT(cppcoreguidelines-missing-std-forward)
{
    CheckGrow(m_Size + 1);

    m_Data[m_Size] = T(std::forward<Args>(args)...);
    m_Size++;
}

template <typename T>
void List<T>::Insert(size_t index)
{
    if (index >= m_Size)
        throw std::invalid_argument("List insert index out of range");

    CheckGrow(m_Size + 1);

    std::memcpy(&m_Data[index + 1], &m_Data[index], (m_Size - index) * m_TypeSize);
    m_Data[index] = T();
    
    m_Size++;
}

template <typename T>
void List<T>::Insert(const T& element, const size_t index)
{
    if (index >= m_Size)
        throw std::invalid_argument("List insert index out of range");

    CheckGrow(m_Size + 1);

    std::memcpy(&m_Data[index + 1], &m_Data[index], (m_Size - index) * m_TypeSize);
    m_Data[index] = element;
    
    m_Size++;
}

template <typename T>
void List<T>::Insert(T&& element, const size_t index)
{
    if (index >= m_Size)
        throw std::invalid_argument("List insert index out of range");

    CheckGrow(m_Size + 1);

    std::memcpy(&m_Data[index + 1], &m_Data[index], (m_Size - index) * m_TypeSize);
    m_Data[index] = std::move(element);
    
    m_Size++;
}

template <typename T>
void List<T>::InsertZeroed(const size_t index)
{
    if (index >= m_Size)
        throw std::invalid_argument("List insert index out of range");

    CheckGrow(m_Size + 1);

    std::memcpy(Access(index + 1), Access(index), (m_Size - index) * m_TypeSize);
    std::memset(Access(index), 0, m_TypeSize);
    
    m_Size++;
}

template <typename T>
void List<T>::Remove(const T& element)
{
    size_t i;
    for (i = 0; i < m_Size; i++)
    {
        if (m_Data[i] == element)
            break;
    }

    if (i == m_Size)
        return;

    m_Data[i].~T();
    std::memcpy(&m_Data[i], &m_Data[i + 1], (m_Size - i - 1) * m_TypeSize);

    CheckShrink(m_Size - 1);
    m_Size--;
}

template <typename T>
void List<T>::RemoveAt(const size_t index)
{
    if (index >= m_Size)
        throw std::invalid_argument("List remove at subscript out of range");

    m_Data[index].~T();
    std::memcpy(Access(index), Access(index + 1), (m_Size - index - 1) * m_TypeSize);

    CheckShrink(m_Size - 1);
    m_Size--;
}

template <typename T>
void List<T>::RemoveRange(const size_t start, const size_t end)
{
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
    
    std::memcpy(Access(start), Access(end + 1), (m_Size - end - 1) * m_TypeSize);

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
size_t List<T>::GetTypeSize() const
{
    return m_TypeSize;
}

template <typename T>
T& List<T>::operator[](const size_t index)
{
    if (index >= m_Size)
        throw std::invalid_argument("List subscript out of range");

    return *Access(index);
}

template <typename T>
const T& List<T>::operator[](const size_t index) const
{
    if (index >= m_Size)
        throw std::invalid_argument("List subscript out of range");

    return *Access(index);
}

template <typename T>
void List<T>::Malloc(const size_t size)
{
    m_Data = static_cast<T*>(std::malloc(size * m_TypeSize));
}

template <typename T>
void List<T>::Calloc(const size_t size)
{
    m_Data = static_cast<T*>(std::calloc(size, m_TypeSize));
}

template <typename T>
void List<T>::Realloc(const size_t size)
{
    m_Data = static_cast<T*>(std::realloc(m_Data, size * m_TypeSize));
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

template <typename T>
T* List<T>::Access(const size_t index)
{
    return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(m_Data) + index * m_TypeSize);
}

template <typename T>
const T* List<T>::Access(const size_t index) const
{
    return reinterpret_cast<const T*>(reinterpret_cast<const uint8_t*>(m_Data) + index * m_TypeSize);
}

END_XNOR_CORE
