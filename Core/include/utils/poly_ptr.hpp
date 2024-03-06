#pragma once

#include "utils/utils.hpp"

/// @file poly_ptr.hpp
/// @brief Defines the PolyPtr class.

BEGIN_XNOR_CORE

/// @brief PolyPtr is an abstraction for polymorphic pointers (hence the name), the idea is that it allows the same behavior of typeid(ptr)
/// even with a class that isn't pure virtual
/// 
/// This was needed for reflection, because the @ref Component class isn't pure virtual, but we use a @ref List of @ref Component pointers in @ref Entity.
///
/// The PolyPtr allows us to get the needed information for reflection without too much overhead 
/// 
/// @tparam T Base type
template <typename T>
class PolyPtr
{
public:
    /// @brief The type of the PolyPtr<T>, refers to T
    using Type = T;

    PolyPtr();
    ~PolyPtr();

    DEFAULT_COPY_MOVE_OPERATIONS(PolyPtr)

    /// @brief Creates a PolyPtr with the child type of @c U
    /// @tparam U Child type
    template <typename U>
    void Create();

    /// @brief Creates a PolyPtr with the child type of @c U
    /// @tparam U Child type
    /// @param value Pointer value
    template <typename U>
    void Create(U* value);

    /// @brief *ptr operator 
    /// @return Reference to the underlying value
    T& operator*();
    
    /// @brief *ptr operator 
    /// @return Const reference to the underlying value
    const T& operator*() const;

    /// @brief ptr-> operator
    /// @return Returns the underlying pointer
    T* operator->();
    
    /// @brief ptr-> operator
    /// @return Returns the underlying pointer
    const T* operator->() const;

    /// @brief Casts the pointer to a specified child type (no runtime check is performed)
    /// @tparam U Child type
    /// @return Converted pointer
    template <typename U>
    [[nodiscard]]
    U* Cast();

    /// @brief Casts the pointer to a specified child type (no runtime check is performed)
    /// @tparam U Child type
    /// @return Converted pointer
    template <typename U>
    [[nodiscard]]
    const U* Cast() const;

    /// @brief Casts the pointer to @c void*
    /// @return Void* pointer representation
    [[nodiscard]]
    void* AsVoid();

    /// @brief Casts the pointer to @c void*
    /// @return Void* pointer representation
    [[nodiscard]]
    const void* AsVoid() const;

    /// @brief Checks if the underlying pointer is of a certain type
    /// @tparam U Child type
    /// @return Is of type
    template <typename U>
    [[nodiscard]]
    bool_t IsOfType() const;

    /// @brief Gets the hash of the underlying pointer type
    /// @return Hash
    [[nodiscard]]
    size_t GetHash() const;

private:
    void Destroy();
    
    T* m_Ptr;
    size_t m_Hash;
};

END_XNOR_CORE

#include "utils/poly_ptr.inl"
