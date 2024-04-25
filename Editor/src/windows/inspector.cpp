#include "windows/inspector.hpp"

#include "imgui/imgui.h"
#include "reflection/type_renderer.hpp"

using namespace XnorEditor;

Inspector::Inspector(Editor* const editor)
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

    const std::pair<void*, const char_t*> windowInfo = XnorCore::TypeRenderer::DisplayObject<XnorCore::Entity>(ptr);

    if (windowInfo.first != nullptr)
    {
        m_Editor->OpenCreatedWindow(windowInfo.second, windowInfo.first);
        XnorCore::Logger::LogInfo("{} ; {}", windowInfo.first, windowInfo.second);
    }
    
    ImGui::PopID();
}
