#include "editing/transform_gizmo.hpp"

using namespace XnorEditor;


void TransfromGizmo::SetRendering(const XnorCore::Camera& camera, const Vector2 windowPos, const Vector2i windowSize)
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    camera.GetProjection(windowSize, &m_Projection);
    camera.GetView(&m_View);
    ImGuizmo::SetRect(windowPos.x, windowPos.y, static_cast<float_t>(windowSize.x), static_cast<float_t>(windowSize.y));
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
        
        ImGuizmo::DecomposeMatrixToComponents(transform.worldMatrix.Raw(), position.Raw(), eulerRotation.Raw(), scale.Raw());
        transform.SetPosition() = position;
        // Convert Imgui gizmoRot
        transform.SetRotationEulerAngle() = eulerRotation * Calc::Deg2Rad;
        transform.SetRotation() = Quaternion::FromEuler(eulerRotation);
        transform.SetScale() = scale;

        return true;
    }

    return false;
}
