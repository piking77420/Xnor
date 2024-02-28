#pragma once

#include <string>
#include <typeinfo>
#include <unordered_map>
#include "core.hpp"
#include "logger.hpp"
#include "reflectable.hpp"
#include "refl/refl.hpp"

#include "meta_programming.hpp"
#include "utils.hpp"

BEGIN_XNOR_CORE

using MemberAttribute = refl::attr::usage::member;
template <typename T>
using TypeDescriptor = refl::type_descriptor<T>;

template<std::size_t I = 0, typename... Tp>
std::enable_if_t<I == sizeof...(Tp), void>
GetType(std::vector<MemberAttribute>&, const std::tuple<Tp...>&)
{ }

template<std::size_t I = 0, typename... Tp>
std::enable_if_t<I < sizeof...(Tp), void>
GetType(std::vector<MemberAttribute>& v, const std::tuple<Tp...>& t)
{
    using Type = std::remove_reference_t<decltype(std::get<I>(t))>;
    v.push_back(Type());
    GetType<I + 1, Tp...>(v, t);
}

struct NotSerializable : MemberAttribute
{
};

struct ExpandPointer : MemberAttribute
{
};

struct HideInInspector : MemberAttribute
{
};

class FieldInfo
{
public:
    std::string name;
    std::string typeName;
    size_t typeHash;
    size_t fullSize;
    size_t elementSize;
    size_t offset;
    bool_t isNativeType;
    bool_t isMathType;
    bool_t isArray;
    bool_t isList;
    bool_t isPointer;
    bool_t isPolyPointer;
    bool_t isXnorPointer;
    bool_t isStatic;
    bool_t isConst;
    std::vector<MemberAttribute> attributes;

    [[nodiscard]]
    XNOR_ENGINE constexpr size_t GetArraySize() const
    {
        if (!isArray)
            return 0;

        return fullSize / elementSize;
    }

