#pragma once

#include <functional>

#include "core.hpp"

/// @file list.hpp
/// @brief Defines the XnorCore::List class.

BEGIN_XNOR_CORE

/// @brief A dynamic array implementation.
/// 
/// ### Reasons
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

END_XNOR_CORE

#include "utils/list.inl"
