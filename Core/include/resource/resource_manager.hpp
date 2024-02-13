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

private:
    static inline std::unordered_map<std::string, Pointer<Resource>> m_Resources;
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

END_XNOR_CORE
