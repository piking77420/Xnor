#include "utils/file_system_watcher.hpp"

#include "file/file_manager.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

FileSystemWatcher::FileSystemWatcher(const std::string& path)
{
    std::filesystem::path fPath = path;
    if (!exists(fPath))
    {
        Logger::LogError("Invalid path for FileSystemWatcher {}", fPath);
        throw std::invalid_argument("FileSystemWatcher path does not exist");
    }

    if (is_directory(fPath))
        m_IsDirectory = true;

    if (FileManager::Contains(fPath))
        m_Entry = FileManager::Get<Entry>(fPath);
    else
        m_Entry = m_IsDirectory ? static_cast<Pointer<Entry>>(FileManager::AddDirectory(fPath)) : static_cast<Pointer<Entry>>(FileManager::Add(fPath));

    m_Thread = new std::thread(Run);
}

FileSystemWatcher::FileSystemWatcher(const std::string& path, std::string filter)
    : FileSystemWatcher(path)
{
    m_Filter = std::move(filter);
}

FileSystemWatcher::~FileSystemWatcher()
{
    m_Running = false;
    
    if (m_Thread->joinable())
        m_Thread->join();
    
    delete m_Thread;
}

void FileSystemWatcher::Run()
{
    while (m_Running)
    {
        // TODO
    }
}
