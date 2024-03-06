#pragma once

#include <stack>
#include <string>

#include <RapidXML/rapidxml.hpp>
#include <RapidXML/RapidXMLSTD.hpp>

#include "core.hpp"

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

END_XNOR_CORE

#include "serialization/serializer.inl"
