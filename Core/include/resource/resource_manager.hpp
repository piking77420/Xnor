#pragma once

#include <type_traits>
#include <unordered_map>

#include "resource.hpp"
#include "utils/logger.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

template<class T>
concept ResourceT = std::is_base_of_v<Resource, T>;

class ResourceManager final
{
    STATIC_CLASS(ResourceManager)
    
public:
    /// @brief Creates the @ref Resource corresponding to the given @p name without loading it.
    template<ResourceT T>
    static Pointer<T> Add(std::string name);
    
    /// @brief Creates the @ref Resource corresponding to the given @p file without loading it.
    template<ResourceT T>
    static Pointer<T> Add(const Pointer<File>& file);

    /// @brief Creates the @ref Resource corresponding to the given @p file and loads it.
    template<ResourceT T>
    static Pointer<T> Load(const Pointer<File>& file);

    /// @brief Creates the @ref Resource "Resources" corresponding to each @ref FileManager entry.
    XNOR_ENGINE static void LoadAll();

    [[nodiscard]]
    XNOR_ENGINE static bool Contains(const std::string& name);

    template<ResourceT T>
    [[nodiscard]]
    static Pointer<T> Get(const std::string& name);

    template<ResourceT T>
    [[nodiscard]]
    static Pointer<T> Get(const Pointer<File>& file);

    /// @brief Finds all @ref Resource "Resources" of type @p T.
    /// @tparam T The type of @ref Resource to find.
    /// @return All stored @ref Resource "Resources" of type @p T.
    template<ResourceT T>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll();
    
    /// @see @ref ResourceManager::FindAll()
    template<ResourceT T>
    static void FindAll(std::vector<Pointer<T>>* result);

    /// @brief Finds a specific @ref Resource based on a predicate.
    /// @tparam T The type of @ref Resource to find.
    /// @param predicate The predicate used to find the correct @ref Resource. This function will be
    /// called for each stored @ref Resource.
    /// @return The first @ref Resource for which the @p predicate returned @c true. If every @ref Resource
    /// returned @c false, instead return a null @ref Pointer.
    template<ResourceT T>
    [[nodiscard]]
    static Pointer<T> Find(std::function<bool(Pointer<T>)>&& predicate);
    
    /// @brief Finds a list of @ref Resource "Resources" based on a predicate.
    /// @tparam T The type of @ref Resource to find.
    /// @param predicate The predicate used to find the correct @ref Resource. This function will be
    /// called for each stored @ref Resource.
    /// @return The first @ref Resource for which the @p predicate returned @c true. If every @ref Resource
    /// returned @c false, instead return a null @ref Pointer.
    template<ResourceT T>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll(std::function<bool(Pointer<T>)>&& predicate);

