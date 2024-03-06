#pragma once

#include <type_traits>
#include <unordered_map>

#include "resource.hpp"
#include "file/file.hpp"
#include "resource/shader.hpp"
#include "utils/logger.hpp"
#include "utils/pointer.hpp"

/// @file resource_manager.hpp
/// @brief Defines the ResourceManager class.

BEGIN_XNOR_CORE

/// @brief A class satisfies the ResourceT concept if it is derived of Resource.
template <class T>
concept ResourceT = std::is_base_of_v<Resource, T>;

/// @brief Static class used to add, load, get, or unload @ref Resource "Resources".
///
/// It contains all wrapper instances of the Resource class. These are either added or loaded using the corresponding
/// function: ResourceManager::Add and ResourceManager::Load.
class ResourceManager final
{
    STATIC_CLASS(ResourceManager)
    
public:
    /// @brief Special prefix used to store automatically added @ref Shader "Shaders".
    ///
    /// This is mainly used in LoadAll.
    static constexpr const char_t* ReservedShaderPrefix = "_shaders/";
    
    /// @brief Creates the Resource corresponding to the given @p name without loading it.
    template <ResourceT T>
    static Pointer<T> Add(std::string name);
    
    /// @brief Creates the Resource corresponding to the given @p file without loading it.
    template <ResourceT T>
    static Pointer<T> Add(const Pointer<File>& file);

    /// @brief Creates the Resource corresponding to the given @p file and loads it.
    template <ResourceT T>
    static Pointer<T> Load(const Pointer<File>& file);

    /// @brief Creates one Resource for each @ref FileManager entry.
    XNOR_ENGINE static void LoadAll();

    /// @brief Checks whether the ResourceManager contains the specified Resource name.
    [[nodiscard]]
    XNOR_ENGINE static bool_t Contains(const std::string& name);

    /// @brief Checks whether the ResourceManager contains the specified Resource file name.
    [[nodiscard]]
    XNOR_ENGINE static bool_t Contains(const Pointer<File>& file);

    /// @brief Returns the Resource that was either added or loaded using the given @p name.
    template <ResourceT T = Resource>
    [[nodiscard]]
    static Pointer<T> Get(const std::string& name);

    /// @brief Returns the Resource that was either added or loaded using the given file name.
    template <ResourceT T = Resource>
    [[nodiscard]]
    static Pointer<T> Get(const Pointer<File>& file);

    /// @brief Renames the Resource with the given @p name to @p newName.
    ///
    /// @note This function only renames the key used to store this Resource, and doesn't in any case rename the Resource itself.
    /// For this exact reason, using Resource::SetName instead is the preferred way or renaming a Resource.
    XNOR_ENGINE static void Rename(const std::string& name, const std::string& newName);

    /// @brief Renames the given @p resource to @p newName.
    ///
    /// @note This function only renames the key used to store this Resource, and doesn't in any case rename the Resource itself.
    /// For this exact reason, using Resource::SetName instead is the preferred way or renaming a Resource.
    XNOR_ENGINE static void Rename(const Pointer<Resource>& resource, const std::string& newName);

    /// @brief Finds all Resource of type @p T.
    /// @tparam T The type of Resource to find.
    /// @return All stored Resource of type @p T.
    template <ResourceT T>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll();
    
    /// @see ResourceManager::FindAll()
    template <ResourceT T>
    static void FindAll(std::vector<Pointer<T>>* result);

    /// @brief Finds a specific Resource based on a predicate.
    /// @tparam T The type of Resource to find.
    /// @param predicate The predicate used to find the correct Resource. This function will be
    /// called for each stored Resource.
    /// @return The first Resource for which the @p predicate returned @c true. If every Resource
    /// returned @c false, instead return a null @ref Pointer.
    template <ResourceT T = Resource>
    [[nodiscard]]
    static Pointer<T> Find(std::function<bool_t(Pointer<T>)>&& predicate);
    
    /// @brief Finds a list of Resource based on a predicate.
    /// @tparam T The type of Resource to find.
    /// @param predicate The predicate used to find the correct Resource. This function will be
    /// called for each stored Resource.
    /// @return The first Resource for which the @p predicate returned @c true. If every Resource
    /// returned @c false, instead return a null @ref Pointer.
    template <ResourceT T = Resource>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll(std::function<bool_t(Pointer<T>)>&& predicate);

    /// @see @ref FileManager::FindAll(std::function<bool_t(Pointer<T>)>&&)
    template <ResourceT T>
    static void FindAll(std::function<bool_t(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result);

    /// @brief Checks whether the given @p name corresponds to a Resource of type @p T.
    template <ResourceT T>
    [[nodiscard]]
    static bool_t IsResourceOfType(const std::string& name);

    /// @brief Unloads the Resource with the given @p name.
    XNOR_ENGINE static void Unload(const std::string& name);

    /// @brief Unloads the given @p resource.
    template <ResourceT T>
    static void Unload(const Pointer<T>& resource);

    /// @brief Unloads all stored @ref Resource "Resources".
    XNOR_ENGINE static void UnloadAll();

private:
    XNOR_ENGINE static inline std::unordered_map<std::string, Pointer<Resource>> m_Resources;
    
    template <ResourceT T>
    static Pointer<T> AddNoCheck(std::string name);
    
    template <ResourceT T>
    static Pointer<T> LoadNoCheck(Pointer<File> file);

    template <ResourceT T>
    [[nodiscard]]
    static Pointer<T> GetNoCheck(const std::string& name);
};

END_XNOR_CORE

#include "resource/resource_manager.inl"
