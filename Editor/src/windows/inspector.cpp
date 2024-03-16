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
    
    void* const ptr = m_Editor->data.selectedEntity;
    
    ImGui::PushID(ptr);

    using ObjType = std::remove_reference_t<decltype(*m_Editor->data.selectedEntity)>;

    DisplayObject(static_cast<ObjType*>(ptr));
    
    ImGui::PopID();
}
