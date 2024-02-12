#pragma once

#include <vector>

#include "core.hpp"
#include "transform.hpp"
#include "..\utils\guid.hpp"

class Component;

template<class T>
concept ComponentT = std::is_base_of_v<Component, T>;

class Entity
{
public:

    Entity* parent = nullptr;

    Transform transform;

    XNOR_ENGINE Entity(const Guid& entiyID);

    XNOR_ENGINE Entity();

    XNOR_ENGINE ~Entity();

    template<class ComponentT>
    void AddComponent();
    
    template<class ComponentT>
    const ComponentT* GetComponent() const;

    template<class ComponentT>
    void GetComponents(std::vector<ComponentT*>* components);

    template<class ComponentT>
    void GetComponents(std::vector<const ComponentT*>* components) const ;
    
    template<class ComponentT>
    ComponentT* GetComponent();
    
    template<class ComponentT>
    bool TryGetComponent(ComponentT** output);

    template<class ComponentT>
    void RemoveComponent();

    XNOR_ENGINE const Guid& GetID() const
    {
        return m_entityId;
    }
    
    XNOR_ENGINE void Begin();

    XNOR_ENGINE void Update();

    XNOR_ENGINE bool operator==(const Entity& entity);

    
private:

    Guid m_entityId;

    std::vector<Component*> m_Components;
};

template <class ComponentT>
void Entity::AddComponent()
{
    Component* newT = new ComponentT();
    newT->entity = this;
    
    m_Components.emplace_back(newT);
}

template <class ComponentT>
const ComponentT* Entity::GetComponent() const
{
    for (Component* comp: m_Components)
    {
        if (dynamic_cast<ComponentT*>(comp))
        {
            return reinterpret_cast<ComponentT*>(comp);
        }
    }

    return nullptr;
}

template <class ComponentT>
void Entity::GetComponents(std::vector<ComponentT*>* components)
{

    for (int i = 0; i < m_Components.size(); ++i)
    {
        if (dynamic_cast<ComponentT*>(m_Components[i]))
        {
            components->push_back(reinterpret_cast<ComponentT*>(&m_Components[i]));
        }
    }
}

template <class ComponentT>
void Entity::GetComponents(std::vector<const ComponentT*>* components) const
{
    for (int i = 0; i < m_Components.size(); ++i)
    {
        if (dynamic_cast<ComponentT*>(m_Components[i]))
        {
            components->push_back(reinterpret_cast<const ComponentT*>(&m_Components[i]));
        }
    }
}

template <class ComponentT>
ComponentT* Entity::GetComponent()
{
    for (Component* comp : m_Components)
    {
        if (dynamic_cast<ComponentT*>(comp))
        {
            return reinterpret_cast<ComponentT*>(comp);
        }
    }
    return nullptr;
}

template <class ComponentT>
void Entity::RemoveComponent()
{
    for (int i = 0; i < m_Components.size(); ++i)
    {
        if (dynamic_cast<ComponentT*>(m_Components[i]))
        {
            const std::vector<Component*>::iterator iterator = std::vector<Component*>::begin() + i;
            m_Components.erase(iterator);
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
