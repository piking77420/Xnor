#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "scene/component.hpp"
#include "utils/memory_leak_detector.hpp"

using namespace XnorCore;

void Scene::Begin()
{
    for (Entity* ent : m_Entities)
    {
        ent->Begin();
    }
}

void Scene::Update()
{
    for (Entity* ent : m_Entities)
    {
        ent->Update();
    }
}

Entity* Scene::GetEntityById(const Guid& xnorGuid)
{
    for (Entity* ent : m_Entities)
    {
        if (ent->GetId() == xnorGuid)
        {
            return ent;
        }
    }

    Logger::LogWarning("No entity with id %s in scene", static_cast<std::string>(xnorGuid).c_str());

    return nullptr;
}

void Scene::CreateBasicScene()
{
    scene = new Scene;
}

Scene::~Scene()
{
    for (Entity* ent : m_Entities)
    {
        delete ent;
    }
}
