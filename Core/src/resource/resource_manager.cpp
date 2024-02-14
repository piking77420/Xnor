#include "resource/resource_manager.hpp"

using namespace XnorCore;

bool ResourceManager::Contains(const std::string& name)
{
    return m_Resources.contains(name);
}

void ResourceManager::Delete(const std::string& name)
{
    if (m_Resources.erase(name) == 0)
        Logger::LogWarning("Attempt to delete an unknown resource: %s", name.c_str());
}

void ResourceManager::DeleteAll()
{
    // Smart pointers are deleted automatically, we only need to clear the container
    m_Resources.clear();
}
