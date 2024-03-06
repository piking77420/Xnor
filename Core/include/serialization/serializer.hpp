#pragma once

#include <format>
#include <stack>
#include <string>

#include <RapidXML/rapidxml.hpp>
#include <RapidXML/RapidXMLSTD.hpp>

#include "core.hpp"
#include "rendering/light/directional_light.hpp"
#include "rendering/light/point_light.hpp"
#include "rendering/light/spot_light.hpp"
#include "scene/component/mesh_renderer.hpp"
#include "scene/component/test_component.hpp"
#include "utils/guid.hpp"
#include "utils/logger.hpp"
#include "utils/meta_programming.hpp"
#include "utils/serializable.hpp"

/// @file serializer.hpp
/// @brief Defines the serializer static class.

BEGIN_XNOR_CORE

/// @brief A utility class that handles XML parsing, serialization and deserialization
class Serializer
{
    STATIC_CLASS(Serializer)

    enum SerializeFlags : size_t
    {
        NONE = 0 << 0,
        EXPAND_POINTER = 1 << 0
    };

public:
    /// @brief Starts a serialization with the provided file
    /// @param filePath File name
    XNOR_ENGINE static void StartSerialization(const std::string& filePath);

    /// @brief Ends the serialization
    XNOR_ENGINE static void EndSerialization();

    /// @brief Starts a root element
    /// @param elementName Name of root
    /// @param elementValue Value of root
    XNOR_ENGINE static void BeginRootElement(const std::string& elementName, const std::string& elementValue);

    /// @brief Ends the root element
    XNOR_ENGINE static void EndRootElement();

    /// @brief Starts an XML element
    /// @param elementName Name of the element
    /// @param elementValue Value of the element
    XNOR_ENGINE static void BeginXmlElement(const std::string& elementName, const std::string& elementValue);

    /// @brief Ends an XML element
    XNOR_ENGINE static void EndXmlElement();

    /// @brief Adds an attribute to the current XML element
    /// @tparam T Value type
    /// @param attributeName Attribute name
    /// @param value Attribute value
    template <typename T>
    static void FetchAttribute(const std::string& attributeName, const T& value);

    /// @brief Serializes an object in the current document
    /// @tparam ReflectT Type
    /// @param obj Object pointer
    /// @param isRoot Is the root element of the document
    template <typename ReflectT>
    static void Serialize(const ReflectT* obj, bool_t isRoot);

    /// @brief Deserializes an object in the current document
    /// @tparam ReflectT Type
    /// @param obj Object pointer
    template <typename ReflectT>
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
    static void SerializeSimpleType(const MemberT* ptr, const char_t* name, size_t flags);

    template <typename MemberT>
    static void SerializeArrayType(const MemberT* ptr, const char_t* name, size_t flags);

    template <typename MemberT>
    static void SerializeListType(const MemberT* ptr, const char_t* name, size_t flags);

    template <typename T>
    [[nodiscard]]
    static constexpr size_t GetFlags(T member);
};

