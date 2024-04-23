#include "utils/file_system_watcher.hpp"

#include <regex>

#include "file/file_manager.hpp"
#include "utils/logger.hpp"
#include "utils/windows.hpp"

using namespace XnorCore;

FileSystemWatcher::FileSystemWatcher(const std::string& path)
{
    SetPath(path);
}

FileSystemWatcher::~FileSystemWatcher() { Stop(); }

void FileSystemWatcher::Start()
{
    if (m_Running)
        return;

    m_Running = true;
    
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

void FileSystemWatcher::Update() { m_CondVar.notify_one(); }

std::filesystem::path FileSystemWatcher::GetPath() const { return m_Path; }

void FileSystemWatcher::SetPath(const std::filesystem::path& newPath)
{
    // Regex: https://regex101.com/r/cV0Bbz/1
    // Matches any valid Windows filesystem path
    static const std::regex Regex(R"((?:[a-zA-Z]:[\\\/](?:[a-zA-Z0-9]+[\\\/])*([^(|?\\\/":<>\*\n)]+))|(?:[a-zA-Z0-9]+[\\\/])*([^(|?\\\/":<>\*\n)]+))");

    // We might want to watch for a path that will soon be created
    if (!exists(newPath))
    {
        // In case the path does not exist, make sure it is at least valid
        std::smatch results;
        const std::string pathStr = newPath.string();
        if (!std::regex_match(pathStr.cbegin(), pathStr.cend(), results, Regex))
            throw std::runtime_error("Invalid path");

        // Its parent directory must also exist
        if (!exists(newPath.parent_path()))
            throw std::runtime_error("Parent directory of given path does not exist");
    }

    m_IsDirectory = is_directory(newPath);

    m_Path = newPath;

    m_PathChanged = true;
}

void FileSystemWatcher::Run()
{
    (void) SetThreadDescription(m_Thread.native_handle(), L"FileSystemWatcher Thread");
    Windows::CheckError();
    
    std::unique_lock lock(m_Mutex);

    std::filesystem::path watchedPath;
    std::string pathAbs;
    
    HANDLE file = nullptr;
    OVERLAPPED overlapped;
    
    while (m_Running)
    {
        if (m_PathChanged)
        {
            const std::filesystem::path&& abs = absolute(m_Path);
            watchedPath = abs.parent_path();
            pathAbs = abs.string();
            
            file = CreateFileW(watchedPath.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr);
            Windows::CheckError();
            
            overlapped.hEvent = CreateEventW(nullptr, FALSE, 0, nullptr);
            Windows::CheckError();
        }
        
        constexpr size_t bufferSize = 1024;
        std::array<uint8_t, bufferSize> buffer{};
        ReadDirectoryChangesW(file, buffer.data(), bufferSize, recursive, FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, nullptr, &overlapped, nullptr);
        Windows::SilenceError(); // Windows would return an error because the 0ms timeout of WaitForSingleObject expired
        
        m_CondVar.wait_for(lock, updateRate);

        const DWORD waitResult = WaitForSingleObject(overlapped.hEvent, 0);
        Windows::SilenceError(); // Same here

        if (waitResult == WAIT_OBJECT_0)
        {
            DWORD bytesTransferred = 0;
            GetOverlappedResult(file, &overlapped, &bytesTransferred, FALSE);
            Windows::CheckError();
            
            const FILE_NOTIFY_INFORMATION* information = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer.data());
            FswRenamedEventArgs renamedArgs; // We need this to be defined outside the next loop because the renamed event is in two steps: old name and new name
            
            while (true)
            {
                const std::filesystem::path path = std::wstring_view(information->FileName, information->FileNameLength / sizeof(WCHAR));

                // Because we watch the parent directory, we need to check if the changed entry is the correct one
                if (absolute(path).string().starts_with(pathAbs))
                {
                    switch (information->Action)
                    {
                        case FILE_ACTION_ADDED:
                            created({path});
                            break;

                        case FILE_ACTION_REMOVED:
                            deleted({path});
                            break;

                        case FILE_ACTION_MODIFIED:
                            modified({path});
                            break;

                        case FILE_ACTION_RENAMED_OLD_NAME:
                            renamedArgs.oldPath = path;
                            break;

                        case FILE_ACTION_RENAMED_NEW_NAME:
                            renamedArgs.path = path;
                            renamed(renamedArgs);
                            break;

                        default: ;
                    }
                }

                // Are there more events to handle?
                if (information->NextEntryOffset)
                    *reinterpret_cast<const uint8_t**>(&information) += information->NextEntryOffset;  // NOLINT(clang-diagnostic-undefined-reinterpret-cast)
                else
                    break;
            }
        }
    }
}

DWORD FileSystemWatcher::NotifyFiltersToWindows(const ENUM_VALUE(FswNotifyFilters) filters)
{
    DWORD result = 0;

    if (filters & FswNotifyFilters::FileName)
        result |= FILE_NOTIFY_CHANGE_FILE_NAME;

    // TODO: Complete function

    return result;
}
