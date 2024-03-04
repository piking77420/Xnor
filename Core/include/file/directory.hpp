#pragma once

#include <filesystem>

#include "core.hpp"
#include "file/entry.hpp"
#include "utils/pointer.hpp"

/// @file directory.hpp
/// @brief Defines the @ref Directory class.

BEGIN_XNOR_CORE

class File;

/// @brief Defines a directory on the filesystem.
///
/// This is meant to be used with @ref Pointer "Pointers" and with the @ref FileManager.
class Directory : public Entry
{
public:
    [[nodiscard]]
    XNOR_ENGINE explicit Directory(std::filesystem::path&& filepath);

    XNOR_ENGINE ~Directory() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Directory)

    /// @brief Loads the contents of this Directory in the FileManager
    XNOR_ENGINE bool Load() override;
    
    XNOR_ENGINE void Unload() override;

    [[nodiscard]]
    XNOR_ENGINE const std::vector<Pointer<Entry>>& GetChildEntries() const;

    [[nodiscard]]
    XNOR_ENGINE std::vector<Pointer<Entry>>& GetChildEntries();
    
    [[nodiscard]]
    XNOR_ENGINE const std::vector<Pointer<File>>& GetChildFiles() const;
    
    [[nodiscard]]
    XNOR_ENGINE std::vector<Pointer<File>>& GetChildFiles();
    
    [[nodiscard]]
    XNOR_ENGINE const std::vector<Pointer<Directory>>& GetChildDirectories() const;
    
    [[nodiscard]]
    XNOR_ENGINE std::vector<Pointer<Directory>>& GetChildDirectories();
    
    void SetName(const std::string& newName) override;

private:
    std::vector<Pointer<Entry>> m_ChildEntries;
    std::vector<Pointer<File>> m_ChildFiles;
    std::vector<Pointer<Directory>> m_ChildDirectories;
};

END_XNOR_CORE
