#include "scene/scene.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"
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

const std::vector<Entity*>& Scene::GetEntities()
{
    return m_Entities;
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


void Scene::GetEntities(std::vector<const Entity*>* entities) const
{
    if (entities == nullptr)
    {
        Logger::LogError("GetEntities entity vector is nullPtr");
        throw std::runtime_error("Entity vector is nullPtr");
    }
    
    for (const Entity* entPtr : m_Entities)
    {
        entities->push_back(entPtr);
    }
    
}

void Scene::Serialize() const
{
    Serializer::BeginRootElement("Scene", "SceneCaca");
    for (const Entity* ent : m_Entities)
    {
        ent->Serialize();
    }
    
    Serializer::EndRootElement();
}

void Scene::Deserialize()
{
   
}

Scene::~Scene()
{
    for (const Entity* ent : m_Entities)
    {
        delete ent;
    }
}
