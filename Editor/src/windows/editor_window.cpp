#include "windows/editor_window.hpp"

using namespace XnorEditor; 

EditorWindow::EditorWindow(Editor* editor,const std::string& name, XnorCore::RendererContext* rendererContext)
    : RenderWindow(editor,name,rendererContext)
{
    rendererContext->IsEditor = true;
}


void EditorWindow::Display()
{
    RenderWindow::Display();
    
    if (IsFocused())
    {
        if(XnorCore::CoreInput::GetMouseButton(XnorCore::MouseButton::Right,XnorCore::KeyStatus::DOWN) &&  m_IsMoving == false)
        {
            m_IsMoving = true;
        }

        if(XnorCore::CoreInput::GetMouseButton(XnorCore::MouseButton::Right,XnorCore::KeyStatus::RELEASE))
        {
            m_IsMoving = false;
        }

        if(m_IsMoving)
            EditorCameraRotation();
        
        EditorCameraMovement();


    }
    else
    {
        m_FirstMove = false;
        m_IsMoving = false;
    }
}

void EditorWindow::EditorCameraUpdate()
{
    EditorCameraRotation();
    EditorCameraMovement();
}

void EditorWindow::EditorCameraRotation()
{
    using namespace XnorCore;
    
    const Vector2 mousePos = XnorCore::CoreInput::GetCursorPos<Vector2>();
    
    if(!m_FirstMove)
    {
        lastX = mousePos.x;
        lastY = mousePos.y;
        m_FirstMove = true;
    } 
    
    const float xoffset = mousePos.x - lastX;
    const float yoffset = lastY - mousePos.y; // reversed since y-coordinates range from bottom to top
    lastX = mousePos.x;
    lastY = mousePos.y;
    
    m_Yaw   += xoffset;
    m_Pitch += yoffset; 
    
    if(m_Pitch > MaxPitch)
        m_Pitch = MaxPitch;
    if(m_Pitch < -MaxPitch)
        m_Pitch = -MaxPitch;


    
    m_Camera.front.x = std::cos(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    m_Camera.front.y = std::sin(m_Pitch * Calc::Deg2Rad);
    m_Camera.front.z = std::sin(m_Yaw * Calc::Deg2Rad) * std::cos(m_Pitch * Calc::Deg2Rad);
    m_Camera.front = m_Camera.front.Normalized();

    m_Camera.right = Vector3::Cross(m_Camera.front,Vector3::UnitY()).Normalized();
    m_Camera.up = Vector3::Cross(m_Camera.right,m_Camera.front).Normalized();
}

void EditorWindow::EditorCameraMovement()
{
    using namespace XnorCore;
    float_t cameraSpeed = m_Editor->data.cameraSpeed;

    if(CoreInput::GetKey(KeyCode::KEY_W))
    {
        m_Camera.pos += m_Camera.front * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_S))
    {
        m_Camera.pos -= m_Camera.front * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_D))
    {
        m_Camera.pos +=  m_Camera.right * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_A))
    {
        m_Camera.pos -= m_Camera.right * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_SPACE))
    {
        m_Camera.pos += m_Camera.up * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_LEFT_CONTROL))
    {
        m_Camera.pos -= m_Camera.up * cameraSpeed ;
    }
}

