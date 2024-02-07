#include "scene.hpp"

void Scene::Begin()
{
    for (Entity* ent : m_entities)
    {
        ent->Begin();
    }
}

void Scene::Update()
{
    for (Entity* ent : m_entities)
    {
        ent->Update();
    }
}

Scene::~Scene()
{
    for (Entity* ent : m_entities)
    {
        delete ent;
    }
}