    /// @see @ref FileManager::FindAll(std::function<bool(Pointer<T>)>&&)
    template<ResourceT T>
    static void FindAll(std::function<bool(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result);

    template<ResourceT T>
    [[nodiscard]]
    static bool IsResourceOfType(const std::string& name);

    XNOR_ENGINE static void Unload(const std::string& name);

    template<ResourceT T>
    static void Unload(const Pointer<T>& resource);

    XNOR_ENGINE static void UnloadAll();

private:
    XNOR_ENGINE static inline std::unordered_map<std::string, Pointer<Resource>> m_Resources;
    
    template<ResourceT T>
    static Pointer<T> AddNoCheck(std::string name);
    
    template<ResourceT T>
    static Pointer<T> LoadNoCheck(const Pointer<File>& file);

    template<ResourceT T>
    [[nodiscard]]
    static Pointer<T> GetNoCheck(const std::string& name);
};

template<ResourceT T>
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

template<ResourceT T>
Pointer<T> ResourceManager::Add(const Pointer<File>& file)
{
    return Add<T>(file->GetPathString());
}

template<ResourceT T>
Pointer<T> ResourceManager::Load(const Pointer<File>& file)
{
    Logger::LogDebug("Loading resource {}", file->GetPath());

    if (Contains(file->GetPathString()))
    {
        Pointer<T> resource = GetNoCheck<T>(file->GetPathString());
        const bool loaded = resource->GetLoaded();
        Logger::LogWarning(
            "This resource has already been {}, consider using ResourceManager::Get instead{}",
            loaded ? "loaded" : "added but isn't loaded",
            loaded ? "" : ". Loading it"
        );
        if (!loaded)
            resource->Load(file);
        return resource;
    }

    return LoadNoCheck<T>(file);
}

template<ResourceT T>
Pointer<T> ResourceManager::Get(const std::string& name)
{
    if (!Contains(name))
    {
        Logger::LogError("Attempt to get an unknown resource: {}", name);
        return nullptr;
    }

    return GetNoCheck<T>(name);
}

template<ResourceT T>
Pointer<T> ResourceManager::Get(const Pointer<File>& file)
{
    return Get<T>(file->GetPathString());
}

template<ResourceT T>
std::vector<Pointer<T>> ResourceManager::FindAll()
{
    std::vector<Pointer<T>> result;
    FindAll<T>(&result);
    return result;
}

template<ResourceT T>
void ResourceManager::FindAll(std::vector<Pointer<T>>* result)
{
    result->clear();
    
    for (auto&& mapEntry : m_Resources)
    {
        Pointer<T> entry = Utils::DynamicPointerCast<T>(mapEntry.second);
        
        if (entry)
            result->push_back(std::move(entry));
    }
}

template<ResourceT T>
Pointer<T> ResourceManager::Find(std::function<bool(Pointer<T>)>&& predicate)
{
    for (auto&& mapResource : m_Resources)
    {
        Pointer<Resource> resource = mapResource.second;
        
        if (Utils::DynamicPointerCast<T>(resource) && predicate(resource))
            return resource;
    }

    return nullptr;
}

template<ResourceT T>
std::vector<Pointer<T>> ResourceManager::FindAll(std::function<bool(Pointer<T>)>&& predicate)
{
    std::vector<Pointer<T>> result;
    FindAll<T>(predicate, &result);
    return result;
}

template<ResourceT T>
void ResourceManager::FindAll(std::function<bool(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result)
{
    result->clear();
    
    for (auto&& mapResource : m_Resources)
    {
        Pointer<Resource> resource = mapResource.second;
        
        if (Utils::DynamicPointerCast<T>(resource) && predicate(resource))
            result->push_back(resource);
    }
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
void ResourceManager::Unload(const Pointer<T>& resource)
{
    Logger::LogDebug("Unloading resource {}", resource);
    
    const size_t oldSize = m_Resources.size();
    
    for (decltype(m_Resources)::iterator it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
        Pointer<Resource>& storedResource = it->second;
        if (storedResource == Utils::DynamicPointerCast<Resource>(resource))
        {
            if (storedResource->GetLoadedInRhi())
                storedResource->DestroyInRhi();
            
            if (storedResource->GetLoaded())
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

template<ResourceT T>
Pointer<T> ResourceManager::AddNoCheck(std::string name)
{
    Pointer<T> resource(std::forward<std::string>(name));

    // We cannot reuse the variable 'name' here in case it was moved inside the Resource constructor
    m_Resources[resource->GetName()] = static_cast<Pointer<Resource>>(resource.CreateStrongReference());

    // Make sure to return a weak reference
    resource.ToWeakReference();

    return resource;
}

template<ResourceT T>
Pointer<T> ResourceManager::LoadNoCheck(const Pointer<File>& file)
{
    Pointer<T> resource(file->GetPathString());

    m_Resources[resource->GetName()] = static_cast<Pointer<Resource>>(resource.CreateStrongReference());

    // Make sure to return a weak reference
    resource.ToWeakReference();

    resource->Load(file);

    resource->CreateInRhi();

    return resource;
}

template<ResourceT T>
Pointer<T> ResourceManager::GetNoCheck(const std::string& name)
{
    return Pointer<T>(m_Resources.at(name));
}

END_XNOR_CORE
