#pragma once

// ReSharper disable CppInconsistentNaming

#include "core.hpp"
#include "utils/meta_programming.hpp"
#include "refl/refl.hpp"

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

/// @brief Concept that forces a type to be a child of @ref Reflectable
template <class T>
concept ReflectT = Meta::IsBaseOf<Serializable, T>;

/// @brief Implements the reflection in a .cpp file, it provides a body for @ref Serialize and @ref Deserialize
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

/// @brief Implements the reflection in a .hpp file, it provides a declaration for @ref Serialize and @ref Deserialize
/// @param type Type name
#define REFLECTABLE_IMPL_MINIMAL(type)                                                                                                  \
public:                                                                                                                                 \
virtual void Serialize() const override;                                                                                                \
                                                                                                                                        \
virtual void Deserialize() override;                                                                                                    \
                                                                                                                                        \
private:                                                                                                                                \
friend struct refl_impl::metadata::type_info__<type>;

/// @brief Implements the reflection in a .hpp file, it provides a body for @ref Serialize and @ref Deserialize
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
    static constexpr AttributeT GetAttribute(DescriptorT descriptor);
}

template <typename ReflectT>
constexpr TypeDescriptor<ReflectT> Reflection::GetTypeInfo()
{
    return refl::reflect<ReflectT>();
}

template <typename AttributeT, typename DescriptorT>
constexpr bool_t Reflection::HasAttribute(const DescriptorT descriptor)
{
    return refl::descriptor::has_attribute<AttributeT>(descriptor);
}

template <typename AttributeT, typename DescriptorT>
constexpr AttributeT Reflection::GetAttribute(const DescriptorT descriptor)
{
    return refl::descriptor::get_attribute<AttributeT>(descriptor);
}

END_XNOR_CORE
