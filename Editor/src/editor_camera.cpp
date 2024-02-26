#include "editor_camera.hpp"

#include "editor.hpp"
#include "input/input.hpp"
#include "input/time.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorEditor;
using namespace XnorCore;


void EditorCamera::UpdateCamera(const XnorEditor::Editor& editor,Camera& camera)
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
    if (Input::GetMouseButton(MouseButton::Right, MouseButtonStatus::Down))
    {
        m_ComputeDeltaMouse = true;
        EditorCameraRotation();
        Window::HideCursor(true);
    }
    
    if (Input::GetMouseButton(MouseButton::Right, MouseButtonStatus::Release))
    {
        m_ResetDeltaMouse = true;
        Window::HideCursor(false);
    }
    
}

void EditorCamera::EditorCameraRotation()
{
    m_Yaw   += m_MouseOffSet.x;
    m_Pitch += m_MouseOffSet.y; 
    
    if(m_Pitch > MaxPitch)
        m_Pitch = MaxPitch;
    if(m_Pitch < -MaxPitch)
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
    const float_t dt = Time::GetDeltaTime();
    const float_t cameraSpeed = m_CameraSpeed * dt;

    if (Input::GetKey(Key::W, KeyStatus::Down))
        m_EditorRefCamera->pos += m_EditorRefCamera->front * cameraSpeed;

    if (Input::GetKey(Key::S, KeyStatus::Down))
        m_EditorRefCamera->pos -= m_EditorRefCamera->front * cameraSpeed;

    if (Input::GetKey(Key::A, KeyStatus::Down))
        m_EditorRefCamera->pos += m_EditorRefCamera->right * cameraSpeed;
    
    if (Input::GetKey(Key::D, KeyStatus::Down))
        m_EditorRefCamera->pos -=  m_EditorRefCamera->right * cameraSpeed;
   
    if (Input::GetKey(Key::Space, KeyStatus::Down))
        m_EditorRefCamera->pos += Vector3::UnitY() * cameraSpeed;

    if (Input::GetKey(Key::LeftControl, KeyStatus::Down))
        m_EditorRefCamera->pos -= Vector3::UnitY() * cameraSpeed;
}

void EditorCamera::OnMiddleButton()
{
    using namespace XnorCore;
    
    if (Input::GetMouseButton(MouseButton::Middle,MouseButtonStatus::Down))
    {
        m_ComputeDeltaMouse = true;
        const Vector3 vector = (m_EditorRefCamera->right * -m_MouseOffSet.x) + (m_EditorRefCamera->up * m_MouseOffSet.y);
        m_EditorRefCamera->pos += vector * Time::GetDeltaTime() * m_CameraSpeed ; 
    }
    
    if (Input::GetMouseButton(MouseButton::Middle,MouseButtonStatus::Release))
    {
        m_ResetDeltaMouse = true;
    }
   

}

void EditorCamera::ComputeDeltaMouse()
{
    using namespace XnorCore;

    const Vector2 mousePos = Input::GetCursorPos<Vector2>();
    
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
    
    if (Input::GetKey(Key::F,KeyStatus::Press))
    {
        m_GotoObject = true;
    }
    else
    {
        return;
    }

    const Entity& currentEntiy = *m_EditorRef->data.selectedEntity;

    const MeshRenderer* meshRenderer = currentEntiy.GetComponent<MeshRenderer>();
    
    if (meshRenderer == nullptr)
    {
        m_ObjectPos = currentEntiy.transform.position;
        m_DistanceToStop = 1.f;
    }

    m_GotoObject = true;
}

void EditorCamera::GoToObject()
{
    if (!m_GotoObject)
        return;
    
    Vector3 forwartVec = (m_ObjectPos - m_EditorRefCamera->pos);
    const float distance = forwartVec.Length();
        
    if(distance <= m_DistanceToStop)
    {
        m_GotoObject = false;
        return;
    }
    
    Logger::LogDebug("Go to object");
        
    forwartVec *= 1.f/distance;
    m_EditorRefCamera->pos = Vector3::Lerp(m_EditorRefCamera->pos,m_EditorRefCamera->pos + forwartVec,Time::GetDeltaTime() * distance) ; 
}

