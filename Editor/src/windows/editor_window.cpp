#include "windows/editor_window.hpp"

#include "input/time.hpp"

using namespace XnorEditor; 

EditorWindow::EditorWindow(Editor* editor)
    : RenderWindow(editor, "Editor")
{
    m_RendererContext.isEditor = true;
    m_RendererContext.camera = &editor->data.editorCam;
    m_EditorCamRef = &editor->data.editorCam;
}

void EditorWindow::Display()
{
    RenderWindow::Display();
    CameraControl();
    
    if (!m_Editor->data.selectedEntity)
        return;

    
    
}

void EditorWindow::CameraControl()
{
    if (IsFocused())
    {
        if (XnorCore::CoreInput::GetMouseButton(XnorCore::MouseButton::Right, XnorCore::MouseButtonStatus::Down) && m_IsMoving == false)
            m_IsMoving = true;

        if (XnorCore::CoreInput::GetMouseButton(XnorCore::MouseButton::Right, XnorCore::MouseButtonStatus::Release))
            ResetCameraRotation();

        if (m_IsMoving)
            EditorCameraRotation();
        
        EditorCameraMovement();
    }
    else
    {
        ResetCameraRotation();
    }
}

void EditorWindow::OnGotoObject()
{
    if (!XnorCore::CoreInput::GetKey(m_Editor->data.goToObjectKey, XnorCore::KeyStatus::Down))
        return;

    
}

void EditorWindow::EditorCameraUpdate()
{
    EditorCameraRotation();
    EditorCameraMovement();
}

void EditorWindow::EditorCameraRotation()
{
    using namespace XnorCore;
    
    const Vector2 mousePos = CoreInput::GetCursorPos<Vector2>();
    
    if(!m_FirstMove)
    {
        lastX = mousePos.x;
        lastY = mousePos.y;
        m_FirstMove = true;
    } 
    
    const float xoffset = mousePos.x - lastX ;
    const float yoffset = lastY - mousePos.y ; // reversed since y-coordinates range from bottom to top
    lastX = mousePos.x;
    lastY = mousePos.y;
    
    m_Yaw   += xoffset;
    m_Pitch += yoffset; 
    
    if(m_Pitch > MaxPitch)
        m_Pitch = MaxPitch;
    if(m_Pitch < -MaxPitch)
        m_Pitch = -MaxPitch;

    m_EditorCamRef->front.x = std::cos(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    m_EditorCamRef->front.y = std::sin(m_Pitch * Calc::Deg2Rad);
    m_EditorCamRef->front.z = std::sin(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    m_EditorCamRef->front = m_EditorCamRef->front.Normalized();

    m_EditorCamRef->right = Vector3::Cross(Vector3::UnitY(),m_EditorCamRef->front).Normalized();
    m_EditorCamRef->up = Vector3::Cross(m_EditorCamRef->front,m_EditorCamRef->right).Normalized();
}

void EditorWindow::EditorCameraMovement()
{
    using namespace XnorCore;
    const float_t dt = Time::GetDeltaTime();
    const float_t cameraSpeed = m_Editor->data.cameraSpeed * dt;

    if (CoreInput::GetKey(Key::W, KeyStatus::Down))
        m_EditorCamRef->pos += m_EditorCamRef->front * cameraSpeed;

    if (CoreInput::GetKey(Key::S, KeyStatus::Down))
        m_EditorCamRef->pos -= m_EditorCamRef->front * cameraSpeed;

    if (CoreInput::GetKey(Key::A, KeyStatus::Down))
        m_EditorCamRef->pos += m_EditorCamRef->right * cameraSpeed;
    
    if (CoreInput::GetKey(Key::D, KeyStatus::Down))
        m_EditorCamRef->pos -=  m_EditorCamRef->right * cameraSpeed;
   
    if (CoreInput::GetKey(Key::Space, KeyStatus::Down))
        m_EditorCamRef->pos += Vector3::UnitY() * cameraSpeed;

    if (CoreInput::GetKey(Key::LeftControl, KeyStatus::Down))
        m_EditorCamRef->pos -= Vector3::UnitY() * cameraSpeed;
}

void EditorWindow::ResetCameraRotation()
{
    m_FirstMove = false;
    m_IsMoving = false;
    lastX = 0;
    lastY = 0;
}

