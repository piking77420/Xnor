#pragma once

#include "refl/refl.hpp"
#include "logger.hpp"
#include "core.hpp"
#include <string>
#include <unordered_map>
#include <typeinfo>

BEGIN_XNOR_CORE

class FieldInfo
{
public:
    std::string name;
    std::string typeName;
    size_t typeHash;
    size_t fullSize;
    size_t elementSize;
    size_t offset;
    bool isArray;
    bool isPointer;
    bool isStatic;
    bool isConst;
    
    [[nodiscard]]
    XNOR_ENGINE constexpr size_t GetArraySize() const
    {
        if (!isArray)
            return 0;

        return fullSize / elementSize;
    }
};

/// @brief Enables reflection for a class
///        A class that inherits from this class should implement the macro REFLECTABLE_IMPL(ClassName) in order to provide the ful implementation for the reflection
class Reflectable
{
public:
    virtual ~Reflectable() = default;
    
    XNOR_ENGINE virtual void CreateTypeInfo() const = 0;
};

template<class T>
concept ReflectT = std::is_base_of_v<Reflectable, T>;

#define REFLECTABLE_IMPL(type) \
public:\
virtual void CreateTypeInfo() const override \
{ \
    XnorCore::TypeInfo::Create<refl::trait::remove_qualifiers_t<decltype(*this)>>(); \
} \
/*
Even though the use of the friend functionality is prohibited in the C++ style guidelines of this project, it has to be used here to
circumvent the issue of accessing private members through reflection, the friend declaration allows the underlying code generated
by the reflection library to access private types.
There might be another solution to this, but it's the easiest one found so far, also it works and isn't too ugly
*/\
private:\
friend struct refl_impl::metadata::type_info__<type>;

class TypeInfo
{
public:
    /**
     * \brief Creates a type info from a class that inherits <c>XnorCore::Reflectable</c>
     * \tparam ReflectT Type
     */
    template <typename ReflectT>
    static constexpr void Create();

    /**
     * \brief Gets the type info of a class
     * \tparam ReflectT Type
     * \return Type info
     */
    template <typename ReflectT>
    static constexpr const TypeInfo& Get();

    /**
     * \brief Gets the type info of a type from a hash
     * \param typeHash Type hash
     * \return Type info
     */
    XNOR_ENGINE static const TypeInfo& Get(size_t typeHash);

private:
    /**
     * \brief Unordered map of the type info, the key is the hash value of the typeid
     */
    XNOR_ENGINE static inline std::unordered_map<size_t, TypeInfo> m_TypeInfo;

    /**
     * \brief Gets the offset of of class/struct member
     * \tparam T refl member type
     * \param member refl member
     * \return Offset
     */
    template <typename T>
    static size_t GetMemberOffset(const T member);

public:
    /**
     * \brief Gets the name of the type
     * \return Name
     */
    XNOR_ENGINE constexpr const std::string& GetName() const;
    
    XNOR_ENGINE constexpr const size_t GetSize() const;

    /**
     * \brief Gets the members of the type
     * \return Members
     */
    XNOR_ENGINE constexpr const std::vector<FieldInfo>& GetMembers() const;

    /**
     * \brief Gets the parents of the type
     * \return Members
     */
    XNOR_ENGINE constexpr const std::vector<size_t>& GetParents() const;

private:
    std::string m_Name;
    size_t m_Size;
    std::vector<FieldInfo> m_Members;
    std::vector<size_t> m_BaseClasses;

    /**
     * \brief Creates the type info from a type descriptor
     * \tparam ReflectT Type
     * \param desc refl Type descriptor
     */
    template <typename ReflectT>
    explicit constexpr TypeInfo(refl::type_descriptor<ReflectT> desc);

    /**
     * \brief Parses the members of a type from a type descriptor
     * \tparam ReflectT Type
     * \param desc refl Type descriptor
     */
    template <typename ReflectT>
    constexpr void ParseMembers(refl::type_descriptor<ReflectT> desc);

    /**
     * \brief Parses the parent classes of a type from a type descriptor
     * \tparam ReflectT Type
     * \param desc refl Type descriptor
     */
    template <typename ReflectT>
    constexpr void ParseParents(refl::type_descriptor<ReflectT> desc);
};

template <typename T>
size_t TypeInfo::GetMemberOffset(const T member)
{
    char buffer[50];
    (void) sprintf_s(buffer, sizeof(buffer), "%p", member.pointer);
    char* end = &buffer[sizeof(buffer)];
    return std::strtoll(buffer, &end, 16);
}

