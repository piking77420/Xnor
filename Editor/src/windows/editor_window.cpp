#include "windows/editor_window.hpp"


using namespace XnorEditor; 

EditorWindow::EditorWindow(Editor* editor, XnorCore::Viewport& viewport)
    : RenderWindow(editor, "Editor",viewport) , m_EditorCamera(*m_Editor, m_Editor->data.editorCam)
{
   
}

void EditorWindow::Display()
{
    RenderWindow::Display();
    DrawOnTopOfImage();
    if (IsFocused())
    {
        m_EditorCamera.UpdateCamera();
    }

    m_EditorCamera.OnPressGoToObject();
    EditTransform();

    if (ImGui::IsMouseClicked(0))
    {
        const Vector2 mousepos = XnorCore::Utils::FromImVec(ImGui::GetMousePos());
        Vector2i mouseposI = { static_cast<int32_t>(mousepos.x) , static_cast<int32_t>(mousepos.y) };
        mouseposI -= m_Position;
        // Flip the y axis 
        mouseposI.y = m_Size.y - mouseposI.y;
        
        if (mouseposI.x >= m_Size.x || mouseposI.y >= m_Size.y || mouseposI.x < 0 || mouseposI.y < 0)
        {
            XnorCore::Logger::LogInfo("OffScree",mouseposI);
            return;
        }
        XnorCore::Logger::LogInfo("Mouse pos {}",mouseposI);

        XnorCore::Entity* ptr = nullptr;

        if(m_Viewport->GetEntityFromScreen(mouseposI,&ptr))
        {
            XnorCore::Logger::LogInfo("Entity name is {}",ptr->name);
            m_Editor->data.selectedEntity = ptr;
        }
    }
}

void EditorWindow::DrawOnTopOfImage()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.f);
    ImGui::SetCursorPos({25,25});
    if (ImGui::Button("Translation",{25,25}))
    {
        m_TransfromGizmo.currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    }
    ImGui::PopStyleVar();

    ImGui::SetCursorPos({50,25});
    if (ImGui::Button("Rotation",{25,25}))
    {
        m_TransfromGizmo.currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
    }
    
    ImGui::SetCursorPos({75,25});
    if (ImGui::Button("Scale",{25,25}))
    {
        m_TransfromGizmo.currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,5.f);

    ImGui::SetCursorPos({ImGui::GetWindowWidth() - 50,25});
    if (ImGui::Button("Snap",{25,25}))
    {
        m_TransfromGizmo.useSnap = !m_TransfromGizmo.useSnap;
    }
    ImGui::PopStyleVar();
}

void EditorWindow::EditTransform()
{
    if (!m_Editor->data.selectedEntity)
        return;

    const XnorCore::Camera& cam = m_Editor->data.editorCam;
    m_TransfromGizmo.SetRendering(cam,{ static_cast<float_t>(m_Position.x), static_cast<float_t>(m_Position.y) }, m_Size);
    m_TransfromGizmo.Manipulate(*m_Editor->data.selectedEntity);
}
