#include "resource/resource.hpp"

#include "utils/logger.hpp"

using namespace XnorCore;

Resource::Resource(std::string name)
    : m_Name(std::move(name))
{
}

void Resource::Load(File& file)
{
    if (!file.IsLoaded())
        Logger::LogWarning("Tried to load resource %s with an unloaded file: %s", m_Name.c_str(), file.GetFilepath().string().c_str());
    
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
