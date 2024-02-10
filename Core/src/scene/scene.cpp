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

void Scene::CreateBasicScene()
{
    scene = new Scene();
    scene->m_entities.emplace_back(new Entity());
}

Scene::~Scene()
{
    for (Entity* ent : m_entities)
    {
        delete ent;
    }
}
