#pragma once

#include "core.hpp"
#include "utils/event.hpp"

BEGIN_XNOR_CORE

/// @private
struct FileSystemWatcherEventArgs
{
    std::filesystem::path path;
};

/// @private
struct RenamedFileSystemWatcherEventArgs : FileSystemWatcherEventArgs
{
    std::filesystem::path oldPath;
};

/// @private
class FileSystemWatcher
{
public:
    Event<const FileSystemWatcherEventArgs&> modified;
    Event<const FileSystemWatcherEventArgs&> created;
    Event<const FileSystemWatcherEventArgs&> deleted;
    Event<const RenamedFileSystemWatcherEventArgs&> renamed;

    std::chrono::milliseconds updateRate{750};
    
    XNOR_ENGINE explicit FileSystemWatcher(const std::string& path);

    XNOR_ENGINE ~FileSystemWatcher();

    DELETE_COPY_MOVE_OPERATIONS(FileSystemWatcher)

    XNOR_ENGINE void Start();

    XNOR_ENGINE void Stop();

private:
    std::thread m_Thread;
    std::condition_variable m_CondVar;
    std::mutex m_Mutex;
    
    std::filesystem::path m_Path;
    bool_t m_IsDirectory = false;

    bool_t m_Running = false;

    void Run();
};

END_XNOR_CORE
