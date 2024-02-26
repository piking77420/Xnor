#include "editor_camera.hpp"

#include "input/input.hpp"
#include "input/time.hpp"

using namespace XnorEditor;

void EditorCamera::UpdateCamera(XnorCore::Camera* camera)
{
    if (!camera)
        return;

    CameraOnRightClick(camera);
    OnMiddleButton(camera);
    
    if(m_ComputeDeltaMouse)
        ComputeDeltaMouse();

    if(m_ResetDeltaMouse)
        ResetDeltatMouse();
    
        
    EditorCameraMovement(camera);
    OnMiddleButton(camera);
}
    

void EditorCamera::ResetDeltatMouse()
{
    m_FirstMove = false;
    m_LastInput = Vector2(0.f);
}

void EditorCamera::CameraOnRightClick(XnorCore::Camera* camera)
{
    if (XnorCore::Input::GetMouseButton(XnorCore::MouseButton::Right, XnorCore::MouseButtonStatus::Release))
    {
        m_ResetDeltaMouse = true;
    }
    else
    {
        m_ResetDeltaMouse = false;
    }
    
    if (XnorCore::Input::GetMouseButton(XnorCore::MouseButton::Right, XnorCore::MouseButtonStatus::Down))
    {
        m_ComputeDeltaMouse = true;
        EditorCameraRotation(camera);
    }
    else
    {
        m_ComputeDeltaMouse = false;
    }
}

void EditorCamera::EditorCameraRotation(XnorCore::Camera* camera)
{
    using namespace XnorCore;

    ComputeDeltaMouse();
    
    m_Yaw   += m_MouseOffSet.x;
    m_Pitch += m_MouseOffSet.y; 
    
    if(m_Pitch > MaxPitch)
        m_Pitch = MaxPitch;
    if(m_Pitch < -MaxPitch)
        m_Pitch = -MaxPitch;

    camera->front.x = std::cos(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    camera->front.y = std::sin(m_Pitch * Calc::Deg2Rad);
    camera->front.z = std::sin(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    camera->front = camera->front.Normalized();

    camera->right = Vector3::Cross(Vector3::UnitY(),camera->front).Normalized();
    camera->up = Vector3::Cross(camera->front,camera->right).Normalized();
}

void EditorCamera::EditorCameraMovement(XnorCore::Camera* camera)
{
    using namespace XnorCore;
    const float_t dt = Time::GetDeltaTime();
    const float_t cameraSpeed = m_CameraSpeed * dt;

    if (Input::GetKey(Key::W, KeyStatus::Down))
        camera->pos += camera->front * cameraSpeed;

    if (Input::GetKey(Key::S, KeyStatus::Down))
        camera->pos -= camera->front * cameraSpeed;

    if (Input::GetKey(Key::A, KeyStatus::Down))
        camera->pos += camera->right * cameraSpeed;
    
    if (Input::GetKey(Key::D, KeyStatus::Down))
        camera->pos -=  camera->right * cameraSpeed;
   
    if (Input::GetKey(Key::Space, KeyStatus::Down))
        camera->pos += Vector3::UnitY() * cameraSpeed;

    if (Input::GetKey(Key::LeftControl, KeyStatus::Down))
        camera->pos -= Vector3::UnitY() * cameraSpeed;
}

void EditorCamera::OnMiddleButton(XnorCore::Camera* camera)
{
    using namespace XnorCore;
    
    if (Input::GetMouseButton(MouseButton::Middle,MouseButtonStatus::Down))
    {
        m_ComputeDeltaMouse = true;
        const Vector3 vector = (camera->right * -m_MouseOffSet.x) + (camera->up * m_MouseOffSet.y);
        camera->pos += vector * Time::GetDeltaTime() * m_CameraSpeed ; 
    }
    else
    {
        m_ComputeDeltaMouse = false;
    }
    
    if (Input::GetMouseButton(MouseButton::Middle,MouseButtonStatus::Release))
    {
        m_ResetDeltaMouse = true;
    }
    else
    {
        m_ResetDeltaMouse = false;
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


