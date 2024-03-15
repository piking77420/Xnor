#pragma once

// ReSharper disable CppInconsistentNaming

#include <refl/refl.hpp>

#include "core.hpp"
#include "utils/meta_programming.hpp"

/// @file serializable.hpp
/// @brief Defines serialization/reflection types and concepts

BEGIN_XNOR_CORE

/// @brief refl::type_descriptor<T> shorthand
/// @tparam T Type
template <typename T>
using TypeDescriptor = refl::type_descriptor<T>;

/// @brief refl::attr::usage::type shorthand
///
/// Specifies that an attribute can only be used on a type
using FieldType = refl::attr::usage::type;

/// @brief refl::attr::usage::field shorthand
///
/// Specifies that an attribute can only be used on a field
using FieldAttribute = refl::attr::usage::field;

/// @brief refl::attr::usage::function shorthand
///
/// Specifies that an attribute can only be used on a function
using FunctionAttribute = refl::attr::usage::function;

/// @brief refl::attr::usage::member shorthand
///
/// Specifies that an attribute can only be used on a member (field and function)
using MemberAttribute = refl::attr::usage::member;



/// @brief Enables serialization for a class
///
/// A class that inherits from this class should implement either:
/// - REFLECTABLE_IMPL_H(ClassName) in the .hpp
/// - REFLECTABLE_IMPL_MINIMAL(ClassName) in the .hpp and REFLECTABLE_IMPL_CPP(ClassName) in the .cpp
class XNOR_ENGINE Serializable
{
public:
    Serializable() = default;
    
    virtual ~Serializable() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Serializable)

    /// @brief Serializes the object
    virtual void Serialize() const = 0;

    /// @brief Deserializes the object
    virtual void Deserialize() = 0;
};

/// @brief Concept that forces a type to be a child of Serializable
template <class T>
concept SerializableT = Meta::IsBaseOf<Serializable, T>;

/// @brief Implements the reflection in a .cpp file, it provides a body for Serializable::Serialize and Serializable::Deserialize
/// @param type Type name
#define REFLECTABLE_IMPL_CPP(type)                                                                                                      \
void type::Serialize() const                                                                                                            \
{                                                                                                                                       \
    Serializer::Serialize<type>(this, true);                                                                                            \
}                                                                                                                                       \
                                                                                                                                        \
void type::Deserialize()                                                                                                                \
{                                                                                                                                       \
    Serializer::Deserialize<type>(this);                                                                                                \
}

/// @brief Implements the reflection in a .hpp file, it provides a declaration for Serializable::Serialize and Serializable::Deserialize
/// @param type Type name
#define REFLECTABLE_IMPL_MINIMAL_DLL(type)                                                                                              \
public:                                                                                                                                 \
XNOR_ENGINE virtual void Serialize() const override;                                                                                    \
\
XNOR_ENGINE virtual void Deserialize() override;                                                                                        \
\
private:                                                                                                                                \
friend struct refl_impl::metadata::type_info__<type>;

/// @brief Implements the reflection in a .hpp file, it provides a declaration for Serializable::Serialize and Serializable::Deserialize
/// @param type Type name
#define REFLECTABLE_IMPL_MINIMAL(type)                                                                                                  \
public:                                                                                                                                 \
virtual void Serialize() const override;                                                                                                \
                                                                                                                                        \
virtual void Deserialize() override;                                                                                                    \
                                                                                                                                        \
private:                                                                                                                                \
friend struct refl_impl::metadata::type_info__<type>;

/// @brief Implements the reflection in a .hpp file, it provides a body for Serializable::Serialize and Serializable::Deserialize
/// @param type Type name
#define REFLECTABLE_IMPL_H_DLL(type)                                                                                                    \
public:                                                                                                                                 \
XNOR_ENGINE virtual void Serialize() const override                                                                                     \
{                                                                                                                                       \
    Serializer::Serialize<type>(this, true);                                                                                            \
}                                                                                                                                       \
                                                                                                                                        \
XNOR_ENGINE virtual void Deserialize() override                                                                                         \
{                                                                                                                                       \
    Serializer::Deserialize<type>(this);                                                                                                \
}                                                                                                                                       \
                                                                                                                                        \
/*
Even though the use of the friend functionality is prohibited in the C++ style guidelines of this project, it has to be used here to
circumvent the issue of accessing private members through reflection, the friend declaration allows the underlying code generated
by the reflection library to access private types.
There might be another solution to this, but it's the easiest one found so far, also it works and isn't too ugly
*/                                                                                                                                      \
private:                                                                                                                                \
friend struct refl_impl::metadata::type_info__<type>;

