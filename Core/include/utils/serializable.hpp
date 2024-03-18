#pragma once

// ReSharper disable CppInconsistentNaming

#include <refl/refl.hpp>

#include "core.hpp"
#include "utils/meta_programming.hpp"

/// @file serializable.hpp
/// @brief Defines serialization/reflection types and concepts

BEGIN_XNOR_CORE

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
#define REFLECTABLE_IMPL_MINIMAL_DLL_PRIVATE_CTOR(type)                                                                                 \
private:                                                                                                                                \
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

END_XNOR_CORE
