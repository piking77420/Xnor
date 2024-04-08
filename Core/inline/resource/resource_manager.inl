#pragma once

#include <ranges>

#include "resource/compute_shader.hpp"
#include "resource/shader.hpp"

BEGIN_XNOR_CORE

template <Concepts::ResourceT T>
Pointer<T> ResourceManager::Add(std::string name)
{
    Logger::LogDebug("Adding resource {}", name);

    if (Contains(name))
    {
        Logger::LogWarning("This resource has already been added, consider using ResourceManager::Get instead");
        return GetNoCheck<T>(name);
    }
    
    return AddNoCheck<T>(std::forward<std::string>(name));
}

template <Concepts::ResourceT T>
Pointer<T> ResourceManager::Add(const Pointer<File>& file)
{
    return Add<T>(file->GetPathString());
}

template <Concepts::ResourceT T>
Pointer<T> ResourceManager::Load(const Pointer<File>& file, bool_t loadInRhi)
{
    Logger::LogDebug("Loading resource {}", file->GetPath());

    if (Contains(file->GetPathString()))
    {
        Pointer<T> resource = GetNoCheck<T>(file->GetPathString());
        const bool_t loaded = resource->IsLoaded();
        Logger::LogWarning(
            "This resource has already been {}, consider using ResourceManager::Get instead{}",
            loaded ? "loaded" : "added but isn't loaded",
            loaded ? "" : ". Loading it"
        );

        if (!loaded)
            resource->Load(file);

        return resource;
    }

    return LoadNoCheck<T>(file, loadInRhi);
}

template <Concepts::ResourceT T>
Pointer<T> ResourceManager::Get(const std::string& name)
{
    if (!Contains(name))
    {
        Logger::LogError("Attempt to get an unknown resource: {}", name);
        return nullptr;
    }

    return GetNoCheck<T>(name);
}

template <>
inline Pointer<Shader> ResourceManager::Get<Shader>(const std::string& name)
{
    if (!Contains(name))
    {
        if (Contains(ReservedShaderPrefix + name))
            return GetNoCheck<Shader>(ReservedShaderPrefix + name);
        
        Logger::LogError("Attempt to get an unknown shader: {}", name);
        return nullptr;
    }

    return GetNoCheck<Shader>(name);
}

template <>
inline Pointer<ComputeShader> ResourceManager::Get<ComputeShader>(const std::string& name)
{
    if (!Contains(name))
    {
        if (Contains(ReservedShaderPrefix + name))
            return GetNoCheck<ComputeShader>(ReservedShaderPrefix + name);
        
        Logger::LogError("Attempt to get an unknown resource: {}", name);
        return nullptr;
    }

    return GetNoCheck<ComputeShader>(name);
}


template <Concepts::ResourceT T>
Pointer<T> ResourceManager::Get(const Pointer<File>& file)
{
    return Get<T>(file->GetPathString());
}

template <Concepts::ResourceT T>
Pointer<T> ResourceManager::Get(const Guid& guid)
{
    auto&& it = m_GuidMap.find(guid);

    if (it == m_GuidMap.end())
        return nullptr;

    return Utils::DynamicPointerCast<T>(GetNoCheck<T>(it->second));
}

template <Concepts::ResourceT T>
std::vector<Pointer<T>> ResourceManager::FindAll()
{
    std::vector<Pointer<T>> result;
    FindAll<T>(&result);
    return result;
}

template <Concepts::ResourceT T>
void ResourceManager::FindAll(std::vector<Pointer<T>>* result)
{
    result->clear();
    
    for (auto& val : m_Resources | std::views::values)
    {
        // ReSharper disable once CppTooWideScope
        Pointer<T> entry = Utils::DynamicPointerCast<T>(val);
        
        if (entry)
            result->push_back(std::move(entry));
    }
}

template <Concepts::ResourceT T>
Pointer<T> ResourceManager::Find(std::function<bool_t(Pointer<T>)>&& predicate)
{
    for (const auto& val : m_Resources | std::views::values)
    {
        Pointer<Resource> resource = val;

        Pointer<T> r = Utils::DynamicPointerCast<T>(val);
        if (r && predicate(r))
            return r;
    }

    return nullptr;
}

template <Concepts::ResourceT T>
std::vector<Pointer<T>> ResourceManager::FindAll(std::function<bool_t(Pointer<T>)>&& predicate)
{
    std::vector<Pointer<T>> result;
    FindAll<T>(std::forward<decltype(predicate)>(predicate), &result);
    return result;
}

template <Concepts::ResourceT T>
void ResourceManager::FindAll(std::function<bool_t(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result)
{
    result->clear();

    for (const Pointer<Resource>& val : m_Resources | std::views::values)
    {
        Pointer<T> r = Utils::DynamicPointerCast<T>(val);
        if (r && predicate(r))
            result->push_back(r);
    }
}
 
template <Concepts::ResourceT T>
bool_t ResourceManager::IsResourceOfType(const std::string& name)
{
    Pointer<T> resource = Get<T>(name);

    if (resource.IsValid())
        return true;

    return false;
}

template <Concepts::ResourceT T>
void ResourceManager::Unload(const Pointer<T>& resource)
{
    Logger::LogDebug("Unloading resource {}", resource);
    
    const size_t oldSize = m_Resources.size();
    
    for (decltype(m_Resources)::iterator it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
        Pointer<Resource>& storedResource = it->second;
        if (storedResource == Utils::DynamicPointerCast<Resource>(resource))
        {
            if (storedResource->IsLoadedInRhi())
                storedResource->DestroyInRhi();
            
            if (storedResource->IsLoaded())
                storedResource->Unload();

            it = m_Resources.erase(it);

            if (it == m_Resources.end())
                break;
        }
    }

    // If no resources were deleted
    if (oldSize == m_Resources.size())
        Logger::LogWarning("Attempt to unload an unknown file entry: {}", static_cast<T*>(resource));
}

template <Concepts::ResourceT T>
Pointer<T> ResourceManager::AddNoCheck(std::string name)
{
    Pointer<T> resource = Pointer<T>::Create(std::forward<std::string>(name));

    {
        std::scoped_lock lock(m_ResourcesMutex);
        // We cannot reuse the variable 'name' here in case it was moved inside the Resource constructor
        m_Resources[resource->GetName()] = static_cast<Pointer<Resource>>(resource.CreateStrongReference());
    }

    // Make sure to return a weak reference
    resource.ToWeakReference();

    return resource;
}

template <Concepts::ResourceT T>
Pointer<T> ResourceManager::LoadNoCheck(Pointer<File> file, const bool_t loadInRhi)
{
    Pointer<T> resource = Pointer<T>::Create(file->GetPathString());
    
    {
        std::scoped_lock lock(m_ResourcesMutex);
        m_Resources[resource->GetName()] = static_cast<Pointer<Resource>>(resource.CreateStrongReference());
    }

    // Make sure to return a weak reference
    resource.ToWeakReference();

    resource->Load(file);

    file->m_Resource = std::move(Pointer<Resource>(resource, false));

    if (loadInRhi)
        resource->CreateInRhi();

    return resource;
}

template <Concepts::ResourceT T>
Pointer<T> ResourceManager::GetNoCheck(const std::string& name)
{
    std::scoped_lock lock(m_ResourcesMutex);
    return Pointer<T>(m_Resources.at(name));
}

END_XNOR_CORE