/// @brief Implements the reflection in a .hpp file, it provides a body for Serializable::Serialize and Serializable::Deserialize
/// @param type Type name
#define REFLECTABLE_IMPL_H(type)                                                                                                        \
public:                                                                                                                                 \
virtual void Serialize() const override                                                                                                 \
{                                                                                                                                       \
    Serializer::Serialize<type>(this, true);                                                                                            \
}                                                                                                                                       \
                                                                                                                                        \
virtual void Deserialize() override                                                                                                     \
{                                                                                                                                       \
    Serializer::Deserialize<type>(this);                                                                                                \
}                                                                                                                                       \
                                                                                                                                        \
/*
Even though the use of the friend functionality is prohibited in the C++ style guidelines of this project, it has to be used here to
circumvent the issue of accessing private members through reflection, the friend declaration allows the underlying code generated
by the reflection library to access private types.
There might be another solution to this, but it's the easiest one found so far, also it works and isn't too ugly
*/                                                                                                                                      \
private:                                                                                                                                \
friend struct refl_impl::metadata::type_info__<type>;


/// @brief Provides utility functions for reflection
namespace Reflection
{
    /// @brief Specifies that the field shouldn't be serialized
    struct NotSerializable : FieldAttribute
    {
    };

    /// @brief Specifies that the field shouldn't be serialized as a Guid, but that the values behind the pointer should be serialized instead
    struct ExpandPointer : FieldAttribute
    {
    };

    /// @brief Specifies that the field shouldn't be displayed in the inspector
    struct HideInInspector : FieldAttribute
    {
    };

    /// @brief Allows a boolean to be set to true when the field is modified
    /// @tparam T Parent type
    template <typename T>
    struct NotifyChange : FieldAttribute
    {
        /// @brief Shorthand for a class member pointer
        using PtrType = bool_t T::*;

        /// @brief Pointer to the boolean
        const PtrType pointer;

        /// @brief Creates a notify change attribute using a pointer to a boolean inside the concerned class
        /// @param ptr Boolean pointer in the class
        constexpr explicit NotifyChange(const PtrType ptr) : pointer(ptr) {}
    };

    /// @brief Allows an enum to be treated as a list of binary flags
    struct EnumFlags : FieldAttribute
    {
    };

    /// @brief Allows an integer or floating type to be bound between a minimum and a maximum value, it will display the field using a slider
    /// @tparam T Field type
    template <typename T>
    struct Range : FieldAttribute
    {
        static_assert(Meta::IsIntegralOrFloating<T>, "Range attribute can only be used on integer or floating values");

        /// @brief Minimum value
        const T minimum;
        /// @brief Maximum value
        const T maximum;

        /// @brief Creates a range
        constexpr explicit Range(const T& min, const T& max) : minimum(min), maximum(max) {}
    };

    /// @brief Allows a tooltip to be bound to a field
    struct Tooltip : FieldAttribute
    {
        /// @brief Tooltip text
        const char_t* const text;

        /// @brief Creates a tooltip from a string literal
        constexpr explicit Tooltip(const char_t* const t) : text(t) {}
    };
    
    /// @brief Gets the type info of a class
    /// @tparam ReflectT Type
    /// @return Type info
    template <typename ReflectT>
    static constexpr TypeDescriptor<ReflectT> GetTypeInfo();

    /// @brief Checks if a descriptor has a specified attribute
    /// @tparam DescriptorT Descriptor type
    /// @tparam AttributeT Attribute type
    /// @param descriptor Type descriptor
    /// @return Result
    template <typename AttributeT, typename DescriptorT>
    static constexpr bool_t HasAttribute(DescriptorT descriptor);

    /// @brief Gets the specified attribute of a descriptor
    /// @tparam DescriptorT Descriptor type
    /// @tparam AttributeT Attribute type
    /// @param descriptor Type descriptor
    /// @return Attribute
    template <typename AttributeT, typename DescriptorT>
    static constexpr const AttributeT& GetAttribute(DescriptorT descriptor);

    template <typename AttributeT, typename DescriptorT>
    static constexpr const AttributeT* TryGetAttribute(DescriptorT descriptor);
}

END_XNOR_CORE

#include "utils/serializable.inl"
