#include "windows/editor_window.hpp"

#include <ImguiGizmo/ImGuizmo.h>

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
    const XnorCore::Camera& cam = m_Editor->data.editorCam;
    
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    if (ImGui::IsKeyPressed(ImGuiKey_T))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_Y)) 
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    
    if (ImGuizmo::IsUsing())
    {
        window_flags = ImGuiWindowFlags_NoMove;
    }
    else
    {
        window_flags = ImGuiWindowFlags_None;
    }
    
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    
    const Vector2 sizef = { static_cast<float_t>(m_Size.x), static_cast<float_t>(m_Size.y) };
    const Vector2 posf = { static_cast<float_t>(m_Position.x), static_cast<float_t>(m_Position.y) };
    const Vector2i apsectRatioImguiWindow = { static_cast<int32_t>(sizef.x) , static_cast<int32_t>(sizef.y) };
    const Vector2i apsectRatioWindow = { XnorCore::Window::GetSize().x ,XnorCore::Window::GetSize().y };
    
    Matrix proj;
    cam.GetProjection(apsectRatioImguiWindow,&proj);
    Matrix view;
    cam.GetView(&view);
    
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(),ImGui::GetWindowHeight());
    if (ImGuizmo::Manipulate(view.Raw(), proj.Raw(), mCurrentGizmoOperation, mCurrentGizmoMode, transform.worldMatrix.Raw()))
    {
        Vector3 position;
        Vector3 eulerRotation;
        Vector3 scale;
        transform.changed = true;
        
        ImGuizmo::DecomposeMatrixToComponents(transform.worldMatrix.Raw(), position.Raw(), eulerRotation.Raw(), scale.Raw());
        transform.position = position;
        // Convert Imgui gizmoRot
        transform.eulerRotation = eulerRotation * Calc::Deg2Rad;
        transform.rotation = Quaternion::FromEuler(eulerRotation);
        transform.scale = scale;
    }
}
