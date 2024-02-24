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

    /// @brief Finds a specific @ref Entry based on a predicate.
    /// @tparam T The type of @ref Entry to find.
    /// @param predicate The predicate used to find the correct @ref Entry. This function will be
    /// called for each stored @ref Entry.
    /// @return The first @ref Entry for which the @p predicate returned @c true. If every @ref Entry
    /// returned @c false, instead return a null @ref Pointer.
    template<EntryT T = File>
    [[nodiscard]]
    static Pointer<T> Find(std::function<bool(Pointer<T>)>&& predicate);
    
    /// @brief Finds a list of @ref Entry "Entries" based on a predicate.
    /// @tparam T The type of @ref Entry to find.
    /// @param predicate The predicate used to find the correct @ref Entry. This function will be
    /// called for each stored @ref Entry.
    /// @return The first @ref Entry for which the @p predicate returned @c true. If every @ref Entry
    /// returned @c false, instead return a null @ref Pointer.
    template<EntryT T = File>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll(std::function<bool(Pointer<T>)>&& predicate);

    /// @see @ref FileManager::FindAll(std::function<bool(Pointer<T>)>&&)
    template<EntryT T = File>
    static void FindAll(std::function<bool(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result);

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
        return nullptr;
    }

    return static_cast<Pointer<T>>(m_Entries.at(path));
}

template<EntryT T>
Pointer<T> FileManager::Find(std::function<bool(Pointer<T>)>&& predicate)
{
    for (auto&& mapEntry : m_Entries)
    {
        Pointer<Entry> entry = mapEntry.second;
        
        if (Utils::DynamicPointerCast<T>(entry) && predicate(entry))
            return entry;
    }

    return nullptr;
}

template<EntryT T>
std::vector<Pointer<T>> FileManager::FindAll(std::function<bool(Pointer<T>)>&& predicate)
{
    std::vector<Pointer<T>> result;
    FindAll<T>(predicate, &result);
    return result;
}

template<EntryT T>
void FileManager::FindAll(std::function<bool(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result)
{
    result->clear();
    
    for (auto&& mapEntry : m_Entries)
    {
        Pointer<Entry> entry = mapEntry.second;
        
        if (Utils::DynamicPointerCast<T>(entry) && predicate(entry))
            result->push_back(entry);
    }
}

END_XNOR_CORE
