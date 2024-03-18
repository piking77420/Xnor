﻿#include "windows/inspector.hpp"

#include "imgui/imgui.h"
#include "reflection/type_renderer.hpp"
#include "utils/factory.hpp"

using namespace XnorEditor;

Inspector::Inspector(Editor* editor)
    : UiWindow(editor, "Inspector")
{
}

void Inspector::Display()
{
    XnorCore::Entity* const ptr = m_Editor->data.selectedEntity;
    
    // Check if an entity was selected
    if (!ptr)
    {
        ImGui::Text("Nothing selected");
        return;
    }
    
    ImGui::PushID(ptr);

    XnorCore::TypeRenderer::DisplayObject<XnorCore::Entity>(ptr);
    
    ImGui::PopID();
}