    template <typename T>
    [[nodiscard]]
    bool HasAttribute() const
    {
        return std::ranges::find(attributes, T()) != attributes.end();
    }
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

#define REFLECTABLE_IMPL(type)                                                                                                          \
public:                                                                                                                                 \
virtual void CreateTypeInfo() const override                                                                                            \
{                                                                                                                                       \
    XnorCore::TypeInfo::Create<std::remove_cvref_t<decltype(*this)>>();                                                                 \
}                                                                                                                                       \
                                                                                                                                        \
virtual void Serialize() const override                                                                                                 \
{                                                                                                                                       \
    Serializer::Serialize<std::remove_cvref_t<decltype(*this)>>(this);                                                                  \
}                                                                                                                                       \
                                                                                                                                        \
virtual void Deserialize() override                                                                                                     \
{                                                                                                                                       \
    Serializer::Deserialize<std::remove_cvref_t<decltype(*this)>>(this);                                                                \
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
     * @brief Creates a type info from a class that inherits <c>XnorCore::Reflectable</c>
     * @tparam ReflectT Type
     */
    template <typename ReflectT>
    static constexpr void Create();

    /**
     * @brief Gets the type info of a class
     * @tparam ReflectT Type
     * @return Type info
     */
    template <typename ReflectT>
    static constexpr TypeDescriptor<ReflectT> Get();

    /**
     * @brief Gets the type info of a type from a hash
     * @param typeHash Type hash
     * @return Type info
     */
    XNOR_ENGINE static const TypeInfo& Get(size_t typeHash);

    XNOR_ENGINE static bool Contains(size_t typeHash);

private:
    /**
     * @brief Unordered map of the type info, the key is the hash value of the typeid
     */
    XNOR_ENGINE static inline std::unordered_map<size_t, TypeInfo> m_TypeInfo;

    /**
     * @brief Gets the offset of of class/struct member
     * @tparam T refl member type
     * @param member refl member
     * @return Offset
     */
    template <typename T>
    static size_t GetMemberOffset(T member);

    /// @brief Parses the attributes of a member
    /// @tparam T refl member type
    /// @param member refl member
    /// @param info Field info
    template <typename T>
    static void ParseMemberAttributes(T member, FieldInfo& info);

public:
    /**
     * @brief Gets the name of the type
     * @return Name
     */
    XNOR_ENGINE constexpr const std::string& GetName() const;
    
    XNOR_ENGINE constexpr size_t GetSize() const;

    /**
     * @brief Gets the members of the type
     * @return Members
     */
    XNOR_ENGINE constexpr const std::vector<FieldInfo>& GetMembers() const;

    /**
     * @brief Gets the parents of the type
     * @return Members
     */
    XNOR_ENGINE constexpr const std::vector<size_t>& GetParents() const;

private:
    std::string m_Name;
    size_t m_Size;
    std::vector<FieldInfo> m_Members;
    std::vector<size_t> m_BaseClasses;

    /**
     * @brief Creates the type info from a type descriptor
     * @tparam ReflectT Type
     * @param desc refl Type descriptor
     */
    template <typename ReflectT>
    explicit constexpr TypeInfo(TypeDescriptor<ReflectT> desc);

    /**
     * @brief Parses the members of a type from a type descriptor
     * @tparam ReflectT Type
     * @param desc refl Type descriptor
     */
    template <typename ReflectT>
    constexpr void ParseMembers(TypeDescriptor<ReflectT> desc);

    /**
     * @brief Parses the parent classes of a type from a type descriptor
     * @tparam ReflectT Type
     * @param desc refl Type descriptor
     */
    template <typename ReflectT>
    constexpr void ParseParents(TypeDescriptor<ReflectT> desc);
};

template <typename T>
size_t TypeInfo::GetMemberOffset(const T member)
{
    // TODO fix this awful hack
    char buffer[50];
    (void) sprintf_s(buffer, sizeof(buffer), "%p", member.pointer);
    char* end = &buffer[sizeof(buffer)];
    return std::strtoll(buffer, &end, 16);
}

template <typename T>
void TypeInfo::ParseMemberAttributes(T member, FieldInfo& info)
{
    GetType(info.attributes, member.attributes);
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
    
    TypeInfo ti(refl::reflect<ReflectT>());

    m_TypeInfo.emplace(typeid(ReflectT).hash_code(), ti);
}

template <typename ReflectT>
constexpr TypeDescriptor<ReflectT> TypeInfo::Get()
{
    return refl::reflect<ReflectT>();
}

template <typename ReflectT>
constexpr TypeInfo::TypeInfo(const TypeDescriptor<ReflectT> desc)
{
    // Get type name
    m_Name = std::string(desc.name.c_str());
    Logger::LogDebug("Creating type info for : {}", m_Name);
    m_Size = sizeof(ReflectT);

    ParseMembers(desc);
    ParseParents(desc);
}

template <typename ReflectT>
constexpr void TypeInfo::ParseMembers(TypeDescriptor<ReflectT> desc)
{
    // Loop over type members (variables only)
    refl::util::for_each(desc.members, [&]<typename T>(const T member)
    {
        // Get offset of the member in the type
        const size_t offset = GetMemberOffset(member);

        // Get metadata about the member
        constexpr bool_t isArray = std::is_array_v<typename T::value_type>;
        constexpr bool_t isPointer = std::is_pointer_v<typename T::value_type>;
        bool_t isPolyPointer = Meta::IsPolyPtr<typename T::value_type>;
        bool_t isNativeType = Meta::IsNativeType<typename T::value_type>;
        bool_t isMathType = Meta::IsMathType<typename T::value_type>;
        bool_t isXnorPointer = Meta::IsXnorPointer<typename T::value_type>;
        constexpr bool_t isReflectable = std::is_base_of_v<Reflectable, typename T::value_type>;
        constexpr bool_t isStatic = member.is_static;
        constexpr bool_t isList = Meta::IsXnorList<typename T::value_type>;

        // A member is const if it can't be written to
        constexpr bool_t isConst = !member.is_writable;
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
            using ArrayT = std::remove_all_extents_t<typename T::value_type>;
            hash = typeid(ArrayT).hash_code();
            elementSize = sizeof(ArrayT);
            isPolyPointer = Meta::IsPolyPtr<ArrayT>;
            isXnorPointer = Meta::IsXnorPointer<ArrayT>;
            isNativeType = Meta::IsNativeType<ArrayT>;
            isMathType = Meta::IsNativeType<ArrayT>;
        }
        else if constexpr (isList)
        {
            using ListT = typename T::value_type::Type;
            hash = typeid(ListT).hash_code();
            elementSize = sizeof(ListT);
            isPolyPointer = Meta::IsPolyPtr<ListT>;
            isXnorPointer = Meta::IsXnorPointer<ListT>;
            isNativeType = Meta::IsNativeType<ListT>;
            isMathType = Meta::IsNativeType<ListT>;
        }
        else if constexpr (Meta::IsXnorPointer<typename T::value_type>)
        {
            using XnorPointerT = typename T::value_type::Type;
            hash = typeid(XnorPointerT).hash_code();
            elementSize = sizeof(XnorPointerT);
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
        FieldInfo info = {
            .name = name,
            .typeName = typeName,
            .typeHash = hash,
            .fullSize = fullSize,
            .elementSize = elementSize,
            .offset = offset,
            .isNativeType = isNativeType,
            .isMathType = isMathType,
            .isArray = isArray,
            .isList = isList,
            .isPointer = isPointer,
            .isPolyPointer = isPolyPointer,
            .isXnorPointer = isXnorPointer,
            .isStatic = isStatic,
            .isConst = isConst
        };

        ParseMemberAttributes(member, info);

        // Add to members
        m_Members.push_back(info);
    });
}

template <typename ReflectT>
constexpr void TypeInfo::ParseParents(TypeDescriptor<ReflectT> desc)
{
    if constexpr (desc.declared_bases.size != 0)
    {
        refl::util::for_each(refl::util::reflect_types(desc.declared_bases), [&]<typename T>(const T)
            {
                // We store the type hash of the parent class, so we can get it back from the list of type info later 
                m_BaseClasses.push_back(typeid(T::type).hash_code());
            }
        );
    }
}

END_XNOR_CORE
