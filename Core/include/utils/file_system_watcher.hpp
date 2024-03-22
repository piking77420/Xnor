#pragma once

#include "core.hpp"
#include "utils/event.hpp"

BEGIN_XNOR_CORE

BEGIN_ENUM(FileSystemWatcherChangeTypes)
{
    Changed = 1 << 0,
    Created = 1 << 1,
    Deleted = 1 << 2,
    Renamed = 1 << 3
}
END_ENUM

struct FileSystemWatcherEventArgs
{
    Pointer<Entry> entry;
    ENUM_VALUE(FileSystemWatcherChangeTypes) changeTypes;
};

struct RenamedFileSystemWatcherEventArgs : FileSystemWatcherEventArgs
{
    std::string oldName;
    std::string oldFullPath;
};

class FileSystemWatcher
{
public:
    Event<const FileSystemWatcherEventArgs&> changed;
    Event<const FileSystemWatcherEventArgs&> created;
    Event<const FileSystemWatcherEventArgs&> deleted;
    Event<const RenamedFileSystemWatcherEventArgs&> renamed;
    
    bool_t recursive = false;

    explicit FileSystemWatcher(const std::string& path);

    FileSystemWatcher(const std::string& path, std::string filter);

    ~FileSystemWatcher();

private:
    std::thread* m_Thread = nullptr;
    Pointer<Entry> m_Entry;
    std::string m_Filter;
    bool_t m_IsDirectory = false;

    bool_t m_Running = true;

    void Run();
};

END_XNOR_CORE