template <typename ReflectT>
constexpr void TypeInfo::Create()
{
    if (m_TypeInfo.contains(typeid(ReflectT).hash_code()))
    {
        // The type already exists, no need to parse it again
        // This should happen often due to inheritance from Entity (and probably other types)
        // Since the type info creation is called in begin, the call will go back up in the inheritance chain and call
        // the type info creation for every parent class, which results in it being called multiple times
        return;
    }
    
    // FIXME TODO constexpr TypeInfo ti(refl::reflect<ReflectT>());
    TypeInfo ti(refl::reflect<ReflectT>());
        
    m_TypeInfo.emplace(typeid(ReflectT).hash_code(), ti);
}

template <typename ReflectT>
constexpr const TypeInfo& TypeInfo::Get()
{
    return m_TypeInfo.at(typeid(ReflectT).hash_code());
}

template <typename ReflectT>
constexpr TypeInfo::TypeInfo(const refl::type_descriptor<ReflectT> desc)
{
    // Get type name
    m_Name = std::string(desc.name.c_str());
    m_Size = sizeof(ReflectT);

    ParseMembers(desc);
    ParseParents(desc);
}

template <typename ReflectT>
constexpr void TypeInfo::ParseMembers(refl::type_descriptor<ReflectT> desc)
{
    // Loop over type members (variables only)
    refl::util::for_each(desc.members, [&]<typename T>(const T member)
    {
        // Get offset of the member in the type
        const size_t offset = GetMemberOffset(member);

        // Get metadata about the member
        constexpr bool isArray = std::is_array<typename T::value_type>();
        constexpr bool isPointer = std::is_pointer<typename T::value_type>();
        constexpr bool isReflectable = std::is_base_of_v<Reflectable, typename T::value_type>;
        constexpr bool isStatic = member.is_static;
        // A member is const if it can't be written to
        constexpr bool isConst = !member.is_writable;
        constexpr size_t fullSize = sizeof(T::value_type);

        // Get names
        // Name of the variable
        // e.g., the name of 'float foo' is 'foo'
        const std::string name = std::string(member.name.c_str());
        // Name of the type of the variable
        // e.g., the type name of 'float foo' is 'float'
        const std::string typeName = std::string(typeid(T::value_type).name());
        
        size_t hash;
        size_t elementSize;
        if constexpr (isPointer)
        {
            // Get the hash code of the pointed type, instead of the hash code of the pointer type
            // e.g., if the variable is of type 'float*', we get the hash code of 'float' and not the hash code of 'float*'
            hash = typeid(std::remove_pointer_t<typename T::value_type>).hash_code();
            elementSize = sizeof(std::remove_pointer_t<typename T::value_type>);
        }
        else if constexpr (isArray)
        {
            // Same logic as above, get the hash code of the array elements, not of the array itself
            // e.g., if the variable is of type 'float[5]', we get the hash code of 'float' and not the hash code of 'float[5]'
            hash = typeid(std::remove_all_extents_t<typename T::value_type>).hash_code();
            elementSize = sizeof(std::remove_all_extents_t<typename T::value_type>);
        }
        else
        {
            // "Trivial" type, simply get the hash
            hash = typeid(T::value_type).hash_code();
            elementSize = fullSize;

            if constexpr (isReflectable)
            {
                // TODO call the CreateTypeInfo function directly here if possible
            }
        }

        // Construct field info
        const FieldInfo info = {
            .name = name,
            .typeName = typeName,
            .typeHash = hash,
            .fullSize = fullSize,
            .elementSize = elementSize,
            .offset = offset,
            .isArray = isArray,
            .isPointer = isPointer,
            .isStatic = isStatic,
            .isConst = isConst
        };

        // Add to members
        m_Members.push_back(info);
    });
}

template <typename ReflectT>
constexpr void TypeInfo::ParseParents(refl::type_descriptor<ReflectT> desc)
{
    if constexpr (desc.declared_bases.size == 0)
        return;

    refl::util::for_each(refl::util::reflect_types(desc.declared_bases), [&]<typename T>([[maybe_unused]] const T t)
    {
        // We store the type hash of the parent class, so we can get it back from the list of type info later 
        m_BaseClasses.push_back(typeid(T::type).hash_code());
    });
}

END_XNOR_CORE
