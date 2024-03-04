#include "scene/scene.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Scene::Begin()
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        m_Entities[i]->Begin();
    }
}

void Scene::Update()
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        m_Entities[i]->Update();
    }
}

Entity* Scene::GetEntityById(const Guid& xnorGuid)
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        if (m_Entities[i]->GetId() == xnorGuid)
        {
            return m_Entities[i];
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

    m_Entities.Add(e);

    return e;
}

void Scene::DestroyEntity(Entity* const entity)
{
    DestroyEntityChildren(entity);
    
    entity->SetParent(nullptr);
    
    delete entity;
}

bool Scene::HasEntity(const Entity* const entity) const
{
    return m_Entities.Contains(const_cast<Entity* const>(entity));
}

const List<Entity*>& Scene::GetEntities()
{
    return m_Entities;
}

void Scene::DestroyEntityChildren(Entity* const entity)
{
    // Remove from array
    m_Entities.Remove(entity);
    
    for (size_t i = 0; i < entity->GetChildCount(); i++)
    {
        Entity* const child = entity->GetChild(i);
            
        DestroyEntityChildren(child);
    }

    entity->m_Children.Clear();
}

Scene::~Scene()
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        delete m_Entities[i];
    }

    m_Entities.Clear();
}
