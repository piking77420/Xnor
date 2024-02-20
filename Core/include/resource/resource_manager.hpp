#pragma once

#include <iostream>
#include <type_traits>
#include <unordered_map>

#include "utils/pointer.hpp"
#include "resource.hpp"
#include "utils/logger.hpp"

BEGIN_XNOR_CORE

template<class T>
concept ResourceT = std::is_base_of_v<Resource, T>;

class ResourceManager final
{
public:
    // You cannot instantiate this class
    ResourceManager() = delete;
    
    /// @brief Creates the resource corresponding to the given @p name without loading it
    template<ResourceT T>
    static Pointer<T> Add(std::string name);

    /// @brief Creates the resource corresponding to the given @p file and loads it
    template<ResourceT T>
    static Pointer<T> Load(const Pointer<File>& file);

    [[nodiscard]]
    XNOR_ENGINE static bool Contains(const std::string& name);

    template<ResourceT T>
    [[nodiscard]]
    static Pointer<T> Get(const std::string& name);

    template<ResourceT T>
    [[nodiscard]]
    static bool IsResourceOfType(const std::string& name);

    XNOR_ENGINE static void Remove(const std::string& name);

    template<ResourceT T>
    static void Remove(const Pointer<T>& resource);

    XNOR_ENGINE static void RemoveAll();

private:
    XNOR_ENGINE static inline std::unordered_map<std::string, Pointer<Resource>> m_Resources;
};

template<ResourceT T>
Pointer<T> ResourceManager::Add(std::string name)
{
    Logger::LogDebug("Adding resource {}", name);
    
    Pointer<T> resource(std::forward<std::string>(name));

    // We cannot reuse the variable 'name' here in case it was moved inside the Resource constructor
    m_Resources[resource->GetName()] = static_cast<Pointer<Resource>>(resource.CreateStrongReference());

    // Make sure to return a weak reference
    resource.ToWeakReference();

    return resource;
}

template<ResourceT T>
Pointer<T> ResourceManager::Load(const Pointer<File>& file)
{
    Logger::LogDebug("Loading resource {}", file->GetFilepath());
    
    Pointer<T> resource(Utils::PathToForwardSlashes(file->GetFilepath()));

    m_Resources[resource->GetName()] = static_cast<Pointer<Resource>>(resource.CreateStrongReference());

    // Make sure to return a weak reference
    resource.ToWeakReference();

    resource->Load(*const_cast<Pointer<File>&>(file));

    return resource;
}

template<ResourceT T>
Pointer<T> ResourceManager::Get(const std::string& name)
{
    if (!Contains(name))
    {
        Logger::LogError("Attempt to get an unknown resource: {}", name);
        return Pointer<T>();
    }

    return Pointer<T>(m_Resources.at(name));
}

template<ResourceT T>
bool ResourceManager::IsResourceOfType(const std::string& name)
{
    Pointer<T> resource = Get<T>(name);

    if (resource.IsValid())
        return true;

    return false;
}

template<ResourceT T>
void ResourceManager::Remove(const Pointer<T>& resource)
{
    Logger::LogDebug("Removing resource {}", resource);
    
    const size_t oldSize = m_Resources.size();
    
    for (decltype(m_Resources)::iterator it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
        if (it->second == *reinterpret_cast<const Pointer<Resource>*>(&resource)) // TODO Change this to a static_cast
        {
            if (it->second->IsLoaded())
                it->second->Unload();
            
            it = m_Resources.erase(it);

            if (it == m_Resources.end())
                break;
        }
    }

    // If no resources were deleted
    if (oldSize == m_Resources.size())
        Logger::LogWarning("Attempt to delete an unknown file entry: {}", static_cast<T*>(resource));
}

END_XNOR_CORE
