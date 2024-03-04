#pragma once

#include <filesystem>
#include <map>

#include "core.hpp"
#include "file/directory.hpp"
#include "file/file.hpp"
#include "utils/logger.hpp"
#include "utils/pointer.hpp"

/// @file file_manager.hpp
/// @brief Defines the @ref FileManager class.

BEGIN_XNOR_CORE

/// @brief A class satisfies the @ref EntryT concept if it is derived of @ref Entry.
template <class T>
concept EntryT = std::is_base_of_v<Entry, T>;

/// @brief Static class used to add, load, get, or unload @ref File "Files" and @ref Directory "Directories".
///
/// It contains all wrapper instances of the @ref File and @ref Directory classes. These are either added or loaded using the corresponding
/// function: @ref FileManager::Add and @ref FileManager::Load for @ref File "Files",and @ref FileManager::AddDirectory and
/// @ref FileManager::LoadDirectory for @ref Directory "Directories".
///
/// @paragraph usage Usage
/// 
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

    /// @brief Tries to get the @ref Entry with the given @p path.
    /// @tparam T The type of @ref Entry to get.
    /// @param path The system path to get the @ref Entry of.
    /// @return The @ref Entry corresponding to the given @p path, or nullptr if it hasn't been added yet.
    template <EntryT T = File>
    [[nodiscard]]
    static Pointer<T> Get(const std::filesystem::path& path);

    XNOR_ENGINE static void Rename(const std::filesystem::path& path, const std::filesystem::path& newPath);

    XNOR_ENGINE static void Rename(const Pointer<Entry>& entry, const std::filesystem::path& newPath);

    /// @brief Finds the first @ref Entry of type @p T.
    /// @tparam T The type of @ref Entry to find.
    /// @return The first @ref Entry of type @p T.
    template <EntryT T = File>
    [[nodiscard]]
    static Pointer<T> Find();

    /// @brief Finds a specific @ref Entry based on a predicate.
    /// @tparam T The type of @ref Entry to find.
    /// @param predicate The predicate used to find the correct @ref Entry. This function will be
    /// called for each stored @ref Entry.
    /// @return The first @ref Entry for which the @p predicate returned @c true. If every @ref Entry
    /// returned @c false, instead return a null @ref Pointer.
    template <EntryT T = File>
    [[nodiscard]]
    static Pointer<T> Find(std::function<bool_t(Pointer<T>)>&& predicate);

    /// @brief Finds all @ref Entry "Entries" of type @p T.
    /// @tparam T The type of @ref Entry to find.
    /// @return All stored @ref Entry "Entries" of type @p T.
    template <EntryT T = File>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll();
    
    /// @see @ref FileManager::FindAll()
    template <EntryT T = File>
    static void FindAll(std::vector<Pointer<T>>* result);
    
    /// @brief Finds a list of @ref Entry "Entries" based on a predicate.
    /// @tparam T The type of @ref Entry to find.
    /// @param predicate The predicate used to find the correct @ref Entry. This function will be
    /// called for each stored @ref Entry.
    /// @return The first @ref Entry for which the @p predicate returned @c true. If every @ref Entry
    /// returned @c false, instead return a null @ref Pointer.
    template <EntryT T = File>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll(std::function<bool_t(Pointer<T>)>&& predicate);

    /// @see @ref FileManager::FindAll(std::function<bool(Pointer<T>)>&&)
    template <EntryT T = File>
    static void FindAll(std::function<bool(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result);

    XNOR_ENGINE static void Unload(const std::filesystem::path& path);

    XNOR_ENGINE static void Unload(const Pointer<Entry>& entry);

    XNOR_ENGINE static void UnloadAll();
    
private:
    XNOR_ENGINE static inline std::map<std::filesystem::path, Pointer<Entry>> m_Entries;
};

template <EntryT T>
Pointer<T> FileManager::Get(const std::filesystem::path& path)
{
    if (!Contains(path))
    {
        Logger::LogError("Attempt to get an unknown FileManager entry: {}", path);
        return nullptr;
    }

    return static_cast<Pointer<T>>(m_Entries.at(path));
}

template <EntryT T>
Pointer<T> FileManager::Find()
{
    for (auto&& mapEntry : m_Entries)
    {
        Pointer<Entry> entry = mapEntry.second;
        
        if (Utils::DynamicPointerCast<T>(entry))
            return entry;
    }

    return nullptr;
}

template <EntryT T>
Pointer<T> FileManager::Find(std::function<bool_t(Pointer<T>)>&& predicate)
{
    for (auto&& mapEntry : m_Entries)
    {
        Pointer<Entry> entry = mapEntry.second;
        
        if (Utils::DynamicPointerCast<T>(entry) && predicate(entry))
            return entry;
    }

    return nullptr;
}

template <EntryT T>
std::vector<Pointer<T>> FileManager::FindAll()
{
    std::vector<Pointer<T>> result;
    FindAll<T>(&result);
    return result;
}

template <EntryT T>
void FileManager::FindAll(std::vector<Pointer<T>>* result)
{
    result->clear();
    
    for (auto&& mapEntry : m_Entries)
    {
        Pointer<T> entry = Utils::DynamicPointerCast<T>(mapEntry.second);
        
        if (entry)
            result->push_back(std::move(entry));
    }
}

template <EntryT T>
std::vector<Pointer<T>> FileManager::FindAll(std::function<bool_t(Pointer<T>)>&& predicate)
{
    std::vector<Pointer<T>> result;
    FindAll<T>(predicate, &result);
    return result;
}

template <EntryT T>
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
