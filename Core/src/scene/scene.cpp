#include "scene.hpp"
#include "utils/logger.hpp"
#include "component.hpp"

using namespace XnorCore;

void Scene::Begin()
{
    for (Entity& ent : m_Entities)
    {
        ent.Begin();
    }
}

void Scene::Update()
{
    for (Entity& ent : m_Entities)
    {
        ent.Update();
    }
}

Entity* Scene::GetEntityById(const Guid& xnorGuid)
{
    for (Entity& ent : m_Entities)
    {
        if (ent.GetId() == xnorGuid)
        {
            return &ent;
        }
    }

    Logger::LogWarning("There is no entity with this id return nullptr ! ");

    return nullptr;
}

void Scene::CreateBasicScene()
{
    scene = new Scene();
    scene->m_Entities.emplace_back();
    scene->m_Entities[0].AddComponent<TestComponent>();
}
