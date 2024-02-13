#include "resource/resource.hpp"

using namespace XnorCore;

Resource::Resource(const std::string& name)
    : m_Name(name)
{
}

bool Resource::IsLoaded() const
{
    return m_Loaded;
}

std::string Resource::GetName() const
{
    return m_Name;
}
