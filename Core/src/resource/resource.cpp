#include "resource/resource.hpp"

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

    m_File = file;
    
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

Pointer<File> Resource::GetFile() const
{
    return m_File;
}

const Guid& Resource::GetGuid() const
{
    return m_Guid;
}
