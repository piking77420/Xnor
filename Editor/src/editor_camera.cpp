#include "editor_camera.hpp"

#include "editor.hpp"
#include "input/time.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorEditor;

void EditorCamera::UpdateCamera(const Editor& editor, XnorCore::Camera& camera)
{
    m_EditorRef = &editor;
    m_EditorRefCamera = &camera;
  
    if (m_ComputeDeltaMouse)
    {
        ComputeDeltaMouse();
        m_ComputeDeltaMouse = false;
    }
    
    OnMiddleButton();
    CameraOnRightClick();
    EditorCameraMovement();
    OnPressGoToObject();

    if (m_ResetDeltaMouse)
    {
        ResetDeltatMouse();
        m_ResetDeltaMouse = false;
    }
}

void EditorCamera::ResetDeltatMouse()
{
    m_FirstMove = false;
    m_LastInput = Vector2(0.f);
}

void EditorCamera::CameraOnRightClick()
{
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        m_ComputeDeltaMouse = true;
        EditorCameraRotation();
        XnorCore::Window::HideCursor(true);
    }
    
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        m_ResetDeltaMouse = true;
        XnorCore::Window::HideCursor(false);
    }
}

void EditorCamera::EditorCameraRotation()
{
    m_Yaw += m_MouseOffSet.x;
    m_Pitch += m_MouseOffSet.y; 
    
    if (m_Pitch > MaxPitch)
        m_Pitch = MaxPitch;
    if (m_Pitch < -MaxPitch)
        m_Pitch = -MaxPitch;

    m_EditorRefCamera->front.x = std::cos(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    m_EditorRefCamera->front.y = std::sin(m_Pitch * Calc::Deg2Rad);
    m_EditorRefCamera->front.z = std::sin(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    m_EditorRefCamera->front = m_EditorRefCamera->front.Normalized();

    m_EditorRefCamera->right = Vector3::Cross(Vector3::UnitY(),m_EditorRefCamera->front).Normalized();
    m_EditorRefCamera->up = Vector3::Cross(m_EditorRefCamera->front,m_EditorRefCamera->right).Normalized();
}

void EditorCamera::EditorCameraMovement()
{
    const float_t dt = XnorCore::Time::GetDeltaTime();
    const float_t cameraSpeed = m_CameraSpeed * dt;

    if (ImGui::IsKeyDown(ImGuiKey_W))
        m_EditorRefCamera->pos += m_EditorRefCamera->front * cameraSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_S))
        m_EditorRefCamera->pos -= m_EditorRefCamera->front * cameraSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_A))
        m_EditorRefCamera->pos += m_EditorRefCamera->right * cameraSpeed;
    
    if (ImGui::IsKeyDown(ImGuiKey_D))
        m_EditorRefCamera->pos -=  m_EditorRefCamera->right * cameraSpeed;
   
    if (ImGui::IsKeyDown(ImGuiKey_Space))
        m_EditorRefCamera->pos += Vector3::UnitY() * cameraSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        m_EditorRefCamera->pos -= Vector3::UnitY() * cameraSpeed;
}

void EditorCamera::OnMiddleButton()
{
    using namespace XnorCore;
    
    if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
    {
        m_ComputeDeltaMouse = true;
        const Vector3 vector = (m_EditorRefCamera->right * -m_MouseOffSet.x) + (m_EditorRefCamera->up * m_MouseOffSet.y);
        m_EditorRefCamera->pos += vector * Time::GetDeltaTime() * m_CameraSpeed;
    }
    
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
    {
        m_ResetDeltaMouse = true;
    }
}

void EditorCamera::ComputeDeltaMouse()
{
    const Vector2 mousePos = XnorCore::Utils::FromImVec(ImGui::GetMousePos());
    
    if (!m_FirstMove)
    {
        m_LastInput.x = mousePos.x;
        m_LastInput.y = mousePos.y;
        m_FirstMove = true;
    } 
    
    m_MouseOffSet.x = mousePos.x - m_LastInput.x ;
    m_MouseOffSet.y = m_LastInput.y - mousePos.y ; // reversed since y-coordinates range from bottom to top
    m_LastInput.x = mousePos.x;
    m_LastInput.y = mousePos.y;
}

void EditorCamera::OnPressGoToObject()
{
    GoToObject();
    
    if (m_EditorRef->data.selectedEntity == nullptr)
        return;
    
    if (!XnorCore::Input::GetKey(XnorCore::Key::F, XnorCore::KeyStatus::Press))
    {
        return;
    }
    
    m_GotoObject = true;

    const XnorCore::Entity& currentEntiy = *m_EditorRef->data.selectedEntity;

    const XnorCore::MeshRenderer* meshRenderer = currentEntiy.GetComponent<XnorCore::MeshRenderer>();
    
    m_ObjectPos = currentEntiy.transform.position;
    
    if (meshRenderer == nullptr)
    {
        m_DistanceToStop = 1.f;
    }
    else
    {
        const XnorCore::ModelAABB&& aabb = meshRenderer->model->GetAABB();
        const Vector3 radiusVec = aabb.max - aabb.min;
        Vector4 radiusPreScale = Vector4(radiusVec.x,radiusVec.y,radiusVec.z,1.0f);
        radiusPreScale = Matrix::Trs(Vector3(0.f),Quaternion::Identity(),currentEntiy.transform.scale) * radiusPreScale;
        const Vector3 correctVec = {radiusPreScale.x,radiusPreScale.y,radiusPreScale.z};
        
        m_DistanceToStop = correctVec.Length();
    }

    m_GotoObject = true;
}

void EditorCamera::GoToObject()
{
    if (!m_GotoObject)
        return;
    
    Vector3 forwartVec = (m_ObjectPos - m_EditorRefCamera->pos);
    const float distance = forwartVec.Length();
        
    if (distance <= m_DistanceToStop)
    {
        m_GotoObject = false;
        return;
    }
    
    forwartVec *= 1.f / distance;
    m_EditorRefCamera->pos = Vector3::Lerp(m_EditorRefCamera->pos,m_EditorRefCamera->pos + forwartVec, XnorCore::Time::GetDeltaTime() * distance);
}
