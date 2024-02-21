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
        EditorCameraUpdate();
    }
}

void EditorWindow::EditorCameraUpdate()
{
    float cameraSpeed = m_Editor->data.cameraSpeed;

    
}

