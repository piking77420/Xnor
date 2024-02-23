#include "scene/scene.hpp"
#include "scene/entity.hpp"
#include "utils/logger.hpp"

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
        // ent->transform.quaternion = Utils::GetQuaternionEulerAngles(ent->transform.rotation);
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

Entity* Scene::CreateEntity(const std::string&& name, Entity* parent)
{
    Entity* const e = new Entity();

    e->name = name;
    e->SetParent(parent);
    e->Begin();

    m_Entities.push_back(e);

    return e;
}

void Scene::DestroyEntity(Entity* const entity)
{
    DestroyEntityChildren(entity);
    
    entity->SetParent(nullptr);
    
    delete entity;
}

bool Scene::HasEntity(const Entity* entity)
{
    return std::ranges::find(std::as_const(m_Entities), entity) != m_Entities.cend();
}

void Scene::DestroyEntityChildren(Entity* const entity)
{
    // Remove from array
    m_Entities.erase(std::ranges::find(m_Entities, entity));
    
    for (size_t i = 0; i < entity->GetChildCount(); i++)
    {
        Entity* const child = entity->GetChild(i);
            
        DestroyEntityChildren(child);
    }

    entity->m_Children.clear();
}

const std::vector<Entity*>& Scene::GetEntities()
{
    return m_Entities;
}

Scene::~Scene()
{
    for (const Entity* ent : m_Entities)
    {
        delete ent;
    }
}
