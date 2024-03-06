#include "serialization/serializer.hpp"

#include "Maths/vector2i.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Serializer::OpenFileToWrite(const std::string& filePath)
{
    std::string error;
    m_CurrentFilePath = filePath;
    m_XmlDoc = CreateXML(1, "utf-16", error);
}

void Serializer::StartSerialization(const std::string& filePath)
{
    m_CurrentFilePath = filePath;
    OpenFileToWrite(filePath);
}

void Serializer::EndSerialization()
{
    if (!SaveXML(*m_XmlDoc, m_CurrentFilePath))
    {
        Logger::LogError("Failed to write Serialization");
    }

    DisposeXMLObject(m_XmlDoc);
    m_CurrentFilePath = "";
}

void Serializer::BeginRootElement(const std::string& elementName, const std::string& elementValue)
{
    std::string error;
    
    m_RootElement = CreateElement(m_XmlDoc, elementName, elementValue, error);
    if (m_RootElement == nullptr)
    {
        Logger::LogError(error);
        DisposeXMLObject(m_XmlDoc);
    }

    AddElementToDoc();
}

void Serializer::EndRootElement()
{
    if (!m_ElementsStack.empty())
    {
        throw std::runtime_error("Stack is not empty at EndRootElement");
    }
}

void Serializer::BeginXmlElement(const std::string& elementName, const std::string& elementValue)
{
    std::string error;
    
    XMLElement* element = CreateElement(m_XmlDoc, elementName, elementValue, error);
    if (element == nullptr)
    {
        Logger::LogError(error);
        throw std::runtime_error("Failed to create element");
    }
    
    m_ElementsStack.push(element);
}

void Serializer::EndXmlElement()
{
    std::string error;

    if (m_ElementsStack.size() == 1)
    {
        if (!AddElementToElement(m_RootElement, m_ElementsStack.top(), error))
        {
            Logger::LogError(error);    
        }
        m_ElementsStack.pop();
    }
    else
    {
        XMLElement* child = m_ElementsStack.top();
        m_ElementsStack.pop();
        XMLElement* parent = m_ElementsStack.top();
        
        if (!AddElementToElement(parent, child, error))
        {
            Logger::LogError(error);    
        }
    }
}

void Serializer::AddElementToDoc()
{
    std::string error;
    
    if (!::AddElementToDocument(m_XmlDoc, m_RootElement, error))
    {
        Logger::LogError(error);
        DisposeXMLObject(m_XmlDoc);
    }
}

void Serializer::FetchAttributeInternal(const std::string& attributeName, const std::string& attributeValue)
{
    std::string error;
    XMLAttributte* attribute = CreateAttribute(m_XmlDoc, attributeName, attributeValue, error);

    if (!attribute)
    {
        Logger::LogError(error);
    }

    if (!AddAttributeToElement(m_ElementsStack.top(), attribute, error))
    {
        Logger::LogError(error);
    }
}
