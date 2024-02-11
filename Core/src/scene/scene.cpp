﻿#include "scene.hpp"

#include "component.hpp"

void Scene::Begin()
{
    for (Entity& ent : m_entities)
    {
        ent.Begin();
    }
}

void Scene::Update()
{
    for (Entity& ent : m_entities)
    {
        ent.Update();
    }
}

Entity* Scene::GetEntityByID(const XnorGuid& xnorGuid)
{
    for (Entity& ent : m_entities)
    {
        
        if(ent.GetID() == xnorGuid)
        {
            return &ent;
        }
    }

    return nullptr;
}

void Scene::CreateBasicScene()
{
    scene = new Scene();
    scene->m_entities.emplace_back(Entity());
    scene->m_entities[0].AddComponent<TestComponent>();
}

Scene::~Scene()
{
 
}
