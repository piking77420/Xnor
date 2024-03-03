#pragma once

#include "core.hpp"
#include "reflectable.hpp"
#include "refl/refl.hpp"

BEGIN_XNOR_CORE

template <typename T>
using TypeDescriptor = refl::type_descriptor<T>;

using FieldAttribute = refl::attr::usage::field;
using FunctionAttribute = refl::attr::usage::function;
using MemberAttribute = refl::attr::usage::member;

struct NotSerializable : FieldAttribute
{
};

struct ExpandPointer : FieldAttribute
{
};

struct HideInInspector : FieldAttribute
{
};

/// @brief Enables reflection for a class
///        A class that inherits from this class should implement the macro REFLECTABLE_IMPL(ClassName) in order to provide the full implementation for the reflection
class XNOR_ENGINE Reflectable
{
public:
    Reflectable() = default;
    
    virtual ~Reflectable() = default;

    DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(Reflectable)
    
    virtual void CreateTypeInfo() const = 0;

    virtual void Serialize() const = 0;
    virtual void Deserialize() = 0;
};

template<class T>
concept ReflectT = std::is_base_of_v<Reflectable, T>;

#define REFLECTABLE_IMPL_CPP(type)                                                                                                        \
void type::CreateTypeInfo() const                                                                                                       \
{                                                                                                                                       \
    /**/                                                                                                                                \
}                                                                                                                                       \
                                                                                                                                        \
void type::Serialize() const                                                                                                            \
{                                                                                                                                       \
    Serializer::Serialize<type>(this, true);                                                                                            \
}                                                                                                                                       \
                                                                                                                                        \
void type::Deserialize()                                                                                                                \
{                                                                                                                                       \
    Serializer::Deserialize<type>(this);                                                                                                \
}

#define REFLECTABLE_IMPL_MINIMAL(type)                                                                                                  \
public:                                                                                                                                 \
virtual void CreateTypeInfo() const override;                                                                                           \
                                                                                                                                        \
virtual void Serialize() const override;                                                                                                \
                                                                                                                                        \
virtual void Deserialize() override;                                                                                                    \
                                                                                                                                        \
private:                                                                                                                                \
friend struct refl_impl::metadata::type_info__<type>;

#define REFLECTABLE_IMPL_H(type)                                                                                                        \
public:                                                                                                                                 \
virtual void CreateTypeInfo() const override                                                                                            \
{                                                                                                                                       \
    /* XnorCore::TypeInfo::Create<type>(); */                                                                                           \
}                                                                                                                                       \
                                                                                                                                        \
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

class TypeInfo
{
public:
    /**
     * @brief Gets the type info of a class
     * @tparam ReflectT Type
     * @return Type info
     */
    template <typename ReflectT>
    static constexpr TypeDescriptor<ReflectT> Get();
};

template <typename ReflectT>
constexpr TypeDescriptor<ReflectT> TypeInfo::Get()
{
    return refl::reflect<ReflectT>();
}

END_XNOR_CORE
