#include "utils/file_system_watcher.hpp"

#include <chrono>

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
    {
        m_Entry = FileManager::Get<Entry>(fPath);
    }
    else
    {
        if (m_IsDirectory)
            m_Entry = FileManager::AddDirectory(fPath);
        else
            m_Entry = FileManager::Add(fPath);
    }

    m_Thread = std::thread(&FileSystemWatcher::Run, this);
}

FileSystemWatcher::~FileSystemWatcher()
{
    m_Running = false;
    
    if (m_Thread.joinable())
        m_Thread.join();
}

void FileSystemWatcher::Run()
{
    while (m_Running)
    {
        using namespace std::chrono_literals;
        
        std::this_thread::sleep_for(500ms);
    }
}
