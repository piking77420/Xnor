#pragma once
#include <vcruntime_typeinfo.h>
#include <vector>

#include "component.hpp"


class Entity
{
public:
    
    template<class T>
    void AddComponent();
    
    template<class T>
    const T* GetComponent() const ;

    template<class T>
    T* GetComponent();

    template<class T>
    void RemoveComponent();

    template<class T>
    bool TryGetComponent(T*& output);

    ~Entity();

    void Begin();

    void Update();
    
private:
    std::vector<Component*> m_Components;
};

template <class T>
void Entity::AddComponent()
{
    static_assert(std::is_base_of<Component,T>(),"this class is not base on Component");

    m_Components.emplace_back(new T());
}

template <class T>
const T* Entity::GetComponent() const
{
    static_assert(std::is_base_of<Component,T>(),"this class is not base on Component");

    for (Component* comp: m_Components)
    {
        if(typeid(T) == typeid(comp))
        {
            return reinterpret_cast<T*>(comp);
        }
    }

    return nullptr;
}

template <class T>
T* Entity::GetComponent()
{
    static_assert(std::is_base_of<Component,T>(),"this class is not base on Component");

    for (Component* comp: m_Components)
    {
        if(dynamic_cast<T*>(comp) != nullptr)
        {
            return reinterpret_cast<T*>(comp);
        }
    }
    return nullptr;
}

template <class T>
void Entity::RemoveComponent()
{
    static_assert(std::is_base_of<Component,T>(),"this class is not base on Component");

    for (int i = 0; i < m_Components.size(); ++i)
    {
        if(dynamic_cast<T*>(m_Components[i]) != nullptr)
        {
            std::vector<Component*>::iterator iterator = std::vector<Component*>::begin() + i;
            m_Components.erase(iterator);
            return;
        }
    }
}

template <class T>
bool Entity::TryGetComponent(T*& output)
{
    static_assert(std::is_base_of<Component,T>(),"this class is not base on Component");
    for (Component* comp: m_Components)
    {
      
        if(dynamic_cast<T*>(comp) != nullptr)
        {
            output = reinterpret_cast<T*>(comp);
            return true;
        }
    }
    return false;
}
