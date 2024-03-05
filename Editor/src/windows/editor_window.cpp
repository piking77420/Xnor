#include "windows/editor_window.hpp"

#include "input/time.hpp"

using namespace XnorEditor; 

EditorWindow::EditorWindow(Editor* editor)
    : RenderWindow(editor, "Editor") , m_EditorCamera(*m_Editor, m_Editor->data.editorCam)
{
    m_RendererContext.isEditor = true;
    m_RendererContext.camera = &editor->data.editorCam;
}

void EditorWindow::Display()
{
    RenderWindow::Display();
    if (IsFocused())
    {
        m_EditorCamera.UpdateCamera();
    }
    m_EditorCamera.OnPressGoToObject();

}

void EditorWindow::Initialize(Vector2i size)
{
    RenderWindow::Initialize(size);
    m_RendererContext.camera = &m_Editor->data.editorCam;
}