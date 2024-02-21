﻿#pragma once

#include <vector>

#include "core.hpp"
#include "transform.hpp"
#include "component.hpp"
#include "utils/guid.hpp"
#include "utils/reflectable.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

template<class T>
concept ComponentT = std::is_base_of_v<Component, T>;

class Entity : public Reflectable
{
    REFLECTABLE_IMPL(Entity)
    
public:
    Transform transform;
    std::string name;

    template<class ComponentT>
    void AddComponent();
    
    template<class ComponentT>
    [[nodiscard]]
    const ComponentT* GetComponent() const;

    template<class ComponentT>
    void GetComponents(std::vector<ComponentT*>* components);

    template<class ComponentT>
    void GetComponents(std::vector<const ComponentT*>* components) const;
    
    template<class ComponentT>
    ComponentT* GetComponent();
    
    template<class ComponentT>
    [[nodiscard]]
    bool TryGetComponent(ComponentT** output);

    template<class ComponentT>
    void RemoveComponent();

    [[nodiscard]]
    XNOR_ENGINE const Guid& GetId() const;

    [[nodiscard]]
    XNOR_ENGINE Entity* GetParent() const;
    [[nodiscard]]
    XNOR_ENGINE bool HasParent() const;
    [[nodiscard]]
    XNOR_ENGINE Entity* GetChild(size_t index);
    [[nodiscard]]
    XNOR_ENGINE size_t GetChildCount() const;
    [[nodiscard]]
    XNOR_ENGINE bool HasChildren() const;
    [[nodiscard]]
    XNOR_ENGINE bool IsAParentOf(const Entity* child) const;
    
    XNOR_ENGINE void SetParent(Entity* parent);
    XNOR_ENGINE void AddChild(Entity* child);
    XNOR_ENGINE void RemoveChild(Entity* child);
    
    XNOR_ENGINE void Begin() const;

    XNOR_ENGINE void Update() const;

    XNOR_ENGINE bool operator==(const Entity& entity) const;
    
private:
    XNOR_ENGINE explicit Entity(const Guid& entiyId);

    XNOR_ENGINE Entity() = default;

    XNOR_ENGINE ~Entity() override;
    
    Entity* m_Parent = nullptr;
    std::vector<Entity*> m_Children;
    
    Guid m_EntityId;

    List<Component*> m_Components;

    friend class Scene;
};

template <class ComponentT>
void Entity::AddComponent()
{
    Component* newT = new ComponentT();
    newT->entity = this;
    
    m_Components.Add(newT);
}

template <class ComponentT>
const ComponentT* Entity::GetComponent() const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<ComponentT*>(m_Components[i]))
            return reinterpret_cast<ComponentT*>(m_Components[i]);
    }

    return nullptr;
}

template <class ComponentT>
void Entity::GetComponents(std::vector<ComponentT*>* components)
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<ComponentT*>(m_Components[i]))
            components->push_back(reinterpret_cast<ComponentT*>(m_Components[i]));
    }
}

template <class ComponentT>
void Entity::GetComponents(std::vector<const ComponentT*>* components) const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<ComponentT*>(m_Components[i]))
            components->push_back(reinterpret_cast<const ComponentT*>(m_Components[i]));
    }
}

template <class ComponentT>
ComponentT* Entity::GetComponent()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<ComponentT*>(m_Components[i]))
            return reinterpret_cast<ComponentT*>(m_Components[i]);
    }
    return nullptr;
}

template <class ComponentT>
void Entity::RemoveComponent()
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (dynamic_cast<ComponentT*>(m_Components[i]))
        {
            m_Components.RemoveAt(i);
            break;
        }
    }
}

template <class ComponentT>
bool Entity::TryGetComponent(ComponentT** output)
{
    for (Component* comp: m_Components)
    {
        if (dynamic_cast<ComponentT*>(comp) != nullptr)
        {
            *output = reinterpret_cast<ComponentT*>(comp);
            return true;
        }
    }
    
    return false;
}

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::Entity),
    field(name),
    field(transform),
    field(m_Components)
)
