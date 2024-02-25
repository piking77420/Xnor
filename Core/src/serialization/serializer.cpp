#include "serialization/serializer.hpp"



#include "utils/logger.hpp"

using namespace XnorCore;


void Serializer::OpenFileToWrite(const std::string& filePath)
{
    std::string error;
    *m_CurrentFilePath = filePath;
    m_XMLDoc = CreateXML(1, "utf-16", error);
}


void Serializer::StartSerialization(const std::string& filePath)
{
    m_CurrentFilePath = new std::string(filePath);
    OpenFileToWrite(filePath);
}

void Serializer::EndSerialization()
{
    if (!SaveXML(*m_XMLDoc, *m_CurrentFilePath))
    {
        delete m_CurrentFilePath;
        Logger::LogError("Failed to write Serialization");
        DisposeXMLObject(m_XMLDoc);
    }
    delete m_CurrentFilePath;
}

void Serializer::BeginRootElement(const std::string& elementName, const std::string& elementValue)
{
    std::string error;
    
    m_RootElement = CreateElement(m_XMLDoc, elementName, elementValue, error);
    if (m_RootElement == nullptr)
    {
        Logger::LogError(error);
        DisposeXMLObject(m_XMLDoc);
    }

    AddElementToDoc();


}
void Serializer::EndRootElement()
{
    if (!m_ElementsQueue.empty())
    {
        throw std::runtime_error("Stack is not empty at EndRootElement");
    }
}

void Serializer::BeginXmlElement(const std::string& elementName, const std::string& elementValue)
{
    std::string error;
    
    XMLElement* element = ::CreateElement(m_XMLDoc, elementName, elementValue, error);
    if (element == nullptr)
    {
        Logger::LogError(error);
        throw std::runtime_error("Failed to create element");
    }
    m_ElementsQueue.push(element);
}

void Serializer::EndXmlElement()
{
    std::string error;

    if (m_ElementsQueue.size() == 1)
    {
        if (!AddElementToElement(m_RootElement, m_ElementsQueue.top(), error))
        {
            Logger::LogError(error);    
        }
    }
    else
    {
        XMLElement* child = m_ElementsQueue.top();
        m_ElementsQueue.pop();
        XMLElement* parent = m_ElementsQueue.top();
        
        if (!AddElementToElement(parent, child, error))
        {
            Logger::LogError(error);    
        }
    }
    
}

void Serializer::AddElementToDoc()
{
    std::string error;
    
    if (!::AddElementToDocument(m_XMLDoc, m_RootElement, error))
    {
        Logger::LogError(error);
        DisposeXMLObject(m_XMLDoc);
    }
}

void Serializer::FetchAttributeInternal(const std::string& attributeName, const std::string& attributeValue)
{
    std::string error;
    XMLAttributte* attribute = CreateAttribute(m_XMLDoc, attributeName, attributeValue,error);
    
    if (!AddAttributeToElement(m_ElementsQueue.top(), attribute, error))
    {
        Logger::LogError(error);
    }
}

