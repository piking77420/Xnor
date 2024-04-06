#pragma once

#include "core.hpp"
#include "utils/event.hpp"

BEGIN_XNOR_CORE

/// @private
BEGIN_ENUM(FileSystemWatcherChangeTypes)
{
    Changed = 1 << 0,
    Created = 1 << 1,
    Deleted = 1 << 2,
    Renamed = 1 << 3
}
END_ENUM

/// @private
struct FileSystemWatcherEventArgs
{
    Pointer<Entry> entry;
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
    Event<const FileSystemWatcherEventArgs&> changed;
    Event<const FileSystemWatcherEventArgs&> created;
    Event<const FileSystemWatcherEventArgs&> deleted;
    Event<const RenamedFileSystemWatcherEventArgs&> renamed;
    
    bool_t recursive = false;

    explicit FileSystemWatcher(const std::string& path);

    ~FileSystemWatcher();

    DEFAULT_COPY_MOVE_OPERATIONS(FileSystemWatcher)

private:
    struct WatcherData
    {
        FileSystemWatcher* watcher;
        Pointer<Directory> directory;
        bool_t includeSubdirectories;
    };
    
    std::thread m_Thread;
    Pointer<Entry> m_Entry;
    bool_t m_IsDirectory = false;

    bool_t m_Running = true;

    void Run();
};

END_XNOR_CORE
