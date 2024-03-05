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
    XNOR_ENGINE Resource() = default;

    XNOR_ENGINE explicit Resource(std::string name);

    XNOR_ENGINE virtual ~Resource() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Resource)

    /// @brief Load resource from memory.
    ///
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Load(const uint8_t* buffer, int64_t length) = 0;

    /// @brief Load resource from file.
    ///
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Load(const Pointer<File>& file);

    XNOR_ENGINE virtual void CreateInRhi();

    XNOR_ENGINE virtual void DestroyInRhi();

    XNOR_ENGINE virtual void Unload() = 0;

    /// @brief Unloads and then loads back this @ref Resource.
    ///
    /// This is effectively equivalent to calling @ref Unload and then @ref Load(const uint8_t* buffer, int64_t length) "Load".
    /// 
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Reload(const uint8_t* buffer, int64_t length, bool_t reloadInRhi = true);

    /// @brief Unloads and then loads back this @ref Resource.
    ///
    /// This is effectively equivalent to calling @ref Unload and then @ref Load(const Pointer<File>&) "Load".
    /// 
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Reload(const Pointer<File>& file, bool_t reloadInRhi = true);

    /// @brief Unloads and then loads back this @ref Resource.
    ///
    /// This is effectively equivalent to calling @ref Unload and then @ref Load(const Pointer<File>&)
    /// using @ref FileManager::Get(const std::filesystem::path&) as a parameter.
    /// 
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Reload(bool_t reloadInRhi = true);
    
    [[nodiscard]]
    XNOR_ENGINE bool_t IsLoaded() const;

    [[nodiscard]]
    XNOR_ENGINE bool_t IsLoadedInRhi() const;

    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;

    XNOR_ENGINE void SetName(std::string newName);

    [[nodiscard]]
    XNOR_ENGINE const Guid& GetGuid() const;

protected:
    bool_t m_Loaded = false;
    bool_t m_LoadedInRhi = false;

    std::string m_Name;
    Guid m_Guid;
};

END_XNOR_CORE
