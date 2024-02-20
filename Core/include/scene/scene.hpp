﻿#pragma once

#include <vector>

#include "core.hpp"
#include "entity.hpp"

BEGIN_XNOR_CORE

class Scene
{
public:
    
    XNOR_ENGINE Scene() = default;

    XNOR_ENGINE ~Scene();

    DEFAULT_COPY_MOVE_OPERATIONS(Scene)

    template<class ComponentT>
    void GetAllComponentOfType(std::vector<const ComponentT*>*) const;

    template<class ComponentT>
    void GetAllComponentOfType(std::vector<ComponentT*>*);

    XNOR_ENGINE void Begin();

    XNOR_ENGINE void Update();

    XNOR_ENGINE Entity* GetEntityById(const Guid& xnorGuid);

    XNOR_ENGINE std::vector<Entity*>& GetEntities();
    
private:
    std::vector<Entity*> m_Entities;
};


template<class ComponentT>
void Scene::GetAllComponentOfType(std::vector<const ComponentT*>* componentData) const
{
    for (const Entity* ent : m_Entities)
    {
        const ComponentT* component = ent->GetComponent<ComponentT>();

        if (component != nullptr)
            componentData->emplace_back(component);
    }
}

template<class ComponentT>
void Scene::GetAllComponentOfType(std::vector<ComponentT*>* componentData)
{
    for (Entity* ent : m_Entities)
    {
        ComponentT* component = ent->GetComponent<ComponentT>();

        if (component != nullptr)
            componentData->emplace_back(component);
    }
}

END_XNOR_CORE
