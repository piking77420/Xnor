#pragma once

#include <vector>

#include "core.hpp"
#include "entity.hpp"

BEGIN_XNOR_CORE

class Scene : public Reflectable
{
    REFLECTABLE_IMPL(Scene)
    
public:
    XNOR_ENGINE Scene();

    XNOR_ENGINE ~Scene() override;

    DEFAULT_COPY_MOVE_OPERATIONS(Scene)

    template<class ComponentT>
    void GetAllComponentOfType(std::vector<const ComponentT*>*) const;

    template<class ComponentT>
    void GetAllComponentOfType(std::vector<ComponentT*>*);

    XNOR_ENGINE void Begin();

    XNOR_ENGINE void Update();
    
    [[nodiscard]]
    XNOR_ENGINE Entity* GetEntityById(const Guid& xnorGuid);

    XNOR_ENGINE Entity* CreateEntity(const std::string&& name, Entity* parent = nullptr);
    
    XNOR_ENGINE void DestroyEntity(Entity* entity);
    
    [[nodiscard]]
    XNOR_ENGINE bool HasEntity(const Entity* entity) const;

    [[nodiscard]]
    XNOR_ENGINE const List<Entity*>& GetEntities();

private:
    List<Entity*> m_Entities;

    XNOR_ENGINE void DestroyEntityChildren(Entity* entity);
};

template<class ComponentT>
void Scene::GetAllComponentOfType(std::vector<const ComponentT*>* componentData) const
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        const ComponentT* component = m_Entities[i]->GetComponent<ComponentT>();

        if (component != nullptr)
            componentData->push_back(component);
    }
}

template<class ComponentT>
void Scene::GetAllComponentOfType(std::vector<ComponentT*>* componentData)
{
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        ComponentT* component = m_Entities[i]->GetComponent<ComponentT>();

        if (component != nullptr)
            componentData->push_back(component);
    }
}

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Scene),
    field(m_Entities, XnorCore::ExpandPointer(), XnorCore::HideInInspector())
);
