#include "resource/resource.hpp"

#include "utils/logger.hpp"

using namespace XnorCore;

Resource::Resource(std::string name)
    : m_Name(std::move(name))
{
}

void Resource::Load(File& file)
{
    if (!file.GetLoaded())
        Logger::LogWarning("Tried to load resource {} with an unloaded file: {}", m_Name, file.GetPath());
    
    Load(file.GetData<uint8_t>(), file.GetSize());
}

bool Resource::IsLoaded() const
{
    return m_Loaded;
}

std::string Resource::GetName() const
{
    return m_Name;
}
