#include "reflection/filters.hpp"

#include "reflection/xnor_factory.hpp"
#include "world/world.hpp"

using namespace XnorCore;

Entity* Filters::FilterEntity(Entity** target)
{
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
        ImGui::CloseCurrentPopup();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        ImGui::CloseCurrentPopup();
    
    ImGui::EndPopup();
    return e;
}

Component* Filters::FilterComponent(List<Component*>* target)
{
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
        ImGui::CloseCurrentPopup();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        ImGui::CloseCurrentPopup();
    
    ImGui::EndPopup();
    return c;
}

Component* Filters::FilterComponent(Component** target)
{
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
    }

    ImGui::EndPopup();
    return c;
}

void Filters::BeginResourceFilter()
{
    m_TextFilter.Clear();
    ImGui::OpenPopup("Resource");
}

void Filters::BeginEntityFilter()
{
    m_TextFilter.Clear();
    ImGui::OpenPopup("Entity");
}

void Filters::BeginComponentFilter()
{
    m_TextFilter.Clear();
    ImGui::OpenPopup("Component");
}
