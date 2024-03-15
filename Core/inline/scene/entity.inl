#pragma once

BEGIN_XNOR_CORE

template <Concepts::ComponentT T>
T* Entity::AddComponent()
{
    m_Components.Add();
    
    T* newT = new T;
    newT->entity = this;
    m_Components[m_Components.GetSize() - 1].Create(newT);

    return newT;
}

template <Concepts::ComponentT T>
const T* Entity::GetComponent() const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<T>())
            return m_Components[i].Cast<T>();
    }

    return nullptr;
}

template <Concepts::ComponentT T>
void Entity::GetComponents(std::vector<T*>* components)
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<T>())
            components->push_back(m_Components[i].Cast<T>());
    }
}

template <Concepts::ComponentT T>
void Entity::GetComponents(std::vector<const T*>* components) const
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<T>())
            components->push_back(reinterpret_cast<const T*>(m_Components[i]));
    }
}

template <Concepts::ComponentT T>
T* Entity::GetComponent()
{
    for (size_t i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<T>())
            return m_Components[i].Cast<T>();
    }
    
    return nullptr;
}

template <Concepts::ComponentT T>
void Entity::RemoveComponent()
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<T>())
        {
            m_Components.RemoveAt(i);
            break;
        }
    }
}

template <Concepts::ComponentT T>
bool_t Entity::TryGetComponent(T** output)
{
    for (int i = 0; i < m_Components.GetSize(); i++)
    {
        if (m_Components[i].IsOfType<T>())
        {
            *output = m_Components[i].Cast<T>();
            return true;
        }
    }
    
    return false;
}

END_XNOR_CORE
