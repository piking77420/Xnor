#include "windows/inspector.hpp"

#include "imgui/imgui.h"
#include "utils/factory.hpp"

using namespace XnorEditor;

Inspector::Inspector(Editor* editor)
    : UiWindow(editor, "Inspector")
{
}

void Inspector::Display()
{
    // Check if an entity was selected
    if (!m_Editor->data.selectedEntity)
    {
        ImGui::Text("Nothing selected");
        return;
    }
    
    void* const ptr = m_Editor->data.selectedEntity;
    
    ImGui::PushID(ptr);

    using ObjType = std::remove_reference_t<decltype(*m_Editor->data.selectedEntity)>;

    DisplayObject(static_cast<ObjType*>(ptr));
    
    ImGui::PopID();
}

XnorCore::Component* Inspector::FilterComponent(ImGuiTextFilter& filter)
{
    ImGui::OpenPopup("Component");

    if (!ImGui::BeginPopupModal("Component"))
        return nullptr;

    filter.Draw();

    XnorCore::Component* c = nullptr;
    
    std::vector<const char_t*> names;
    XnorCore::Factory::FindAllChildClasses<XnorCore::Component>(&names);

    for (size_t i = 0; i < names.size(); i++)
    {
        if (filter.PassFilter(names[i]) && ImGui::Selectable(names[i]))
        {
            c = static_cast<XnorCore::Component*>(XnorCore::Factory::CreateObject(names[i]));
            break;
        }
    }
    
    ImGui::EndPopup();
    return c;
}
