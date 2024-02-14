#include "file/file_manager.hpp"

#include "utils/logger.hpp"

using namespace XnorCore;

Pointer<File> FileManager::Load(std::filesystem::path filepath)
{
    Pointer<File> file;
    try
    {
        file = Pointer<File>(std::forward<std::filesystem::path>(filepath));
    }
    catch (const std::invalid_argument& ex)
    {
        Logger::LogError("Uncaught exception while creating File object: %s", ex.what());
        // Return the already-constructed null Pointer
        return file;
    }

    m_Files[file->GetFilepath()] = file.CreateStrongReference();

    // Make sure to return a weak reference
    file.ToWeakReference();

    return file;
}

bool FileManager::Contains(const std::filesystem::path& filepath)
{
    return m_Files.contains(filepath);
}

Pointer<File> FileManager::Get(const std::filesystem::path& filepath)
{
    if (!Contains(filepath))
    {
        Logger::LogError("Attempt to get an unknown file entry: %s", filepath.c_str());
        return Pointer<File>();
    }

    Pointer<File> file = m_Files.at(filepath);

    return file;
}

void FileManager::Delete(const std::filesystem::path& filepath)
{
    const size_t oldSize = m_Files.size();
    
    for (decltype(m_Files)::iterator it = m_Files.begin(); it != m_Files.end(); it++)
    {
        if (equivalent(it->first, filepath))
        {
            it = m_Files.erase(it);

            if (it == m_Files.end())
                break;
        }
    }
    
    if (oldSize == m_Files.size())
        Logger::LogWarning("Attempt to delete an unknown file entry: %s", filepath.string().c_str());
}

void FileManager::Delete(const Pointer<File>& file)
{
    const size_t oldSize = m_Files.size();
    
    for (decltype(m_Files)::iterator it = m_Files.begin(); it != m_Files.end(); it++)
    {
        if (it->second == file)
        {
            it = m_Files.erase(it);

            if (it == m_Files.end())
                break;
        }
    }
    
    if (oldSize == m_Files.size())
        Logger::LogWarning("Attempt to delete an unknown file entry: %p", static_cast<File*>(file));
}

void FileManager::DeleteAll()
{
    // Smart pointers are deleted automatically, we only need to clear the container
    m_Files.clear();
}
