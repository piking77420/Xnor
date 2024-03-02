#include "resource/resource.hpp"

#include "file/file.hpp"
#include "resource/resource_manager.hpp"
#include "utils/formatter.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

Resource::Resource(std::string name)
    : m_Name(std::move(name))
    , m_Guid(Guid::New())
{
}

void Resource::Load(const Pointer<File>& file)
{
    if (!file->GetLoaded())
        Logger::LogWarning("Tried to load resource {} with an unloaded file: {}", m_Name, file->GetPath());
    
    Load(file->GetData<uint8_t>(), file->GetSize());
}

void Resource::CreateInRhi()
{
}

void Resource::DestroyInRhi()
{
}

bool_t Resource::GetLoaded() const
{
    return m_Loaded;
}

bool_t Resource::GetLoadedInRhi() const
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
