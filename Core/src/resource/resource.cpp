#include "resource/resource.hpp"

using namespace XnorCore;

Resource::Resource(std::string name)
    : m_Name(std::move(name))
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
