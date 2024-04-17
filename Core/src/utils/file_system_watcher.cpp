#include "utils/file_system_watcher.hpp"

#include <chrono>
#include <regex>

#include "file/file_manager.hpp"
#include "utils/logger.hpp"
#include "utils/windows.hpp"

using namespace XnorCore;

FileSystemWatcher::FileSystemWatcher(const std::string& path)
{
    // Regex: https://regex101.com/r/3TLeFe/1
    // Matches any valid Windows filesystem path
    static const std::regex Regex(R"([a-zA-Z]:[\\\/](?:[a-zA-Z0-9]+[\\\/])*([^(|?\\\/":<>\*\n)]+))");

    if (!exists(m_Path))
    {
        // In case the path does not exist, make sure it is at least valid
        std::smatch results;
        if (!std::regex_match(path.begin(), path.end(), results, Regex))
        {
            Logger::LogError("Invalid path for FileSystemWatcher {}", path);
            throw std::runtime_error("Invalid path");
        }

        // Its parent directory must also exist
        if (!exists(m_Path.parent_path()))
        {
            Logger::LogError("The parent directory of the path {} for FileSystemWatcher does not exist", path);
            throw std::runtime_error("Parent directory of given path does not exist");
        }
    }

    m_Path = path;

    if (is_directory(m_Path))
        m_IsDirectory = true;
}

FileSystemWatcher::~FileSystemWatcher() { Stop(); }

void FileSystemWatcher::Start()
{
    if (m_Running)
        return;
    
    m_Thread = std::thread(&FileSystemWatcher::Run, this);
}

void FileSystemWatcher::Stop()
{
    if (!m_Running)
        return;
    
    m_Running = false;

    m_CondVar.notify_one();
    
    if (m_Thread.joinable())
        m_Thread.join();
}

void FileSystemWatcher::Run()
{
    std::unique_lock lock(m_Mutex);
    
    while (m_Running)
    {
        using namespace std::chrono_literals;

        const std::filesystem::path&& path = m_IsDirectory ? m_Path : m_Path.parent_path();
        const HANDLE file = CreateFileW(m_Path.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
        Windows::CheckError();

        constexpr size_t bufferSize = 1024;
        std::array<uint8_t, bufferSize> buffer{};
        ReadDirectoryChangesW(file, buffer.data(), bufferSize, false, FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, nullptr, nullptr, nullptr);
        Windows::CheckError();

        FILE_NOTIFY_INFORMATION* information = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer.data());
        
        m_CondVar.wait_for(lock, 500ms);
    }
}
