﻿#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "scene/component.hpp"

void XnorCore::Scene::Begin()
{
    for (Entity* ent : m_Entities)
    {
        ent->Begin();
    }
}

void XnorCore::Scene::Update()
{
    for (Entity* ent : m_Entities)
    {
        ent->Update();
    }
}

XnorCore::Entity* XnorCore::Scene::GetEntityById(const Guid& xnorGuid)
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

void XnorCore::Scene::CreateBasicScene()
{
    scene = new Scene;
}

XnorCore::Scene::~Scene()
{
    for (Entity* ent : m_Entities)
    {
        delete ent;
    }
}
