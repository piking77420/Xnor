#pragma once

#include <format>
#include <stack>
#include <string>

#include "core.hpp"
#include "RapidXML/rapidxml.hpp"
#include "RapidXML/RapidXMLSTD.hpp"
#include "utils/reflectable.hpp"

BEGIN_XNOR_CORE

class Serializer
{
    STATIC_CLASS(Serializer)

public:
    enum class SerializeFlags : size_t
    {
        NONE = 0 << 0,
        EXPAND_POINTER = 1 << 0
    };
    
    // Open File for Writing
    XNOR_ENGINE static void StartSerialization(const std::string& filePath);

    XNOR_ENGINE static void EndSerialization();

    XNOR_ENGINE static void BeginRootElement(const std::string& elementName, const std::string& elementValue);

    XNOR_ENGINE static void EndRootElement();

    XNOR_ENGINE static void BeginXmlElement(const std::string& elementName, const std::string& elementValue);
    
    XNOR_ENGINE static void EndXmlElement();
    
    template<typename T>
    static void FetchAttribute(const std::string& attributeName, const T& value);

    template<typename ReflectT>
    static void Serialize(const ReflectT* obj);

    template<typename ReflectT>
    static void Deserialize(ReflectT* obj);
    
private:
    XNOR_ENGINE static inline std::string m_CurrentFilePath;
    
    XNOR_ENGINE static inline XMLDocument* m_XmlDoc = nullptr;
    
    XNOR_ENGINE static inline XMLElement* m_RootElement = nullptr;

    XNOR_ENGINE static inline std::stack<XMLElement*> m_ElementsStack;

    // Serialize
    XNOR_ENGINE static void OpenFileToWrite(const std::string& filePath);
    
    // Element
    XNOR_ENGINE static void AddElementToDoc();
    
    XNOR_ENGINE static void FetchAttributeInternal(const std::string& attributeName, const std::string& attributeValue);

    template <typename MemberT>
    static void SerializeSimpleType(const MemberT* ptr, const char_t* name, SerializeFlags flags);

    template <typename MemberT>
    static void SerializeArrayType(const MemberT* ptr, const char_t* name, SerializeFlags flags);

    template <typename MemberT>
    static void SerializeListType(const MemberT* ptr, const char_t* name, SerializeFlags flags);

    template <typename T>
    [[nodiscard]]
    static constexpr SerializeFlags GetFlags(T member);
};

constexpr Serializer::SerializeFlags& operator|=(Serializer::SerializeFlags& a, const Serializer::SerializeFlags b)
{
    a = static_cast<Serializer::SerializeFlags>(static_cast<size_t>(a) | static_cast<size_t>(b));
    return a;
}

constexpr bool operator&(const Serializer::SerializeFlags a, const Serializer::SerializeFlags b)
{
    return static_cast<size_t>(a) & static_cast<size_t>(b);
}

template<typename T>
void Serializer::FetchAttribute(const std::string& attributeName, const T& value)
{
    if constexpr (std::is_same_v<std::string, T> || std::is_same_v<const char*, T>)
    {
        FetchAttributeInternal(attributeName, value);
    }
    else
    {
        const std::string valueString = std::format("{}", value);
        FetchAttributeInternal(attributeName, valueString);
    }
}

template <typename ReflectT>
void Serializer::Serialize(const ReflectT* const obj)
{
    constexpr TypeDescriptor<ReflectT> desc = TypeInfo::Get<ReflectT>();

    BeginXmlElement(desc.name.c_str(), "");

    refl::util::for_each(desc.members, [&]<typename T>(const T member)
    {
        constexpr bool isFunction = refl::descriptor::is_function(member);
        
        constexpr bool dontSerialize = refl::descriptor::has_attribute<NotSerializable>(member);
        constexpr SerializeFlags flags = GetFlags<T>(member);
            
        if constexpr (!isFunction)
        {
            if constexpr (!dontSerialize)
            {
                using MemberT = typename T::value_type;
                const constexpr char_t* const name = member.name.c_str();

                if constexpr (std::is_array_v<MemberT>)
                {
                    SerializeArrayType<MemberT>(&member.get(obj), name, flags);
                }
                else if constexpr (XnorCore::Meta::IsXnorList<MemberT>)
                {
                    SerializeListType<MemberT>(&member.get(obj), name, flags);
                }
                else
                {
                    SerializeSimpleType<MemberT>(&member.get(obj), name, flags);
                }
            }
        }
        else
        {
            Logger::LogInfo("{}", member.name.c_str());
        }
    });

    EndXmlElement();
}

template <typename ReflectT>
void Serializer::Deserialize([[maybe_unused]] ReflectT* obj)
{
    // const TypeInfo& info = TypeInfo::Get<ReflectT>();
}

template <typename MemberT>
void Serializer::SerializeSimpleType(const MemberT* ptr, const char_t* name, const SerializeFlags flags)
{
    if constexpr (Meta::IsNativeType<MemberT> || Meta::IsMathType<MemberT> || std::is_same_v<MemberT, std::string>)
    {
        FetchAttribute<MemberT>(name, *ptr);
    }
    else if constexpr (XnorCore::Meta::IsColorType<MemberT>)
    {
        // DisplayColor<MemberT>(ptr, name);
    }
    else if constexpr (std::is_pointer_v<MemberT>)
    {
        if (flags & SerializeFlags::EXPAND_POINTER)
        {
            Serialize<std::remove_pointer_t<MemberT>>(*ptr);
        }
        else
        {
            if (*ptr == nullptr)
                FetchAttribute(name, Guid());
            else
                FetchAttribute(name, static_cast<std::string>((*ptr)->GetId()));
        }
    }
    else if constexpr (XnorCore::Meta::IsPolyPtr<MemberT>)
    {

    }
    else if constexpr (XnorCore::Meta::IsXnorPointer<MemberT>)
    {
        // DisplayXnorPointer<MemberT>(ptr, name);
    }
    else if constexpr (std::is_same_v<MemberT, Guid>)
    {
        FetchAttribute<std::string>(name, static_cast<std::string>(*ptr));
    }
    else
    {
        Serialize<MemberT>(ptr);
    }
}

template <typename MemberT>
void Serializer::SerializeArrayType(const MemberT* ptr, const char_t* name, const SerializeFlags flags)
{
}

template <typename MemberT>
void Serializer::SerializeListType(const MemberT* ptr, const char_t* name, const SerializeFlags flags)
{
    using ListT = typename MemberT::Type;
    
    BeginXmlElement(std::string(name), std::string(name));

    for (size_t i = 0; i < ptr->GetSize(); i++)
    {
        SerializeSimpleType<ListT>(&(*ptr)[i], std::to_string(i).c_str(), flags);
    }
    
    EndXmlElement();
}

template <typename T>
constexpr Serializer::SerializeFlags Serializer::GetFlags(const T member)
{
    SerializeFlags flags = SerializeFlags::NONE; 

    if constexpr (refl::descriptor::has_attribute<ExpandPointer>(member))
        flags |= SerializeFlags::EXPAND_POINTER;

    return flags;
}

END_XNOR_CORE
