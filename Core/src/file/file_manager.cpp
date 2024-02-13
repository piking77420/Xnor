#include "file/file_manager.hpp"

#include "utils/logger.hpp"

using namespace XnorCore;

Pointer<File> FileManager::Load(std::filesystem::path filepath)
{
    Pointer<File> file(std::forward<std::filesystem::path>(filepath));

    m_Files[file->GetFilepath()] = file.CreateStrongReference();

    // Make sure to return a weak reference
    file.ToWeakReference();

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
