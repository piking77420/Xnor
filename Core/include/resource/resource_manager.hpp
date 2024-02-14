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

class ResourceManager
{
public:
    // You cannot instantiate this class
    ResourceManager() = delete;
    
    template<ResourceT T>
    [[nodiscard]]
    static Pointer<T> Create(std::string name);

    [[nodiscard]]
    XNOR_ENGINE static bool Contains(const std::string& name);

    template<ResourceT T>
    [[nodiscard]]
    static Pointer<T> Get(const std::string& name);

    template<ResourceT T>
    [[nodiscard]]
    static bool IsResourceOfType(const std::string& name);

    XNOR_ENGINE static void Delete(const std::string& name);

    template<ResourceT T>
    static void Delete(const Pointer<T>& resource);

    XNOR_ENGINE static void DeleteAll();

private:
    XNOR_ENGINE static inline std::unordered_map<std::string, Pointer<Resource>> m_Resources;
};

template<ResourceT T>
Pointer<T> ResourceManager::Create(std::string name)
{
    Pointer<T> resource(std::forward<std::string>(name));

    m_Resources[resource->GetName()] = static_cast<Pointer<Resource>>(resource.CreateStrongReference());

    // Make sure to return a weak reference
    resource.ToWeakReference();

    return resource;
}

template<ResourceT T>
Pointer<T> ResourceManager::Get(const std::string& name)
{
    if (!Contains(name))
    {
        Logger::LogError("Attempt to get an unknown resource: %s", name.c_str());
        return Pointer<T>();
    }

    Pointer<T> resource = m_Resources.at(name);

    return resource;
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
void ResourceManager::Delete(const Pointer<T>& resource)
{
    const size_t oldSize = m_Resources.size();
    
    for (decltype(m_Resources)::iterator it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
        if (it->second == *reinterpret_cast<const Pointer<Resource>*>(&resource)) // TODO Change this to a static_cast
        {
            it = m_Resources.erase(it);

            if (it == m_Resources.end())
                break;
        }
    }
    
    if (oldSize == m_Resources.size())
        Logger::LogWarning("Attempt to delete an unknown file entry: %p", static_cast<T*>(resource));
}

END_XNOR_CORE
