﻿#pragma once

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

    /// @returns <c>false</c> if an error occured while loading.
    XNOR_ENGINE virtual bool Load() = 0;

    XNOR_ENGINE virtual void Unload() = 0;
    
    [[nodiscard]]
    XNOR_ENGINE const std::filesystem::path& GetPath() const;
    
    [[nodiscard]]
    XNOR_ENGINE std::string GetPathString() const;
    
    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;
    
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
