#include "windows/inspector.hpp"

#include "imgui/imgui.h"

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
    
    // Get the base address of the selected entity, we'll use this pointer to do arithmetic on it to get the address of the different fields
    void* const ptr = m_Editor->data.selectedEntity;
    
    ImGui::PushID(ptr);

    using ObjType = std::remove_reference_t<decltype(*m_Editor->data.selectedEntity)>;
    constexpr XnorCore::TypeDescriptor<ObjType> desc = XnorCore::TypeInfo::Get<ObjType>();

    DisplayObject(static_cast<ObjType*>(ptr), desc);
    
    ImGui::PopID();
}
