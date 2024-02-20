#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "scene/component.hpp"
#include "utils/memory_leak_detector.hpp"
#include "scene/entity.hpp"

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

    Logger::LogWarning("No entity with id {} in scene", static_cast<std::string>(xnorGuid));

    return nullptr;
}

std::vector<Entity*>& Scene::GetEntities()
{
    return m_Entities;
}

void Scene::CreateBasicScene()
{
    scene = new Scene;

    for (size_t i = 0; i < 5; i++)
    {
        scene->m_Entities.push_back(new Entity);
        scene->m_Entities[i]->name = std::string("GameObject") + std::to_string(i);

        if (i != 0)
            scene->m_Entities[i]->SetParent(scene->m_Entities[i - 1]);
    }
}

Scene::~Scene()
{
    for (const Entity* ent : m_Entities)
    {
        delete ent;
    }
}
