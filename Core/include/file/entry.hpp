#pragma once

#include <filesystem>

#include "core.hpp"
#include "utils/pointer.hpp"

/// @file entry.hpp
/// @brief Defines the @ref Entry class.

BEGIN_XNOR_CORE

class Directory;

/// @brief File system entry. Can be either a @ref File or a @ref Directory.
class Entry
{
public:
    [[nodiscard]]
    XNOR_ENGINE explicit Entry(std::filesystem::path&& path);

    XNOR_ENGINE virtual ~Entry() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Entry)

    /// @brief Loads the contents of this @ref Entry.
    /// @returns <c>false</c> if an error occured while loading.
    XNOR_ENGINE virtual bool Load() = 0;

    XNOR_ENGINE virtual void Unload() = 0;

    /// @brief Reloads the contents of this @ref Entry.
    ///
    /// This if effectively the same as calling @ref Unload() and then @ref Load().
    XNOR_ENGINE virtual bool Reload();
    
    [[nodiscard]]
    XNOR_ENGINE const std::filesystem::path& GetPath() const;
    
    [[nodiscard]]
    XNOR_ENGINE std::string GetPathString() const;
    
    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;

    /// @brief Renames this @ref Entry on the file system.
    /// @param newName The new name of this @ref Entry.
    XNOR_ENGINE virtual void SetName(const std::string& newName);
    
    [[nodiscard]]
    XNOR_ENGINE bool GetLoaded() const;

    /// @brief Sets the new path of this @ref Entry.
    virtual void SetParent(Pointer<Directory>&& newParent);
    
    [[nodiscard]]
    XNOR_ENGINE Pointer<Directory> GetParent();

protected:
    std::filesystem::path m_Path;
    std::string m_Name;
    
    bool m_Loaded = false;

    Pointer<Directory> m_Parent;

    virtual void UpdateUtilityValues();
};

END_XNOR_CORE

// We cannot include directory.hpp above the class declaration, as the Directory class inherits from Entry.
// However, we also need to include this in every file in which we include entry.hpp, as the Entry class contains a Pointer<Directory>
// which needs the Directory class to be defined at some point so its destructor is defined as well
#include "file/directory.hpp"
