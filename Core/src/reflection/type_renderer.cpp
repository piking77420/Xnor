#include "reflection/type_renderer.hpp"
#include "reflection/factory.hpp"

using namespace XnorCore;

void TypeRenderer::DisplayObjectUsingFactory(void* const obj, const size_t hash)
{
    Factory::DisplayObject(obj, hash);
}

Component* TypeRenderer::FilterComponent(ImGuiTextFilter& filter)
{
    ImGui::OpenPopup("Component");

    if (!ImGui::BeginPopupModal("Component"))
        return nullptr;

    filter.Draw();

    Component* c = nullptr;
    
    std::vector<std::string> names;
    Factory::FindAllChildClasses<Component>(&names);

    for (size_t i = 0; i < names.size(); i++)
    {
        if (filter.PassFilter(names[i].c_str()) && ImGui::Selectable(names[i].c_str()))
        {
            c = static_cast<Component*>(Factory::CreateObject(names[i]));
            break;
        }
    }
    
    ImGui::EndPopup();
    return c;
}
