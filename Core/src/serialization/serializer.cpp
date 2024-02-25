#include "serialization/serializer.hpp"
#include "utils/logger.hpp"

#include "Maths/quaternion.hpp"
#include "Maths/vector2.hpp"
#include "Maths/vector2i.hpp"
#include "Maths/vector3.hpp"
#include "Maths/vector4.hpp"

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
        DisposeXMLObject(m_XmlDoc);
    }
    
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
    
    if (!AddAttributeToElement(m_ElementsStack.top(), attribute, error))
    {
        Logger::LogError(error);
    }
}

void Serializer::SerializeNativeType(const void* obj, const FieldInfo& fieldInfo)
{
    if (fieldInfo.typeHash == typeid(int32_t).hash_code())
    {
        FetchAttribute<int32_t>(fieldInfo.name, *Utils::GetAddress<int32_t>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(uint32_t).hash_code())
    {
        FetchAttribute<uint32_t>(fieldInfo.name, *Utils::GetAddress<uint32_t>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(int16_t).hash_code())
    {
        FetchAttribute<int16_t>(fieldInfo.name, *Utils::GetAddress<int16_t>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(uint16_t).hash_code())
    {
        FetchAttribute<uint16_t>(fieldInfo.name, *Utils::GetAddress<uint16_t>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(int8_t).hash_code())
    {
        FetchAttribute<int8_t>(fieldInfo.name, *Utils::GetAddress<int8_t>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(uint8_t).hash_code())
    {
        FetchAttribute<uint8_t>(fieldInfo.name, *Utils::GetAddress<uint8_t>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(float_t).hash_code())
    {
        FetchAttribute<float_t>(fieldInfo.name, *Utils::GetAddress<float_t>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(double_t).hash_code())
    {
        FetchAttribute<double_t>(fieldInfo.name, *Utils::GetAddress<double_t>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(bool_t).hash_code())
    {
        FetchAttribute<bool_t>(fieldInfo.name, *Utils::GetAddress<bool_t>(obj, fieldInfo.offset, 0));
    }
}

void Serializer::SerializeMathType(const void* obj, const FieldInfo& fieldInfo)
{
    if (fieldInfo.typeHash == typeid(Vector2i).hash_code())
    {
        FetchAttribute<Vector2i>(fieldInfo.name, *Utils::GetAddress<Vector2i>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(Vector2).hash_code())
    {
        FetchAttribute<Vector2>(fieldInfo.name, *Utils::GetAddress<Vector2>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(Vector3).hash_code())
    {
        FetchAttribute<Vector3>(fieldInfo.name, *Utils::GetAddress<Vector3>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(Vector4).hash_code())
    {
        FetchAttribute<Vector4>(fieldInfo.name, *Utils::GetAddress<Vector4>(obj, fieldInfo.offset, 0));
    }
    else if (fieldInfo.typeHash == typeid(Quaternion).hash_code())
    {
        FetchAttribute<Quaternion>(fieldInfo.name, *Utils::GetAddress<Quaternion>(obj, fieldInfo.offset, 0));
    }
}

