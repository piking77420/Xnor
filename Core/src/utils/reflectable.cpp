#include "utils/reflectable.hpp"

using namespace XnorCore;

const TypeInfo& TypeInfo::Get(const size_t typeHash)
{
    if (!m_TypeInfo.contains(typeHash))
    {
        Logger::LogError("Couldn't find type {}", typeHash);
    }
    
    return m_TypeInfo.at(typeHash);
}

constexpr const std::string& TypeInfo::GetName() const
{
    return m_Name;
}

constexpr size_t TypeInfo::GetSize() const
{
    return m_Size;
}

constexpr const std::vector<FieldInfo>& TypeInfo::GetMembers() const
{
    return m_Members;
}

constexpr const std::vector<size_t>& TypeInfo::GetParents() const
{
    return m_BaseClasses;
}