template <typename T>
void Serializer::FetchAttribute(const std::string& attributeName, const T& value)
{
    if constexpr (Meta::IsAny<T, std::string, const char_t*>)
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
void Serializer::Serialize(const ReflectT* const obj, const bool_t isRoot)
{
    constexpr TypeDescriptor<ReflectT> desc = Reflection::GetTypeInfo<ReflectT>();

    if (isRoot)
        BeginRootElement(desc.name.c_str(), "");
    else
        BeginXmlElement(desc.name.c_str(), "");

    refl::util::for_each(desc.members, [&]<typename T>(const T member)
    {
        constexpr bool_t isFunction = refl::descriptor::is_function(member);
        
        constexpr bool_t dontSerialize = Reflection::HasAttribute<NotSerializable>(member);
        constexpr size_t flags = GetFlags<T>(member);
            
        if constexpr (!isFunction)
        {
            if constexpr (!dontSerialize)
            {
                using MemberT = typename T::value_type;
                const constexpr char_t* const name = member.name.c_str();

                if constexpr (Meta::IsArray<MemberT>)
                {
                    SerializeArrayType<MemberT>(&member.get(obj), name, flags);
                }
                else if constexpr (Meta::IsXnorList<MemberT>)
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
            Logger::LogInfo("{} ; {}", desc.name.c_str(), member.name.c_str());
        }
    });

    if (isRoot)
        EndRootElement();
    else
        EndXmlElement();
}

template <typename ReflectT>
void Serializer::Deserialize(ReflectT* const)
{
    // TODO Deserialize
}

template <typename MemberT>
void Serializer::SerializeSimpleType(const MemberT* ptr, const char_t* name, const size_t flags)
{
    if constexpr (Meta::IsNativeType<MemberT> || Meta::IsMathType<MemberT> || Meta::IsSame<MemberT, std::string>)
    {
        FetchAttribute<MemberT>(name, *ptr);
    }
    else if constexpr (Meta::IsColorType<MemberT>)
    {
        // DisplayColor<MemberT>(ptr, name);
    }
    else if constexpr (Meta::IsPointer<MemberT>)
    {
        if (flags & EXPAND_POINTER)
        {
            Serialize<Meta::RemovePointerSpecifier<MemberT>>(*ptr, false);
        }
        else
        {
            if (*ptr == nullptr)
                FetchAttribute(name, Guid());
            else
                FetchAttribute(name, static_cast<std::string>((*ptr)->GetGuid()));
        }
    }
    else if constexpr (Meta::IsPolyPtr<MemberT>)
    {
        const size_t hash = ptr->GetHash();
        
#define POLY_PTR_IF_SER(type)                       \
if (hash == XnorCore::Utils::GetTypeHash<type>())   \
{                                                   \
Serialize<type>(ptr->Cast<type>(), false);          \
}                                                   \
        // TODO find a less ugly solution to that

        POLY_PTR_IF_SER(MeshRenderer);
        POLY_PTR_IF_SER(DirectionalLight);
        POLY_PTR_IF_SER(TestComponent);
        POLY_PTR_IF_SER(PointLight);
        POLY_PTR_IF_SER(SpotLight);
    }
    else if constexpr (Meta::IsXnorPointer<MemberT>)
    {
        if (flags & EXPAND_POINTER)
        {
            // Serialize<Meta::RemovePointerSpecifier<typename MemberT::Type>>(*ptr, false);
        }
        else
        {
            if (*ptr == nullptr)
                FetchAttribute(name, Guid());
            else
                FetchAttribute(name, static_cast<std::string>((*ptr)->GetGuid()));
        }
    }
    else if constexpr (Meta::IsSame<MemberT, Guid>)
    {
        FetchAttribute<std::string>(name, static_cast<std::string>(*ptr));
    }
    else if constexpr (Meta::IsEnum<MemberT>)
    {
        // TODO enum serialization
    }
    else
    {
        Serialize<MemberT>(ptr, false);
    }
}

template <typename MemberT>
void Serializer::SerializeArrayType(const MemberT*, const char_t*, const size_t)
{
    // TODO SerializeArrayType
}

template <typename MemberT>
void Serializer::SerializeListType(const MemberT* ptr, const char_t* name, const size_t flags)
{
    using ListT = typename MemberT::Type;
    
    BeginXmlElement(std::string(name), "");

    for (size_t i = 0; i < ptr->GetSize(); i++)
    {
        SerializeSimpleType<ListT>(&(*ptr)[i], std::to_string(i).c_str(), flags);
    }
    
    EndXmlElement();
}

template <typename T>
constexpr size_t Serializer::GetFlags(const T member)
{
    size_t flags = NONE; 

    if constexpr (Reflection::HasAttribute<ExpandPointer>(member))
        flags |= EXPAND_POINTER;

    return flags;
}

END_XNOR_CORE
