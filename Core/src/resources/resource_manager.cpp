#include "resource_manager.hpp"

using namespace XnorCore;

bool ResourceManager::Contains(const std::string& name)
{
    return m_Resources.contains(name);
}

void ResourceManager::Delete(const std::string& name)
{
    m_Resources.erase(name);
}
