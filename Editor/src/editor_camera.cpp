#include "editor_camera.hpp"

#include <ImGui/imgui_internal.h>
#include <Maths/easing.hpp>

#include "editor.hpp"
#include "input/time.hpp"
#include "scene/component/mesh_renderer.hpp"
#include "utils/logger.hpp"
#include "utils/utils.hpp"

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
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        XnorCore::Window::SetCursorHidden(true);
        m_MouseDrag = ImGui::GetMousePos();
        m_LowPassFilterDeltaMouse.Reset();
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        m_PreviousMouseDrag = m_MouseDrag;
        m_MouseDrag = ImGui::GetMousePos();
        ClampMouseToScreen(&m_MouseDrag, &m_PreviousMouseDrag);

        
        m_MouseDelta = m_MouseDrag - m_PreviousMouseDrag;
        m_LowPassFilterDeltaMouse.AddSample({ m_MouseDelta.x, m_MouseDelta.y } );

        EditorCameraRotation();
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        XnorCore::Window::SetCursorHidden(false);
        m_MouseDrag = Vector2::Zero();
    }
    
    
}

void EditorCamera::EditorCameraRotation()
{
    const Vector2 mousDelta = m_LowPassFilterDeltaMouse.GetAvarage<Vector2>();

    m_Yaw += mousDelta.x;
    m_Pitch -= mousDelta.y; 
    
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
    const float_t cameraSpeed = m_CameraSpeed;

    Vector3 addVector;

    if (ImGui::IsKeyDown(ImGuiKey_W))
        addVector += m_EditorRefCamera->front * cameraSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_S))
        addVector -= m_EditorRefCamera->front * cameraSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_A))
        addVector -= m_EditorRefCamera->right * cameraSpeed;
    
    if (ImGui::IsKeyDown(ImGuiKey_D))
        addVector += m_EditorRefCamera->right * cameraSpeed;
   
    if (ImGui::IsKeyDown(ImGuiKey_Space))
        addVector += Vector3::UnitY() * cameraSpeed;

    if (ImGui::IsKeyDown(ImGuiMod_Shift))
        addVector -= Vector3::UnitY() * cameraSpeed;

    AddMovement(addVector);
}

void EditorCamera::OnMiddleButton()
{
    const ImGuiIO& io = ImGui::GetIO();

    if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
    {
        const Vector3 vector = m_EditorRefCamera->right * -io.MouseDelta.x + m_EditorRefCamera->up * io.MouseDelta.y;
        AddMovement(vector * m_CameraSpeed);
    }

    if (io.MouseWheel == 0.f)
        return;
    
    const Vector3 vector = m_EditorRefCamera->front * io.MouseWheel;
    AddMovement(vector * m_MouseWheelZoom);
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
    
    if (meshRenderer == nullptr)
    {
        m_DistanceToStop = 1.f;
    }
    else
    {
        const XnorCore::Bound&& aabb = meshRenderer->model->GetAabb();
        Vector4 radiusPreScale = Vector4(aabb.extents.x, aabb.extents.y, aabb.extents.z, 1.0f) * 0.5f;
        radiusPreScale.w = 1.0f;
        
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
    
    const XnorCore::Entity& currentEntiy = *m_EditorRef->data.selectedEntity;
    const Vector3 pos = static_cast<Vector3>(currentEntiy.transform.worldMatrix[3]);
    m_EditorRefCamera->LookAt(static_cast<Vector3>(currentEntiy.transform.worldMatrix[3]));

    Vector3 forwardVec = (pos - m_EditorRefCamera->position);
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
    
    m_LowPassFilterMovment.AddSample((movement * m_CameraSpeed) * XnorCore::Time::GetDeltaTime<float_t>());
    
    m_EditorRefCamera->position = m_EditorRefCamera->position + m_LowPassFilterMovment.GetAvarage<Vector3>();
    m_EditorRef->data.gotoObject = false;
}

void EditorCamera::ClampMouseToScreen(Vector2* currentMousePos, Vector2* previousMousePos)
{
    if (currentMousePos == nullptr)
        return;

    // Get Info
    const Vector2 imguiWindowPos = ImGui::GetWindowPos();
    const Vector2 imguiHalfSize = ImGui::GetWindowSize() * 0.5f;
    const Vector2 imguiWindowMid  = imguiWindowPos + imguiHalfSize;
    
    // Mouse Relative to window
    Vector2 mouseToWindow = *currentMousePos - imguiWindowMid ;
    Vector2 previousMouseToWindow = *previousMousePos - imguiWindowMid;

    const Vector2 windowSpacing = ImGui::GetStyle().DisplayWindowPadding;
    bool_t changePos = false;

    if (std::abs(mouseToWindow.x) > imguiHalfSize.x - windowSpacing.x * 2.f)
    {
        // Swap Pos x
        mouseToWindow.x = -mouseToWindow.x;
        previousMouseToWindow.x = -previousMouseToWindow.x;
        changePos = true;
    }

    
    if (std::abs(mouseToWindow.y) > imguiHalfSize.y - windowSpacing.y * 2.f)
    {
        // Swap Pos y
        mouseToWindow.y = -mouseToWindow.y;
        previousMouseToWindow.y = -previousMouseToWindow.y;
        changePos = true;
    }
    
    if (changePos)
    {
        *currentMousePos = mouseToWindow * 0.5f + imguiWindowMid;
        *previousMousePos = previousMouseToWindow * 0.5f + imguiWindowMid;
        
        XnorCore::Window::SetCursorPosition(*currentMousePos);
    }

}


