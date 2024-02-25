#pragma once

#include <format>
#include <string>
#include <stack>

#include "core.hpp"
#include "RapidXML/rapidxml.hpp"
#include "RapidXML/RapidXMLSTD.hpp"


BEGIN_XNOR_CORE

class Serializer
{
public:

    DELETE_COPY_MOVE_OPERATIONS(Serializer)

    // Open File for Writing
    XNOR_ENGINE static void StartSerialization(const std::string& filePath);

    XNOR_ENGINE static void EndSerialization();

    XNOR_ENGINE static void BeginRootElement(const std::string& elementName,const std::string& elementValue);

    XNOR_ENGINE static void EndRootElement();

    XNOR_ENGINE static void BeginXmlElement(const std::string& elementName,const std::string& elementValue);
    
    XNOR_ENGINE static void EndXmlElement();
    
    template<typename T>
    static void FetchAttribute(const std::string& attributeName,const T& value);
    
private:
    XNOR_ENGINE static inline std::string* m_CurrentFilePath = nullptr;
    
    XNOR_ENGINE static inline XMLDocument* m_XMLDoc = nullptr;
    
    XNOR_ENGINE static inline XMLElement* m_RootElement = nullptr;

    XNOR_ENGINE static inline std::stack<XMLElement*> m_ElementsQueue;

    // Serialize
    XNOR_ENGINE static void OpenFileToWrite(const std::string& filePath);
    
    // Element
    XNOR_ENGINE static void AddElementToDoc();
    
    XNOR_ENGINE static void FetchAttributeInternal(const std::string& attributeName, const std::string& attributeValue);
    
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

END_XNOR_CORE
