#include "file/file_manager.hpp"

#include "utils/logger.hpp"

using namespace XnorCore;

Pointer<File> FileManager::Load(std::filesystem::path filepath)
{
    Logger::LogDebug("Loading file {}", filepath);

    if (Contains(filepath))
    {
        Logger::LogWarning("This has already been loaded, consider using FileManager::Get instead");
        return m_Files[filepath];
    }

    Pointer<File> file;
    try
    {
        file = Pointer<File>(std::forward<std::filesystem::path>(filepath));
    }
    catch (const std::invalid_argument& ex)
    {
        Logger::LogError("Exception while creating File object with path {}. Exception message: {}", filepath, ex.what());
        // Return the already-constructed null Pointer
        return file;
    }

    file->Load();

    m_Files[file->GetFilepath()] = file.CreateStrongReference();

    // Make sure to return a weak reference
    file.ToWeakReference();

    return file;
}

void FileManager::LoadDirectory(const std::filesystem::path& path)
{
    Logger::LogDebug("Loading directory {}", path);

    if (!is_directory(path))
    {
        Logger::LogError("Path does not point to a directory: {}", path);
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        const std::filesystem::path& entryPath = entry.path();
        
        if (is_directory(entryPath))
            continue;
        
        Load(entryPath);
    }
}

void FileManager::LoadDirectoryRecursive(const std::filesystem::path& path)
{
    Logger::LogDebug("Recursively loading directory {}", path);

    if (!is_directory(path))
    {
        Logger::LogError("Path does not point to a directory: {}", path);
        return;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
    {
        const std::filesystem::path& entryPath = entry.path();
        
        if (is_directory(entryPath))
            continue;
        
        Load(entryPath);
    }
}

bool FileManager::Contains(const std::filesystem::path& filepath)
{
    return m_Files.contains(filepath);
}

Pointer<File> FileManager::Get(const std::filesystem::path& filepath)
{
    if (!Contains(filepath))
    {
        Logger::LogError("Attempt to get an unknown file entry: {}", filepath);
        return Pointer<File>();
    }

    Pointer<File> file = m_Files.at(filepath);

    return file;
}

void FileManager::Unload(const std::filesystem::path& filepath)
{
    const size_t oldSize = m_Files.size();
    
    for (decltype(m_Files)::iterator it = m_Files.begin(); it != m_Files.end(); it++)
    {
        if (equivalent(it->first, filepath))
        {
            it->second->Unload();
            it = m_Files.erase(it);

            if (it == m_Files.end())
                break;
        }
    }
    
    if (oldSize == m_Files.size())
        Logger::LogWarning("Attempt to delete an unknown file entry: {}", filepath);
}

void FileManager::Unload(const Pointer<File>& file)
{
    const size_t oldSize = m_Files.size();
    
    for (decltype(m_Files)::iterator it = m_Files.begin(); it != m_Files.end(); it++)
    {
        if (it->second == file)
        {
            it->second->Unload();
            it = m_Files.erase(it);

            if (it == m_Files.end())
                break;
        }
    }
    
    /*if (oldSize == m_Files.size())
        Logger::LogWarning("Attempt to delete an unknown file entry: {}", static_cast<File*>(file));*/ // FIXME
}

void FileManager::UnloadAll()
{
    for (auto& entry : m_Files)
        entry.second->Unload();
    
    // Smart pointers are deleted automatically, we only need to clear the container
    m_Files.clear();
}
