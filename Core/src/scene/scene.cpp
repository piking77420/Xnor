﻿#include "scene/scene.hpp"

#include "resource/resource_manager.hpp"
#include "scene/entity.hpp"
#include "utils/logger.hpp"
#include "world/world.hpp"

using namespace XnorCore;

void Scene::Initialize()
{
    // TODO Make menu to select skybox
    skybox.Initialize();
    Pointer<Texture> texture = ResourceManager::Get<Texture>("assets/textures/puresky.hdr");
    texture->loadData.flipVertically = true;
    texture->Reload();
    skybox.LoadFromHdrTexture(texture);
}

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

void Scene::PrePhysics()
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        m_Entities[i]->PrePhysics();
    }
}

void Scene::PostPhysics()
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
        m_Entities[i]->PostPhysics();
}

Entity* Scene::FindEntityById(const Guid& xnorGuid)
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        if (m_Entities[i]->GetGuid() == xnorGuid)
            return m_Entities[i];
    }

    Logger::LogWarning("No entity with id {} in scene", static_cast<std::string>(xnorGuid));

    return nullptr;
}

Entity* Scene::FindEntityByName(const std::string& name)
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        if (m_Entities[i]->name == name)
            return m_Entities[i];
    }

    Logger::LogWarning("No entity with name {} in scene", name);

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

bool_t Scene::HasEntity(const Entity* const entity) const
{
    // TODO fix this thing
    return m_Entities.Contains(const_cast<Entity* const>(entity));
}

const List<Entity*>& Scene::GetEntities()
{
    return m_Entities;
}

uint32_t Scene::GetEntityIndex(const Entity* entity) const
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        if (entity->GetGuid() == m_Entities[i]->GetGuid())
        {
            return static_cast<uint32_t>(i);
        }
    }

    return std::numeric_limits<uint32_t>::max();
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
