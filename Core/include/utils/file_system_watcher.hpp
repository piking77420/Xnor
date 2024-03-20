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
    Pointer<Entry> name;
    FileSystemWatcherChangeTypes::FileSystemWatcherChangeTypes changeTypes;
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

    explicit FileSystemWatcher(std::string path);

    FileSystemWatcher(std::string path, std::string filter);
};

END_XNOR_CORE
