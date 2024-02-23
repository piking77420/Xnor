#pragma once

#include <filesystem>
#include <map>

#include "core.hpp"
#include "file/directory.hpp"
#include "file/file.hpp"
#include "utils/logger.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

template<class T>
concept EntryT = std::is_base_of_v<Entry, T>;

class FileManager final
{
    STATIC_CLASS(FileManager)
    
public:
    /// @brief Creates the @ref File corresponding to the given @p path without loading it.
    XNOR_ENGINE static Pointer<File> Add(std::filesystem::path path);
    
    /// @brief Creates the @ref File corresponding to the given @p path and loads it.
    XNOR_ENGINE static Pointer<File> Load(std::filesystem::path path);
    
    /// @brief Creates a @ref Directory corresponding to the given @p path without loading it.
    XNOR_ENGINE static Pointer<Directory> AddDirectory(std::filesystem::path path);
    
    /// @brief Creates the @ref Directory corresponding to the given @p path and loads it.
    /// @see @ref Directory::Load
    XNOR_ENGINE static Pointer<Directory> LoadDirectory(std::filesystem::path path);

    [[nodiscard]]
    XNOR_ENGINE static bool Contains(const std::filesystem::path& path);

    template<EntryT T = File>
    [[nodiscard]]
    static Pointer<T> Get(const std::filesystem::path& path);

    XNOR_ENGINE static void Unload(const std::filesystem::path& path);

    XNOR_ENGINE static void Unload(const Pointer<Entry>& entry);

    XNOR_ENGINE static void UnloadAll();
    
private:
    XNOR_ENGINE static inline std::map<std::filesystem::path, Pointer<Entry>> m_Entries;
};

template<EntryT T>
Pointer<T> FileManager::Get(const std::filesystem::path& path)
{
    if (!Contains(path))
    {
        Logger::LogError("Attempt to get an unknown FileManager entry: {}", path);
        return Pointer<T>();
    }

    return static_cast<Pointer<T>>(m_Entries.at(path));
}

END_XNOR_CORE
