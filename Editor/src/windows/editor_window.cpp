#include "windows/editor_window.hpp"

#include <assimp/Logger.hpp>
#include <ImguiGizmo/ImGuizmo.h>
#include "input/time.hpp"

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
    if (IsFocused())
    {
        m_EditorCamera.UpdateCamera();
    }
    m_EditorCamera.OnPressGoToObject();
    EditTransform();

}

void EditorWindow::Initialize(Vector2i size)
{
    RenderWindow::Initialize(size);
    m_RendererContext.camera = &m_Editor->data.editorCam;
}

void EditorWindow::EditTransform()
{
    if (!m_Editor->data.selectedEntity)
        return;

    
    XnorCore::Transform& transform = m_Editor->data.selectedEntity->transform; 
    XnorCore::Camera& cam = m_Editor->data.editorCam;


     static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    if (ImGui::IsKeyPressed(ImGuiKey_T))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_Y)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    
    if(ImGuizmo::IsUsing())
    {
        window_flags = ImGuiWindowFlags_NoMove;
        XnorCore::Logger::LogDebug("No Move");
    }
    else
    {
        window_flags = ImGuiWindowFlags_None;
    }

    
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    
    
    const Vector2 sizef = {static_cast<float_t>(m_Size.x), static_cast<float_t>(m_Size.y)};
    const Vector2 Posf = {static_cast<float_t>(m_Position.x), static_cast<float_t>(m_Position.y)};

    Matrix proj;
    cam.GetProjection(m_Size,&proj);

    Matrix view;
    cam.GetView(&view);
    
    ImGuizmo::SetRect(Posf.x, Posf.y, sizef.x, sizef.y);
    if(ImGuizmo::Manipulate(view.Raw(), proj.Raw(), mCurrentGizmoOperation, mCurrentGizmoMode, transform.worldMatrix.Raw(), NULL, NULL))
    {
        
    }
}
