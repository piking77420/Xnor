#include "windows/editor_window.hpp"

#include "input/time.hpp"

using namespace XnorEditor; 

EditorWindow::EditorWindow(Editor* editor)
    : RenderWindow(editor, "Editor")
{
    m_RendererContext.isEditor = true;
    m_RendererContext.camera = &editor->data.editorCam;
}

void EditorWindow::Display()
{
    RenderWindow::Display();

    XnorCore::Logger::LogInfo("{}", IsFocused());
    if (IsFocused())
    {
        m_EditorCamera.UpdateCamera(*m_Editor,m_Editor->data.editorCam);
    }
}


