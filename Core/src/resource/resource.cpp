#include "resource/resource.hpp"

using namespace XnorCore;

Resource::Resource(std::string name)
    : m_Name(std::move(name))
{
}

void Resource::Load(File& file)
{
    const bool wasLoaded = file.IsLoaded();
    if (!wasLoaded)
        file.Load();
    
    Load(file.GetData<uint8_t>(), file.GetSize());

    if (!wasLoaded)
        file.Unload();
}

bool Resource::IsLoaded() const
{
    return m_Loaded;
}

std::string Resource::GetName() const
{
    return m_Name;
}
