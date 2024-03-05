#pragma once

#include "core.hpp"
#include "utils/guid.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

/// @file resource.hpp
/// @brief Defines the Resource class

class File;

/// @brief Interface for resources, which encapsulates most objects used in the engine that come from a file
class Resource
{
public:
    /// @brief Constructs a default Resource.
    XNOR_ENGINE Resource() = default;

    /// @brief Creates a Resource with the given @p name.
    XNOR_ENGINE explicit Resource(std::string name);

    XNOR_ENGINE virtual ~Resource() = 0;

    DEFAULT_COPY_MOVE_OPERATIONS(Resource)

    /// @brief Loads data from memory.
    ///
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Load(const uint8_t* buffer, int64_t length);

    /// @brief Loads data from file.
    ///
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Load(const Pointer<File>& file);

    /// @brief Creates the Resource in the Rhi.
    XNOR_ENGINE virtual void CreateInRhi();

    /// @brief Destroys the Resource in the Rhi.
    XNOR_ENGINE virtual void DestroyInRhi();

    /// @brief Unloads the loaded data.
    XNOR_ENGINE virtual void Unload();

    /// @brief Unloads and then loads back this Resource.
    ///
    /// This is effectively equivalent to calling Unload and then @ref Load(const uint8_t* buffer, int64_t length) "Load".
    /// 
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Reload(const uint8_t* buffer, int64_t length, bool_t reloadInRhi = true);

    /// @brief Unloads and then loads back this Resource.
    ///
    /// This is effectively equivalent to calling Unload and then @ref Load(const Pointer<File>&) "Load".
    /// 
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Reload(const Pointer<File>& file, bool_t reloadInRhi = true);

    /// @brief Unloads and then loads back this Resource.
    ///
    /// This is effectively equivalent to calling Unload and then Load(const Pointer<File>&)
    /// using FileManager::Get(const std::filesystem::path&) as a parameter.
    /// 
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Reload(bool_t reloadInRhi = true);

    /// @brief Returns whether the Resource has already been loaded.
    [[nodiscard]]
    XNOR_ENGINE bool_t IsLoaded() const;

    /// @brief Returns whether the Resource has already been loaded in the Rhi.
    [[nodiscard]]
    XNOR_ENGINE bool_t IsLoadedInRhi() const;

    /// @brief Returns the name of this Resource.
    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;

    /// @brief Sets the name of this Resource.
    ///
    /// This function also renames the Resource entry in the ResourceManager using ResourceManager::Rename(const std::string&, const std::string&).
    XNOR_ENGINE void SetName(std::string newName);

    /// @brief Returns the Guid of this Resource.
    [[nodiscard]]
    XNOR_ENGINE const Guid& GetGuid() const;

protected:
    bool_t m_Loaded = false;
    bool_t m_LoadedInRhi = false;

    std::string m_Name;
    Guid m_Guid;
};

END_XNOR_CORE
