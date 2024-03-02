#include "file/entry.hpp"

#include "file/directory.hpp"
#include "file/file_manager.hpp"

using namespace XnorCore;

Entry::Entry(std::filesystem::path&& path)
    : m_Path(std::move(path))
{
    if (!exists(m_Path))
        throw std::invalid_argument("Path does not exist");
    
    Entry::UpdateUtilityValues();
}

bool Entry::Reload()
{
    if (m_Loaded)
        Unload();
    return Load();
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

void Entry::SetName(const std::string& newName)
{
    std::filesystem::path newPath = m_Path.parent_path().string() + static_cast<char_t>(std::filesystem::path::preferred_separator) + newName;
    std::filesystem::rename(m_Path, newPath);

    FileManager::Rename(m_Path, newPath);

    m_Path = std::move(newPath);

    UpdateUtilityValues();
}

bool Entry::GetLoaded() const
{
    return m_Loaded;
}

const Directory* Entry::GetParent() const
{
    return m_Parent;
}

void Entry::SetParent(Directory* newParent)
{
    m_Parent = newParent;
    std::filesystem::path newPath = m_Parent->GetPathString() + static_cast<char_t>(std::filesystem::path::preferred_separator) + m_Name;

    FileManager::Rename(m_Path, newPath);

    m_Path = std::move(newPath);
}

Directory* Entry::GetParent()
{
    return m_Parent;
}

void Entry::UpdateUtilityValues()
{
    m_Name = m_Path.filename().generic_string();
    const std::filesystem::path parent = m_Path.parent_path();
    if (FileManager::Contains(parent))
        m_Parent = static_cast<Directory*>(FileManager::Get<Directory>(parent));
}
