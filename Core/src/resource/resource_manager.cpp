#include "resource/resource_manager.hpp"

using namespace XnorCore;

bool ResourceManager::Contains(const std::string& name)
{
    return m_Resources.contains(name);
}

void ResourceManager::Remove(const std::string& name)
{
    Logger::LogDebug("Removing resource {}", name);
    
    const auto&& resource = m_Resources.find(name);
    
    if (resource == m_Resources.end())
    {
        Logger::LogWarning("Attempt to delete an unknown resource: {}", name);
        return;
    }

    if (resource->second->IsLoaded())
        resource->second->Unload();

    m_Resources.erase(resource);
}

void ResourceManager::RemoveAll()
{
    Logger::LogDebug("Removing all resources");
    
    for (auto& resource : m_Resources)
    {
        Logger::LogDebug("Unloading resource {}", resource.first);
        
        if (resource.second->IsLoaded())
            resource.second->Unload();
    }
    // Smart pointers are deleted automatically, we only need to clear the container
    m_Resources.clear();
}
