#include "reflection/filters.hpp"

#include "reflection/xnor_factory.hpp"
#include "world/world.hpp"

using namespace XnorCore;

Entity* Filters::FilterEntity(Entity** target)
{
    if (target == m_EntityFilterTarget)
        ImGui::OpenPopup("Entity");

    if (!ImGui::BeginPopupModal("Entity"))
        return nullptr;

    m_TextFilter.Draw();
    const List<Entity*>& entities = World::scene->GetEntities();

    Entity* e = nullptr;
    for (size_t i = 0; i < entities.GetSize(); i++)
    {
        const char_t* const name = entities[i]->name.c_str();
        if (m_TextFilter.PassFilter(name) && ImGui::Selectable(name))
        {
            e = entities[i];
            break;
        }
    }

    if (e != nullptr)
    {
        *target = e;
        m_EntityFilterTarget = nullptr;
    }
    
    ImGui::EndPopup();
    return e;
}

Component* Filters::FilterComponent(List<Component*>* target)
{
    if (target == m_ComponentFilterTarget)
    {
        ImGui::OpenPopup("Component");
    }

    if (!ImGui::BeginPopupModal("Component"))
        return nullptr;

    m_TextFilter.Draw();

    Component* c = nullptr;

    std::vector<std::string> names;
    XnorFactory::FindAllChildClasses<Component>(&names);

    for (size_t i = 0; i < names.size(); i++)
    {
        if (m_TextFilter.PassFilter(names[i].c_str()) && ImGui::Selectable(names[i].c_str()))
        {
            c = static_cast<Component*>(XnorFactory::CreateObject(names[i]));
            break;
        }
    }

    if (c != nullptr)
    {
        target->Add(c);
        m_ComponentFilterTarget = nullptr;
    }
    
    ImGui::EndPopup();
    return c;
}

Component* Filters::FilterComponent(Component** target)
{
    if (target == m_ComponentFilterTarget)
        ImGui::OpenPopup("Component");

    if (!ImGui::BeginPopupModal("Component"))
        return nullptr;

    m_TextFilter.Draw();

    Component* c = nullptr;

    std::vector<std::string> names;
    XnorFactory::FindAllChildClasses<Component>(&names);

    for (size_t i = 0; i < names.size(); i++)
    {
        if (m_TextFilter.PassFilter(names[i].c_str()) && ImGui::Selectable(names[i].c_str()))
        {
            c = static_cast<Component*>(XnorFactory::CreateObject(names[i]));
            break;
        }
    }

    if (c != nullptr)
    {
        *target = c;
        m_ComponentFilterTarget = nullptr;
    }

    ImGui::EndPopup();
    return c;
}

void Filters::BeginResourceFilter(const void* const target)
{
    m_ResourceFilterTarget = target;
    m_TextFilter.Clear();
}

void Filters::BeginEntityFilter(const void* const target)
{
    m_EntityFilterTarget = target;
    m_TextFilter.Clear();
}

void Filters::BeginComponentFilter(const void* const target)
{
    m_ComponentFilterTarget = target;
    m_TextFilter.Clear();
}
