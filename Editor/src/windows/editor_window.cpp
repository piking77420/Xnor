#include "windows/editor_window.hpp"


using namespace XnorEditor; 

EditorWindow::EditorWindow(Editor* editor)
    : RenderWindow(editor, "Editor") , m_EditorCamera(*m_Editor, m_Editor->data.editorCam)
{
    m_RendererContext.isEditor = true;
    m_RendererContext.camera = &editor->data.editorCam;
}

void EditorWindow::Display()
{
    RenderWindow::Display();
    DrawOnTopOfImage();
    if (IsFocused())
    {
        m_EditorCamera.UpdateCamera();
    }

    m_EditorCamera.OnPressGoToObject();
    EditTransform();
}

void EditorWindow::Initialize(const Vector2i size)
{
    RenderWindow::Initialize(size);
    m_RendererContext.camera = &m_Editor->data.editorCam;
}

void EditorWindow::DrawOnTopOfImage()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.f);
    ImGui::SetCursorPos({25,25});
    if (ImGui::Button("Translation",{25,25}))
    {
        m_TransfromGizmo.currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    }
    ImGui::PopStyleVar();

    ImGui::SetCursorPos({50,25});
    if (ImGui::Button("Rotation",{25,25}))
    {
        m_TransfromGizmo.currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
    }
    
    ImGui::SetCursorPos({75,25});
    if (ImGui::Button("Scale",{25,25}))
    {
        m_TransfromGizmo.currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.f);

    ImGui::SetCursorPos({ImGui::GetWindowWidth() - 50,25});
    if (ImGui::Button("Snap",{25,25}))
    {
        m_TransfromGizmo.useSnap = !m_TransfromGizmo.useSnap;
    }
    ImGui::PopStyleVar();
}

void EditorWindow::EditTransform()
{
    if (!m_Editor->data.selectedEntity)
        return;

    const XnorCore::Camera& cam = m_Editor->data.editorCam;
    m_TransfromGizmo.SetRendering(cam,{ static_cast<float_t>(m_Position.x), static_cast<float_t>(m_Position.y) }, m_Size);
    m_TransfromGizmo.Manipulate(*m_Editor->data.selectedEntity);
}
