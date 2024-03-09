#include "windows/render_window.hpp"

#include <ImGui/imgui_internal.h>

#include "world/world.hpp"


using namespace XnorEditor;

RenderWindow::RenderWindow(Editor* editor)
    : UiWindow(editor, "Game Preview")
{
    m_Vieport.Init(XnorCore::Window::GetSize());
    m_Vieport.camera = &m_Editor->data.gameCam;
}

RenderWindow::RenderWindow(Editor* editor, const std::string& title)
    : UiWindow(editor, title)
{
    m_Vieport.Init(XnorCore::Window::GetSize());
    m_Vieport.camera = &m_Editor->data.gameCam;
}

void RenderWindow::Display()
{
    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(m_Vieport.finalImage->GetId()), ImGui::GetContentRegionAvail(),  ImVec2(0, 1), ImVec2(1, 0));
}

void RenderWindow::OnApplicationRendering()
{
    if (!m_Vieport.IsValid() || m_Vieport.viewPortSize != m_Size)
    {
        m_Vieport.OnResize(m_Size);
    }
    
    m_Editor->renderer.RenderViewport(m_Vieport, XnorCore::World::scene, XnorCore::World::skybox);
}



