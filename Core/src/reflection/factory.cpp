#include "reflection/factory.hpp"

#include "scene/component/script_component.hpp"

using namespace XnorCore;

inline void* Factory::CreateObject(const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find type : {}", hash);
        return nullptr;
    }

    return it->second.createFunc();
}

inline void* Factory::CreateObject(const char_t* name)
{
    auto&& it = m_FactoryMapName.find(name);

    if (it == m_FactoryMapName.end())
    {
        Logger::LogError("Couldn't find type : {}", name);
        return nullptr;
    }

    return it->second.createFunc();
}

inline void Factory::DisplayObject(void* const obj, const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find type : {}", hash);
        return;
    }

    it->second.displayFunc(obj);
}

void Factory::RegisterTypes()
{
    // Manually registering the types is sub-optimal, however, it's the best way I've found so far
    // It'll probably stay this way for the classes internal to Core, and as for the user scripts generated from the editor, a solution will be found at a later date probably
    
    RegisterFactoryType<MeshRenderer>();
    RegisterFactoryType<Light>();
    RegisterFactoryType<DirectionalLight>();
    RegisterFactoryType<TestComponent>();
    RegisterFactoryType<PointLight>();
    RegisterFactoryType<SpotLight>();
    RegisterFactoryType<ScriptComponent>();
}

void Factory::Print()
{
    Logger::LogInfo("Number of types : {}", m_FactoryMapHash.size());
    for (auto& val : m_FactoryMapHash | std::views::values)
        Logger::LogInfo("{}", val.name);
}

const char_t* Factory::GetName(const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find type : {}", hash);
        return nullptr;
    }

    return it->second.name;
}
