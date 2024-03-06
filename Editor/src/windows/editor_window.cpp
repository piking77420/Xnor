#include "windows/editor_window.hpp"


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
    EditTransform();

}

void EditorWindow::Initialize(const Vector2i size)
{
    RenderWindow::Initialize(size);
    m_RendererContext.camera = &m_Editor->data.editorCam;
}

void EditorWindow::EditTransform()
{
    if (!m_Editor->data.selectedEntity)
        return;

    XnorCore::Transform& transform = m_Editor->data.selectedEntity->transform; 
    const XnorCore::Camera& cam = m_Editor->data.editorCam;
    m_TransfromGizmo.SetRendering(cam,{ static_cast<float_t>(m_Position.x), static_cast<float_t>(m_Position.y) }, m_Size);
    m_TransfromGizmo.Manipulate(transform);
}
