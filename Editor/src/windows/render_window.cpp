#include "windows/render_window.hpp"

#include "utils/utils.hpp"
#include "world/world.hpp"

using namespace XnorEditor;

RenderWindow::RenderWindow(Editor* editor, XnorCore::Viewport& viewport)
    : UiWindow(editor, "Game Preview"), m_Viewport(&viewport)
{
    m_Viewport->Init(XnorCore::Window::GetSize());
}

RenderWindow::RenderWindow(Editor* editor, const std::string& title, XnorCore::Viewport& viewport)
    : UiWindow(editor, title) , m_Viewport(&viewport)
{
    m_Viewport->Init(XnorCore::Window::GetSize());
}

void RenderWindow::Display()
{
    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_Viewport->GetImage()->GetId()), ImGui::GetContentRegionAvail(),  ImVec2(0, 1), ImVec2(1, 0));
}

void RenderWindow::OnApplicationRendering()
{
    if (!m_Viewport->IsValid() || m_Viewport->viewPortSize != m_Size)
    {
        m_Viewport->OnResize(m_Size);
    }
    
    m_Editor->renderer.RenderViewport(*m_Viewport, *XnorCore::World::scene);
    m_Viewport->ComputePostProcess();
}



