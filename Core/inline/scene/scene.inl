#pragma once

BEGIN_XNOR_CORE

template <class ComponentT>
void Scene::GetAllComponentOfType(std::vector<const ComponentT*>* const components) const
{
    components->clear();
    
    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        const ComponentT* component = m_Entities[i]->GetComponent<ComponentT>();

        if (component != nullptr)
            components->push_back(component);
    }
}

template <class ComponentT>
void Scene::GetAllComponentOfType(std::vector<ComponentT*>* const components)
{
    components->clear();

    for (size_t i = 0; i < m_Entities.GetSize(); i++)
    {
        ComponentT* component = m_Entities[i]->GetComponent<ComponentT>();

        if (component != nullptr)
            components->push_back(component);
    }
}

END_XNOR_CORE
