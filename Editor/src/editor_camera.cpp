#include "editor_camera.hpp"

#include "input/input.hpp"
#include "input/time.hpp"

using namespace XnorEditor;

void EditorCamera::UpdateCamera(XnorCore::Camera* camera)
{
    if(!camera)
        return;
    
    if (XnorCore::Input::GetMouseButton(XnorCore::MouseButton::Right, XnorCore::MouseButtonStatus::Down) && m_IsMoving == false)
        m_IsMoving = true;

    if (XnorCore::Input::GetMouseButton(XnorCore::MouseButton::Right, XnorCore::MouseButtonStatus::Release))
        ResetCameraRotation(camera);

    if (m_IsMoving)
        EditorCameraRotation(camera);
        
    EditorCameraMovement(camera);
}
    

void EditorCamera::ResetCameraRotation(XnorCore::Camera* camera)
{
    m_FirstMove = false;
    m_IsMoving = false;
    m_LastX = 0;
    m_LastY = 0;
}

void EditorCamera::EditorCameraRotation(XnorCore::Camera* camera)
{
    using namespace XnorCore;
    
    const Vector2 mousePos = Input::GetCursorPos<Vector2>();
    
    if(!m_FirstMove)
    {
        m_LastX = mousePos.x;
        m_LastY = mousePos.y;
        m_FirstMove = true;
    } 
    
    const float xoffset = mousePos.x - m_LastX ;
    const float yoffset = m_LastY - mousePos.y ; // reversed since y-coordinates range from bottom to top
    m_LastX = mousePos.x;
    m_LastY = mousePos.y;
    
    m_Yaw   += xoffset;
    m_Pitch += yoffset; 
    
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


