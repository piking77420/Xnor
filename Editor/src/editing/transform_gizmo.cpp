#include "editing/transform_gizmo.hpp"

using namespace XnorEditor;


void TransfromGizmo::SetRendering(const XnorCore::Camera& camera, Vector2 windowPos, Vector2 windowSize)
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    camera.GetProjection({ static_cast<int32_t>(windowSize.x) , static_cast<int32_t>(windowSize.y) }, &m_Projection);
    camera.GetView(&m_View);
    ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x,windowSize.y);
}

void TransfromGizmo::Manipulate(XnorCore::Transform& transform)
{
    if (ImGui::IsKeyPressed(ImGuiKey_T))
        m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
        m_CurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_Y)) 
        m_CurrentGizmoOperation = ImGuizmo::SCALE;
    
    ImGuizmo::Manipulate(m_View.Raw(), m_Projection.Raw(), m_CurrentGizmoOperation, m_CurrentGizmoMode, transform.worldMatrix.Raw());
    
    if (!ImGuizmo::IsOver() || !ImGuizmo::IsUsing())
        return;
    
    Vector3 position;
    Vector3 eulerRotation;
    Vector3 scale;
    ImGuizmo::DecomposeMatrixToComponents(transform.worldMatrix.Raw(), position.Raw(), eulerRotation.Raw(), scale.Raw());

    transform.SetPosition() = position;
    // Convert Imgui gizmoRot
    transform.SetRotationEulerAngle() = eulerRotation * Calc::Deg2Rad;
    transform.SetRotation() = Quaternion::FromEuler(eulerRotation);
    transform.SetScale() = scale;
        
}
