#include "reflection/xnor_factory.hpp"

#include "audio/component/audio_listener.hpp"
#include "audio/component/audio_source.hpp"
#include "physics/components/box_collider.hpp"
#include "physics/components/capsule_collider.hpp"
#include "physics/components/collider.hpp"
#include "physics/components/sphere_collider.hpp"
#include "rendering/light/directional_light.hpp"
#include "rendering/light/light.hpp"
#include "rendering/light/point_light.hpp"
#include "rendering/light/spot_light.hpp"
#include "scene/component/camera_component.hpp"
#include "scene/component/mesh_renderer.hpp"
#include "scene/component/script_component.hpp"
#include "scene/component/skinned_mesh_render.hpp"
#include "scene/component/test_component.hpp"
#include "scene/component/test_component_physics.hpp"

using namespace XnorCore;

inline void* XnorFactory::CreateObject(const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find type : {}", hash);
        return nullptr;
    }

    return it->second.createFunc();
}

inline void* XnorFactory::CreateObject(const std::string& name)
{
    auto&& it = m_FactoryMapName.find(name);

    if (it == m_FactoryMapName.end())
    {
        Logger::LogError("Couldn't find type : {}", name);
        return nullptr;
    }

    return it->second.createFunc();
}

inline void XnorFactory::DisplayObject(void* const obj, const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find type : {}", hash);
        return;
    }

    it->second.displayFunc(obj);
}

void XnorFactory::SerializeObject(void* const obj, const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find type : {}", hash);
        return;
    }

    it->second.serializeFunc(obj);
}

void XnorFactory::DeserializeObject(void* const obj, const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);

    if (it == m_FactoryMapHash.end())
    {
        Logger::LogError("Couldn't find type : {}", hash);
        return;
    }

    it->second.deserializeFunc(obj);
}

void XnorFactory::RegisterAllTypes()
{
    // Manually registering the types is sub-optimal, however, it's the best way I've found so far
    // It'll probably stay this way for the classes internal to Core, and as for the user scripts generated from the editor, a solution will be found at a later date probably
    
    RegisterType<MeshRenderer>();
    RegisterType<DirectionalLight>();
    RegisterType<TestComponent>();
    RegisterType<PointLight>();
    RegisterType<SpotLight>();
    RegisterType<ScriptComponent>();
    RegisterType<Collider>();
    RegisterType<BoxCollider>();
    RegisterType<SphereCollider>();
    RegisterType<CapsuleCollider>();
    RegisterType<TestComponentPhysics>();
    RegisterType<CameraComponent>();
    RegisterType<SkinnedMeshRender>();

    RegisterType<AudioListener>();
    RegisterType<AudioSource>();
}

std::string XnorFactory::GetTypeName(const size_t hash)
{
    auto&& it = m_FactoryMapHash.find(hash);
 
     if (it == m_FactoryMapHash.end())
     {
         Logger::LogError("Couldn't find type : {}", hash);
         return std::string();
     }
 
     return it->second.name;
}

void XnorFactory::Print()
{
    Logger::LogInfo("Number of types : {}", m_FactoryMapHash.size());
    for (auto& val : m_FactoryMapHash | std::views::values)
        Logger::LogInfo("{}", val.name);
}
