#include "editor_camera.hpp"

#include <assimp/Logger.hpp>
#include <ImGui/imgui_internal.h>

#include "editor.hpp"
#include "input/time.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorEditor;

EditorCamera::EditorCamera(Editor& editor, XnorCore::Camera& camera)
    : m_EditorRef(&editor)
    , m_EditorRefCamera(&camera) 
{
}

void EditorCamera::UpdateCamera()
{
    OnMiddleButton();
    CameraOnRightClick();
    EditorCameraMovement();
}

void EditorCamera::CameraOnRightClick()
{
    if (!ImGui::IsWindowHovered())
        return;
    
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        XnorCore::Window::SetCursorHidden(true);
        m_MouseDragStart = XnorCore::Utils::FromImVec(ImGui::GetMousePos());
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        EditorCameraRotation();
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        XnorCore::Window::SetCursorHidden(false);
    }
}

void EditorCamera::EditorCameraRotation()
{
    const ImGuiIO& io = ImGui::GetIO();
    
    m_Yaw -= io.MouseDelta.x;
    m_Pitch += io.MouseDelta.y; 
    
    if (m_Pitch > MaxPitch)
        m_Pitch = MaxPitch;
    if (m_Pitch < -MaxPitch)
        m_Pitch = -MaxPitch;

    m_EditorRefCamera->front.x = std::cos(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    m_EditorRefCamera->front.y = std::sin(m_Pitch * Calc::Deg2Rad);
    m_EditorRefCamera->front.z = std::sin(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    m_EditorRefCamera->front = m_EditorRefCamera->front.Normalized();
    
    m_EditorRefCamera->right = Vector3::Cross(m_EditorRefCamera->front, Vector3::UnitY()).Normalized();
    m_EditorRefCamera->up = Vector3::Cross(m_EditorRefCamera->right, m_EditorRefCamera->front).Normalized();
}

void EditorCamera::EditorCameraMovement()
{
    const float_t dt = XnorCore::Time::GetDeltaTime();
    const float_t cameraSpeed = m_CameraSpeed * dt;

    Vector3 addVector;

    if (ImGui::IsKeyDown(ImGuiKey_W))
        addVector -= m_EditorRefCamera->front * cameraSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_S))
        addVector += m_EditorRefCamera->front * cameraSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_A))
        addVector -= m_EditorRefCamera->right * cameraSpeed;
    
    if (ImGui::IsKeyDown(ImGuiKey_D))
        addVector +=  m_EditorRefCamera->right * cameraSpeed;
   
    if (ImGui::IsKeyDown(ImGuiKey_Space))
        addVector += Vector3::UnitY() * cameraSpeed;

    if (ImGui::IsKeyDown(ImGuiMod_Shift))
        addVector -= Vector3::UnitY() * cameraSpeed;

    AddMovement(addVector);
}

void EditorCamera::OnMiddleButton()
{
    if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
    {
        const ImGuiIO& io = ImGui::GetIO();
        
        const Vector3 vector = m_EditorRefCamera->right * -io.MouseDelta.x + m_EditorRefCamera->up * -io.MouseDelta.y;
        m_EditorRefCamera->position += vector * XnorCore::Time::GetDeltaTime() * m_CameraSpeed;
    }
}

void EditorCamera::OnPressGoToObject()
{
    if (m_EditorRef->data.selectedEntity == nullptr)
        return;

    if (ImGui::IsKeyPressed(ImGuiKey_F))
    {
        m_EditorRef->data.gotoObject = true;
    }
    
    GoToObject();

    if (!m_EditorRef->data.gotoObject)
        return;
    
    const XnorCore::Entity& currentEntiy = *m_EditorRef->data.selectedEntity;
    const XnorCore::MeshRenderer* meshRenderer = currentEntiy.GetComponent<XnorCore::MeshRenderer>();
    m_ObjectPos = currentEntiy.transform.GetPosition();
    m_EditorRefCamera->LookAt(m_ObjectPos);
    
    if (meshRenderer == nullptr)
    {
        m_DistanceToStop = 1.f;
    }
    else
    {
        const XnorCore::Model::Aabb&& aabb = meshRenderer->model->GetAabb();
        const Vector3 radiusVec = aabb.max - aabb.min;
        Vector4 radiusPreScale = Vector4(radiusVec.x, radiusVec.y, radiusVec.z, 1.0f);
        radiusPreScale = Matrix::Trs(Vector3(0.f), Quaternion::Identity(), currentEntiy.transform.GetScale()) * radiusPreScale;
        const Vector3 correctVec = {radiusPreScale.x, radiusPreScale.y, radiusPreScale.z};
        m_DistanceToStop = correctVec.Length();
    }
    
    m_EditorRef->data.gotoObject = true;
}

void EditorCamera::GoToObject()
{
    if (!m_EditorRef->data.gotoObject)
        return;
    
    Vector3 forwardVec = (m_ObjectPos - m_EditorRefCamera->position);
    const float_t distance = forwardVec.Length();
        
    if (distance <= m_DistanceToStop)
    {
        m_EditorRef->data.gotoObject = false;
        return;
    }
    
    forwardVec *= 1.f / distance;
    m_EditorRefCamera->position = Vector3::Lerp(m_EditorRefCamera->position, m_EditorRefCamera->position + forwardVec, XnorCore::Time::GetDeltaTime() * distance);
}

void EditorCamera::AddMovement(const Vector3& movement)
{
    if (movement == Vector3::Zero())
        return;
    
    m_EditorRefCamera->position += movement;
    m_EditorRef->data.gotoObject = false;
}
