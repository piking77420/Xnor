#include "editing/transform_gizmo.hpp"

using namespace XnorEditor;


void TransfromGizmo::SetRendering(const XnorCore::Camera& camera, Vector2 windowPos, Vector2 windowSize)
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    
    camera.GetProjection({ static_cast<int32_t>(windowPos.x) , static_cast<int32_t>(windowPos.y) }, &m_Projection);
    camera.GetView(&m_View);
    ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x,windowSize.y);
}

bool TransfromGizmo::Manipulate(XnorCore::Transform& transform)
{
    if (ImGui::IsKeyPressed(ImGuiKey_T))
        m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey_R))
        m_CurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey_Y)) 
        m_CurrentGizmoOperation = ImGuizmo::SCALE;


    
    if (ImGuizmo::Manipulate(m_View.Raw(), m_Projection.Raw(), m_CurrentGizmoOperation, m_CurrentGizmoMode, transform.worldMatrix.Raw()))
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
        return true;
    }

    return false;
}
