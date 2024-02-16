#include "utils/reflectable.hpp"
#include "core.hpp"

using namespace XnorCore;

constexpr const std::string& TypeInfo::GetName() const
{
    return m_Name;
}

constexpr const std::vector<FieldInfo>& TypeInfo::GetMembers() const
{
    return m_Members;
}
