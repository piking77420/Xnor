#include "resource/resource.hpp"

#include "file/file.hpp"
#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "utils/formatter.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

Resource::Resource(std::string name)
    : m_Name(std::move(name))
    , m_Guid(Guid::New())
{
}

bool_t Resource::Load(const Pointer<File>& file)
{
    if (!file->GetLoaded())
        Logger::LogWarning("Tried to load resource {} with an unloaded file: {}", m_Name, file->GetPath());
    
    return Load(file->GetData<uint8_t>(), file->GetSize());
}

void Resource::CreateInRhi()
{
    m_LoadedInRhi = true;
}

void Resource::DestroyInRhi()
{
    m_LoadedInRhi = false;
}

bool_t Resource::Reload(const uint8_t* buffer, const int64_t length, const bool_t reloadInRhi)
{
    if (reloadInRhi)
        DestroyInRhi();
    Unload();
    const bool_t result = Load(buffer, length);
    if (reloadInRhi)
        CreateInRhi();
    return result;
}

bool_t Resource::Reload(const Pointer<File>& file, const bool_t reloadInRhi)
{
    if (reloadInRhi)
        DestroyInRhi();
    Unload();
    const bool_t result = Load(file);
    if (reloadInRhi)
        CreateInRhi();
    return result;
}

bool_t Resource::Reload(const bool_t reloadInRhi)
{
    return Reload(FileManager::Get(m_Name), reloadInRhi);
}

bool_t Resource::IsLoaded() const
{
    return m_Loaded;
}

bool_t Resource::IsLoadedInRhi() const
{
    return m_LoadedInRhi;
}

std::string Resource::GetName() const
{
    return m_Name;
}

void Resource::SetName(std::string newName)
{
    ResourceManager::Rename(m_Name, newName);
    
    m_Name = std::move(newName);
}

const Guid& Resource::GetGuid() const
{
    return m_Guid;
}
