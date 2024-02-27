#pragma once

#include <format>
#include <string>
#include <stack>

#include "core.hpp"
#include "RapidXML/rapidxml.hpp"
#include "RapidXML/RapidXMLSTD.hpp"
#include "utils/reflectable.hpp"

BEGIN_XNOR_CORE

class Serializer
{
    STATIC_CLASS(Serializer)

public:
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
    static void Serialize(const void* obj);

    template<typename ReflectT>
    static void Deserialize(void* obj);
    
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

    XNOR_ENGINE static void SerializeNativeType(const void* obj, const FieldInfo& fieldInfo);
    XNOR_ENGINE static void SerializeMathType(const void* obj, const FieldInfo& fieldInfo);
};

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
void Serializer::Serialize(const void* obj)
{
    const TypeInfo& info = TypeInfo::Get(typeid(ReflectT).hash_code());

    BeginXmlElement(info.GetName(), info.GetName());

    for (const FieldInfo& m : info.GetMembers())
    {
        if (m.isNativeType)
            SerializeNativeType(obj, m);
        else if (m.isMathType)
            SerializeMathType(obj, m);
    }
    
    EndXmlElement();
}

template <typename ReflectT>
void Serializer::Deserialize([[maybe_unused]] void* obj)
{
    // const TypeInfo& info = TypeInfo::Get<ReflectT>();
}

END_XNOR_CORE
