#pragma once

#include <vector>

#include "core.hpp"
#include "entity.hpp"

BEGIN_XNOR_CORE

class Scene
{
public:
    // TO DO test to virtual memory between dll and editor exe
    // REMOVE IT LATER
    XNOR_ENGINE static void CreateBasicScene();

    // If XNOR_ENGINE so you can export it to the editor
    XNOR_ENGINE static inline Scene* scene = nullptr;

    XNOR_ENGINE ~Scene() = default;

    template<class ComponentT>
    void GetAllComponentOfType(std::vector<const ComponentT*>*) const ;

    template<class ComponentT>
    void GetAllComponentOfType(std::vector<ComponentT*>*);

    XNOR_ENGINE void Begin();

    XNOR_ENGINE void Update();

    XNOR_ENGINE Entity* GetEntityById(const Guid& xnorGuid);
    
private:
    std::vector<Entity> m_Entities;
};


template<class ComponentT>
void Scene::GetAllComponentOfType(std::vector<const ComponentT*>* componentData) const
{
    for (const Entity& ent : m_Entities)
    {
        const ComponentT* component = ent.GetComponent<ComponentT>();

        if (dynamic_cast<const Component*>(component) == nullptr)
            continue;
        componentData->emplace_back(component);

    }

}

template<class ComponentT>
void Scene::GetAllComponentOfType(std::vector<ComponentT*>* componentData)
{
    for (Entity& ent : m_Entities)
    {
        ComponentT* component = ent.GetComponent<ComponentT>();

        if (dynamic_cast<Component*>(component) == nullptr)
            continue;
        componentData->emplace_back(component);

    }

}

END_XNOR_CORE
