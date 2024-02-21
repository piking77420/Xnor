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
            ResetCameraRotation();
        }

        if(m_IsMoving)
            EditorCameraRotation();
        
        EditorCameraMovement();


    }
    else
    {
        ResetCameraRotation();
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
    
    const float xoffset = lastX - mousePos.x ;
    const float yoffset = mousePos.y - lastY ; // reversed since y-coordinates range from bottom to top
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
    constexpr float_t fakeDeltatime = 0.03f;
    float_t cameraSpeed = m_Editor->data.cameraSpeed * fakeDeltatime;

    if(CoreInput::GetKey(KeyCode::KEY_W,KeyStatus::REPEAT) || CoreInput::GetKey(KeyCode::KEY_W,KeyStatus::DOWN))
    {
        m_Camera.pos += m_Camera.front * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_S,KeyStatus::REPEAT) || CoreInput::GetKey(KeyCode::KEY_S,KeyStatus::DOWN))
    {
        m_Camera.pos -= m_Camera.front * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_D,KeyStatus::REPEAT) || CoreInput::GetKey(KeyCode::KEY_D,KeyStatus::DOWN))
    {
        m_Camera.pos +=  m_Camera.right * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_A,KeyStatus::REPEAT) || CoreInput::GetKey(KeyCode::KEY_A,KeyStatus::DOWN))
    {
        m_Camera.pos -= m_Camera.right * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_SPACE,KeyStatus::REPEAT) || CoreInput::GetKey(KeyCode::KEY_SPACE,KeyStatus::DOWN))
    {
        
        m_Camera.pos += m_Camera.up * cameraSpeed ;
    }

    if(CoreInput::GetKey(KeyCode::KEY_LEFT_CONTROL,KeyStatus::REPEAT) || CoreInput::GetKey(KeyCode::KEY_LEFT_CONTROL,KeyStatus::DOWN))
    {
        m_Camera.pos -= m_Camera.up * cameraSpeed ;
    }
}

void EditorWindow::ResetCameraRotation()
{
    m_FirstMove = false;
    m_IsMoving = false;
    lastX = 0;
    lastY = 0;
}

