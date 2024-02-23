#include "file/directory.hpp"

#include "file/file_manager.hpp"

using namespace XnorCore;

Directory::Directory(std::filesystem::path&& filepath)
    : Entry(std::move(filepath))
{
    if (!is_directory(m_Path))
        throw std::invalid_argument("Path does not point to a directory");
}

bool Directory::Load()
{
    for (const auto& entry : std::filesystem::directory_iterator(m_Path))
    {
        const std::filesystem::path& entryPath = entry.path();
        
        if (is_directory(entryPath))
        {
            Pointer<Directory> directory = FileManager::LoadDirectory(entryPath);
            m_Directories.push_back(directory);
            m_Entries.push_back(static_cast<Pointer<Entry>>(directory));
            continue;
        }

        Pointer<File> file = FileManager::Load(entryPath);
        m_Files.push_back(file);
        m_Entries.push_back(static_cast<Pointer<Entry>>(file));
    }
    
    m_Loaded = true;
    return true;
}

void Directory::Unload()
{
    m_Entries.clear();
    m_Files.clear();
    m_Directories.clear();
    m_Loaded = false;
}

const std::vector<Pointer<Entry>>& Directory::GetEntries() const
{
    return m_Entries;
}

std::vector<Pointer<Entry>>& Directory::GetEntries()
{
    return m_Entries;
}

const std::vector<Pointer<File>>& Directory::GetFiles() const
{
    return m_Files;
}

std::vector<Pointer<File>>& Directory::GetFiles()
{
    return m_Files;
}

const std::vector<Pointer<Directory>>& Directory::GetDirectories() const
{
    return m_Directories;
}

std::vector<Pointer<Directory>>& Directory::GetDirectories()
{
    return m_Directories;
}
