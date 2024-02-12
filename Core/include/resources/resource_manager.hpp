#pragma once

#include <iostream>
#include <type_traits>
#include <unordered_map>

#include "pointer.hpp"
#include "resource.hpp"

BEGIN_XNOR_CORE

template<class T>
concept ResourceT = std::is_base_of_v<Resource, T>;

class ResourceManager
{
public:
    // You cannot instantiate this class
    ResourceManager() = delete;
    
    template<ResourceT T>
    static Pointer<T> Create(const std::string& name);

    static bool Contains(const std::string& name);

    template<ResourceT T>
    static Pointer<T> Get(const std::string& name);

    template<ResourceT T>
    static bool IsResourceOfType(const std::string& name);

    static void Delete(const std::string& name);

private:
    static inline std::unordered_map<std::string, Pointer<Resource>> m_Resources;
};

template<ResourceT T>
Pointer<T> ResourceManager::Create(const std::string& name)
{
    Pointer<T> resource(name);

    m_Resources[resource->GetFilenameNoExtension().string()] = static_cast<Pointer<Resource>>(resource.CreateStrongReference());

    // Make sure to return a weak reference
    resource.ToWeakReference();

    return resource;
}

template<ResourceT T>
Pointer<T> ResourceManager::Get(const std::string& name)
{
    if (!Contains(name))
    {
        std::cout << "Attempt to get an unknown resource: " << name << '\n';
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
