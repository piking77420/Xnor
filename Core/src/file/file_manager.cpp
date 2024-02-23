#include "file/file_manager.hpp"

using namespace XnorCore;

Pointer<File> FileManager::Add(std::filesystem::path path)
{
    Logger::LogDebug("Adding file {}", path);

    if (Contains(path))
    {
        Logger::LogWarning("This file has already been added, consider using FileManager::Get instead");
        return static_cast<Pointer<File>>(m_Entries.at(path));
    }

    Pointer<File> file;
    try
    {
        file = Pointer<File>(std::forward<std::filesystem::path>(path));
    }
    catch (const std::invalid_argument& ex)
    {
        Logger::LogError("Exception while creating file with path {}: {}", path, ex.what());
        // Return the already-constructed null Pointer
        return file;
    }

    m_Entries[file->GetPath()] = file.CreateStrongReference();

    // Make sure to return a weak reference
    file.ToWeakReference();

    return file;
}

Pointer<File> FileManager::Load(std::filesystem::path path)
{
    Logger::LogDebug("Loading file {}", path);

    if (Contains(path))
    {
        Pointer<File> file = static_cast<Pointer<File>>(m_Entries.at(path));
        const bool loaded = file->GetLoaded();
        Logger::LogWarning(
            "This file has already been {}, consider using FileManager::Get instead{}",
            loaded ? "loaded" : "added but isn't loaded",
            loaded ? "" : ". Loading it"
        );
        if (!loaded)
            file->Load();
        return file;
    }

    Pointer<File> file;
    try
    {
        file = Pointer<File>(std::forward<std::filesystem::path>(path));
    }
    catch (const std::invalid_argument& ex)
    {
        Logger::LogError("Exception while creating file with path {}: {}", path, ex.what());
        // Return the already-constructed null Pointer
        return file;
    }

    file->Load();

    m_Entries[file->GetPath()] = file.CreateStrongReference();

    // Make sure to return a weak reference
    file.ToWeakReference();

    return file;
}

Pointer<Directory> FileManager::AddDirectory(std::filesystem::path path)
{
    Logger::LogDebug("Adding directory {}", path);

    if (Contains(path))
    {
        Logger::LogWarning("This directory has already been added, consider using FileManager::Get instead");
        return static_cast<Pointer<Directory>>(m_Entries.at(path));
    }

    Pointer<Directory> directory;
    try
    {
        directory = Pointer<Directory>(std::forward<std::filesystem::path>(path));
    }
    catch (const std::invalid_argument& ex)
    {
        Logger::LogError("Exception while creating directory with path {}: {}", directory->GetPath(), ex.what());
        // Return the already-constructed null Pointer
        return directory;
    }

    m_Entries[directory->GetPath()] = directory.CreateStrongReference();

    // Make sure to return a weak reference
    directory.ToWeakReference();

    return directory;
}

Pointer<Directory> FileManager::LoadDirectory(std::filesystem::path path)
{
    Logger::LogDebug("Loading directory {}...", path);

    auto&& start = std::chrono::system_clock::now();

    if (Contains(path))
    {
        Pointer<Directory> directory = static_cast<Pointer<Directory>>(m_Entries.at(path));
        const bool loaded = directory->GetLoaded();
        Logger::LogWarning(
            "This directory has already been {}, consider using FileManager::Get instead{}",
            loaded ? "loaded" : "added but isn't loaded",
            loaded ? "" : ". Loading it"
        );
        if (!loaded)
            directory->Load();
        return directory;
    }

    Pointer<Directory> directory;
    try
    {
        directory = Pointer<Directory>(std::forward<std::filesystem::path>(path));
    }
    catch (const std::invalid_argument& ex)
    {
        Logger::LogError("Exception while creating directory with path {}: {}", directory->GetPath(), ex.what());
        // Return the already-constructed null Pointer
        return directory;
    }

    directory->Load();

    m_Entries[directory->GetPath()] = directory.CreateStrongReference();

    // Make sure to return a weak reference
    directory.ToWeakReference();

    Logger::LogDebug("Directory {} load successful. Took {}", directory->GetPath(), std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start));

    return directory;
}

bool FileManager::Contains(const std::filesystem::path& path)
{
    return m_Entries.contains(path);
}

void FileManager::Unload(const std::filesystem::path& path)
{
    Logger::LogDebug("Unloading FileManager entry {}", path);

    if (!exists(path))
    {
        Logger::LogError("Path does not exist");
        return;
    }

    const size_t oldSize = m_Entries.size();
    
    for (decltype(m_Entries)::iterator it = m_Entries.begin(); it != m_Entries.end(); it++)
    {
        if (equivalent(it->first, path))
        {
            it->second->Unload();
            it = m_Entries.erase(it);

            if (it == m_Entries.end())
                break;
        }
    }
    
    if (oldSize == m_Entries.size())
        Logger::LogWarning("Attempt to delete an unknown FileManager entry: {}", path);
}

void FileManager::Unload(const Pointer<Entry>& entry)
{
    Logger::LogDebug("Unloading FileManager entry {}", entry);
    
    const size_t oldSize = m_Entries.size();
    
    for (decltype(m_Entries)::iterator it = m_Entries.begin(); it != m_Entries.end(); it++)
    {
        if (it->second == entry)
        {
            it->second->Unload();
            it = m_Entries.erase(it);

            if (it == m_Entries.end())
                break;
        }
    }
    
    if (oldSize == m_Entries.size())
        Logger::LogWarning("Attempt to delete an unknown FileManager entry: {}", entry);
}

void FileManager::UnloadAll()
{
    Logger::LogDebug("Unloading all FileManager entries...");

    auto&& start = std::chrono::system_clock::now();
    
    for (auto& entry : m_Entries)
    {
        Logger::LogDebug("Unloading FileManager entry {}", entry.first);
        
        entry.second->Unload();
    }

    // Smart pointers are deleted automatically, we only need to clear the container
    m_Entries.clear();

    Logger::LogDebug("FileManager unload successful. Took {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start));
}
