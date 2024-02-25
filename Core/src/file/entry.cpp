#include "file/entry.hpp"

#include "file/directory.hpp"
#include "file/file_manager.hpp"

using namespace XnorCore;

Entry::Entry(std::filesystem::path&& path)
    : m_Path(std::move(path))
{
    if (!exists(m_Path))
        throw std::invalid_argument("Path does not exist");
    
    m_Name = m_Path.filename().generic_string();
    const std::filesystem::path parent = m_Path.parent_path();
    if (FileManager::Contains(parent))
        m_Parent = static_cast<Directory*>(FileManager::Get<Directory>(parent));
}

const std::filesystem::path& Entry::GetPath() const
{
    return m_Path;
}

std::string Entry::GetPathString() const
{
    return m_Path.generic_string();
}

std::string Entry::GetName() const
{
    return m_Name;
}

bool Entry::GetLoaded() const
{
    return m_Loaded;
}

const Directory* Entry::GetParent() const
{
    return m_Parent;
}

Directory* Entry::GetParent()
{
    return m_Parent;
}
