#pragma once

#include "core.hpp"
#include "utils/event.hpp"

BEGIN_XNOR_CORE

/// @private
BEGIN_ENUM(FileSystemWatcherChangeTypes)
{
    Modified = 1 << 0,
    Created = 1 << 1,
    Deleted = 1 << 2,
    Renamed = 1 << 3
}
END_ENUM

/// @private
struct FileSystemWatcherEventArgs
{
    std::filesystem::path path;
    ENUM_VALUE(FileSystemWatcherChangeTypes) changeTypes;
};

/// @private
struct RenamedFileSystemWatcherEventArgs : FileSystemWatcherEventArgs
{
    std::string oldName;
    std::string oldFullPath;
};

/// @private
class FileSystemWatcher
{
public:
    Event<const FileSystemWatcherEventArgs&> modified;
    Event<const FileSystemWatcherEventArgs&> created;
    Event<const FileSystemWatcherEventArgs&> deleted;
    Event<const RenamedFileSystemWatcherEventArgs&> renamed;
    
    explicit FileSystemWatcher(const std::string& path);

    ~FileSystemWatcher();

    DELETE_COPY_MOVE_OPERATIONS(FileSystemWatcher)

    void Start();

    void Stop();

private:
    std::thread m_Thread;
    std::condition_variable m_CondVar;
    std::mutex m_Mutex;
    
    std::filesystem::path m_Path;
    bool_t m_IsDirectory = false;

    bool_t m_Running = true;

    void Run();
};

END_XNOR_CORE
