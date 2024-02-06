#pragma once
#include <vector>
#include "component.hpp"

class Component;

template<class T>
concept ComponentT = std::is_base_of_v<Component, T>;

class Entity
{
public:

    ~Entity();

    template<class ComponentT>
    void AddComponent();
    
    template<class ComponentT>
    const ComponentT* GetComponent() const;

    template<class ComponentT>
    ComponentT* GetComponent();

    template<class ComponentT>
    void RemoveComponent();

    template<class ComponentT>
    bool TryGetComponent(ComponentT** output);
    
    void Begin();

    void Update();
    
private:
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
        if (typeid(ComponentT) == typeid(comp))
        {
            return reinterpret_cast<ComponentT*>(comp);
        }
    }

    return nullptr;
}

template <class ComponentT>
ComponentT* Entity::GetComponent()
{
    for (Component* comp : m_Components)
    {
        if (dynamic_cast<ComponentT*>(comp) != nullptr)
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
        if (dynamic_cast<ComponentT*>(m_Components[i]) != nullptr)
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
