#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "scene/component.hpp"

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

    Logger::LogWarning("There is no entity with this id return nullptr ! ");

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
