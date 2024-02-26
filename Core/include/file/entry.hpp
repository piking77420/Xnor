#pragma once

#include <filesystem>

#include "core.hpp"

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
    /// This if effectively the same as call @ref Unload() and then @ref Load().
    XNOR_ENGINE virtual bool Reload();
    
    [[nodiscard]]
    XNOR_ENGINE const std::filesystem::path& GetPath() const;
    
    [[nodiscard]]
    XNOR_ENGINE std::string GetPathString() const;
    
    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;

    XNOR_ENGINE virtual void SetName(std::string newName);
    
    [[nodiscard]]
    XNOR_ENGINE bool GetLoaded() const;
    
    [[nodiscard]]
    XNOR_ENGINE const Directory* GetParent() const;
    
    [[nodiscard]]
    XNOR_ENGINE Directory* GetParent();

protected:
    std::filesystem::path m_Path;
    std::string m_Name;
    
    bool m_Loaded = false;

    Directory* m_Parent;
};

END_XNOR_CORE
