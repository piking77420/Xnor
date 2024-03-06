#pragma once

BEGIN_XNOR_CORE

template <class ComponentT>
ComponentT* Entity::AddComponent()
{
    m_Components.Add();
    
    ComponentT* newT = new ComponentT;
    newT->entity = this;
    m_Components[m_Components.GetSize() - 1].Create(newT);

    return newT;
}

template <class ComponentT>
const ComponentT* Entity::GetComponent() const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
            return m_Components[i].Cast<ComponentT>();
    }

    return nullptr;
}

template <class ComponentT>
void Entity::GetComponents(std::vector<ComponentT*>* components)
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
            components->push_back(m_Components[i].Cast<ComponentT>());
    }
}

template <class ComponentT>
void Entity::GetComponents(std::vector<const ComponentT*>* components) const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
            components->push_back(reinterpret_cast<const ComponentT*>(m_Components[i]));
    }
}

template <class ComponentT>
ComponentT* Entity::GetComponent()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
            return m_Components[i].Cast<ComponentT>();
    }
    
    return nullptr;
}

template <class ComponentT>
void Entity::RemoveComponent()
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
        {
            m_Components.RemoveAt(i);
            break;
        }
    }
}

template <class ComponentT>
bool_t Entity::TryGetComponent(ComponentT** output)
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<ComponentT>())
        {
            *output = m_Components[i].Cast<ComponentT>();
            return true;
        }
    }
    
    return false;
}

END_XNOR_CORE
