#include "resource/resource_manager.hpp"

using namespace XnorCore;

bool ResourceManager::Contains(const std::string& name)
{
    return m_Resources.contains(name);
}

void ResourceManager::Remove(const std::string& name)
{
    const auto&& resource = m_Resources.find(name);
    
    if (resource == m_Resources.end())
    {
        Logger::LogWarning("Attempt to delete an unknown resource: %s", name.c_str());
        return;
    }

    if (resource->second->IsLoaded())
        resource->second->Unload();

    m_Resources.erase(resource);
}

void ResourceManager::RemoveAll()
{
    for (auto& resource : m_Resources)
    {
        if (resource.second->IsLoaded())
            resource.second->Unload();
    }
    // Smart pointers are deleted automatically, we only need to clear the container
    m_Resources.clear();
}
